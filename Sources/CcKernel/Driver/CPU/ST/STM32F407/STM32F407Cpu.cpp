/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class STM32F407Cpu
 **/

#include "stm32f4xx_hal.h"
#include "STM32F407Cpu.h"
#include "STM32F407Driver.h"
#include "CcKernel.h"
#include "CcStatic.h"
#include "CcGenericThreadManager.h"
#include "stdlib.h"

STM32F407Cpu* STM32F407Cpu::pCpu      = nullptr;  //!< Cpu data initialized on cpu start
volatile CcThreadContext* pCurrentThreadContext = nullptr;  //!< Cpu current running thread Context
volatile CcThreadData*    pCurrentThreadData    = nullptr;  //!< Cpu current running thread Data
volatile CcThreadData**   pCurrentThreadDataLocation    = nullptr;  //!< Cpu current running thread Data

#ifdef THREADHELPER
CcGenericThreadHelper STM32F407Cpu::oThreadHelper;
#endif

/**
 * @brief ISR for System Tick every 1ms
 */
CCEXTERNC void STM32F407Cpu_SysTick()
{
  HAL_IncTick();
  if(STM32F407Cpu::getCpu() != nullptr)
  {
    STM32F407Cpu::getCpu()->tick();
  }
}

/**
 * @brief ISR for Thread Tick every 10ms
 */
CCEXTERNC void STM32F407Cpu_ThreadTick()
{
  if(STM32F407Cpu::getCpu() != nullptr)
  {
    STM32F407Cpu::getCpu()->changeThread();
  }
  NVIC_ClearPendingIRQ(USART3_IRQn);
}

/**
 * @brief ISR for System tick with saving thread program state for
 *        changing to next thread if required.
 */
CCEXTERNC void SysTick_Handler( void )
{
  __asm volatile("  mrs r0, psp                    \n"); // Load Process Stack Pointer, here we are storing our stack
  __asm volatile("  isb                            \n");
  __asm volatile("                                 \n");
  __asm volatile("  ldr  r3, pCurrentThreadDataConst\n"); // Load current thread context
  __asm volatile("  ldr  r2, [r3]                  \n"); // Write address of first context to r2
  __asm volatile("                                 \n");
  __asm volatile("  tst r14, #0x10                 \n"); // Is the task using the FPU context?  If so, push high vfp registers.
  __asm volatile("  it eq                          \n");
  __asm volatile("  vstmdbeq r0!, {s16-s31}        \n"); // Backup FPU
  __asm volatile("                                 \n");
  __asm volatile("  stmdb r0!, {r4-r11, r14}       \n"); // Backup Registers to stack of current thread
  __asm volatile("  str r0, [r2]                   \n"); // Backup new stack pointer in thread context
  __asm volatile("                                 \n");
  __asm volatile("  stmdb sp!, {r0, r3}            \n"); // Backup current register state on Main Stack Pointer
  __asm volatile("  mov r0, #0                     \n"); // Disable exceptions
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  dsb                            \n");
  __asm volatile("  isb                            \n");

  __asm volatile("  bl STM32F407Cpu_SysTick       \n");  // Publish tick to kernel, it could change thread context too.

  __asm volatile("  mov r0, #0                     \n");
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  ldmia sp!, {r0, r3}            \n"); // Restore registers from MSP
  __asm volatile("                                 \n");
  __asm volatile("  ldr r1, [r3]                   \n"); // Get back thread context
  __asm volatile("  ldr r0, [r1]                   \n"); // Get back stack pointer form thread context
  __asm volatile("  ldmia r0!, {r4-r11, r14}       \n"); // Get back registers from stack of thread
  __asm volatile("                                 \n");
  __asm volatile("  tst r14, #0x10                 \n"); //******************
  __asm volatile("  it eq                          \n"); // Restore FPU
  __asm volatile("  vldmiaeq r0!, {s16-s31}        \n"); //******************
  __asm volatile("                                 \n");
  __asm volatile("  msr psp, r0                    \n"); // Load stack pointer of thread context
  __asm volatile("  isb                            \n");
  __asm volatile("  bx r14                         \n"); // continue execution.
  __asm volatile("                                 \n");
  __asm volatile("  .align 4                       \n");
  __asm volatile("pCurrentThreadDataConst: .word pCurrentThreadData  \n");
}

/**
 * @brief ISR for Thread tick with saving thread program state for
 *        changing to next thread if required.
 */
CCEXTERNC void USART3_IRQHandler( void )
{
  __asm volatile("  mrs r0, psp                    \n"); // Load Process Stack Pointer, here we are storing our stack
  __asm volatile("  isb                            \n");
  __asm volatile("                                 \n");
  __asm volatile("  ldr  r3, pCurrentThreadContextConst2\n"); // Load current thread context
  __asm volatile("  ldr  r2, [r3]                  \n"); // Write address of first context to r2
  __asm volatile("                                 \n");
  __asm volatile("  tst r14, #0x10                 \n"); //******************
  __asm volatile("  it eq                          \n"); // Backup FPU
  __asm volatile("  vstmdbeq r0!, {s16-s31}        \n"); //******************
  __asm volatile("                                 \n");
  __asm volatile("  stmdb r0!, {r4-r11, r14}       \n"); // Backup Registers to stack of current thread
  __asm volatile("  str r0, [r2]                   \n"); // Backup new stack pointer in thread context
  __asm volatile("                                 \n");
  __asm volatile("  stmdb sp!, {r0, r3}            \n"); // Backup current register state on Main Stack Pointer
  __asm volatile("  mov r0, #0                     \n"); // Disable exceptions
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  dsb                            \n");
  __asm volatile("  isb                            \n");

  __asm volatile("  bl STM32F407Cpu_ThreadTick    \n");  // Publish tick to kernel, it could change thread context too.

  __asm volatile("  mov r0, #0                     \n");
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  ldmia sp!, {r0, r3}            \n"); // Restore registers from MSP
  __asm volatile("                                 \n");
  __asm volatile("  ldr r1, [r3]                   \n"); // Get back thread context
  __asm volatile("  ldr r0, [r1]                   \n"); // Get back stack pointer form thread context
  __asm volatile("  ldmia r0!, {r4-r11, r14}       \n"); // Get back registers from stack of thread
  __asm volatile("                                 \n");
  __asm volatile("  tst r14, #0x10                 \n"); //******************
  __asm volatile("  it eq                          \n"); // Restore FPU
  __asm volatile("  vldmiaeq r0!, {s16-s31}        \n"); //******************
  __asm volatile("                                 \n");
  __asm volatile("  msr psp, r0                    \n"); // Load stack pointer of thread context
  __asm volatile("  bx r14                         \n"); // continue execution.
  __asm volatile("                                 \n");
  __asm volatile("  .align 4                       \n");
  __asm volatile("pCurrentThreadContextConst2: .word pCurrentThreadData  \n");
}

STM32F407Cpu::STM32F407Cpu():
  oCpuThreadContext(&oCpuThread, nullptr),
  oCpuThreadData(&oCpuThreadContext)
{
  pCpu = this;
  oCpuThreadContext.setData(&oCpuThreadData);
  pCurrentThreadContext    = &oCpuThreadContext;
  pCurrentThreadData       = &oCpuThreadData;
  pCurrentThreadDataLocation = &pCurrentThreadData;
  CcGenericThreadManager::getInstance()->oThreadsRunning.append(&oCpuThreadContext);
  enterCriticalSection();
  leaveCriticalSection();
  startSysClock();
  NVIC_EnableIRQ(USART3_IRQn);
}

STM32F407Cpu::~STM32F407Cpu()
{
  CcGenericThreadManager::getInstance()->oThreadsRunning.removeItem(&oCpuThreadContext);
}

size_t STM32F407Cpu::coreNumber()
{
  return 1;
}

CcThreadContext* STM32F407Cpu::mainThread()
{
  return &oCpuThreadContext;
}

CcThreadContext* STM32F407Cpu::createThread(IThread* pTargetThread)
{
  CCNEWTYPE(pReturn, CcThreadContext, pTargetThread, nullptr);
  CCNEW(pReturn->pData, CcThreadData, pReturn);
  return pReturn;
}

void  STM32F407Cpu::loadThread(CcThreadContext* pTargetThread)
{
  if(pCurrentThreadContext->pData != nullptr)
  {
    pCurrentThreadContext = pTargetThread;
    pCurrentThreadData    = static_cast<CcThreadData*>(pTargetThread->pData);
  }
}

void  STM32F407Cpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error);
  }
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void STM32F407Cpu::nextThread()
{
  // Do not change thread in isr!
  if(!isInIsr())
  {
    NVIC_SetPendingIRQ(USART3_IRQn);
  }
}

CcThreadContext* STM32F407Cpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool STM32F407Cpu::checkOverflow()
{
  bool bSuccess = true;
  if(pCurrentThreadData->isOverflowDetected())
  {
    bSuccess = false;
  }
  return bSuccess;
}

CCEXTERNC void __malloc_lock( struct _reent *_r );
CCEXTERNC void __malloc_unlock( struct _reent *_r );

void STM32F407Cpu::enterCriticalSection()
{
  __malloc_lock(nullptr);
}

void STM32F407Cpu::leaveCriticalSection()
{
  __malloc_unlock(nullptr);
}

bool STM32F407Cpu::isInIsr()
{
  bool bRet = false;
  uint32 uiIpsr = __get_IPSR();
  if(uiIpsr != 0)
  {
    bRet = true;
  }
  else
  {
    bRet = false;
  }
  return bRet;
}

CcStatus STM32F407Cpu::startSysClock()
{
  CcStatus oStatus(false);
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;   // Crystal frequency in MHz
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
  {
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) == HAL_OK)
    {
      /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
      if (HAL_GetREVID() == 0x1001)
      {
        /* Enable the Flash prefetch */
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
        HAL_SYSTICK_Config(SYSTEM_CLOCK_SPEED / 1000);
        oStatus = true;
      }
      else
      {
        //! @todo What happens if CPU init failed?
      }
    }
    else
    {
      //! @todo What happens if CPU init failed?
    }
  }
  else
  {
    //! @todo What happens if CPU init failed?
  }
  return oStatus;
}
