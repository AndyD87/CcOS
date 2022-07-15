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
 * @brief     Implementation of class STM32F103Cpu
 **/

#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc_ex.h>
#include "STM32F103Cpu.h"
#include "STM32F103Driver.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcGenericThreadHelper.h"
#include "CcStatic.h"
#include "IThread.h"
#include <Driver/CPU/Common/CcThreadData.h>
#include <stdlib.h>

/*-----------------------------------------------------------*/

STM32F103Cpu* STM32F103Cpu::pCpu      = nullptr;  //!< Cpu data initialized on cpu start
volatile CcThreadContext* pCurrentThreadContext = nullptr;  //!< Cpu current running thread Context
volatile CcThreadData* pCurrentThreadData       = nullptr;  //!< Cpu current running thread Data

#ifdef THREADHELPER
CcGenericThreadHelper STM32F103Cpu::oThreadHelper;
#endif

/**
 * @brief ISR for System Tick every 1ms
 */
CCEXTERNC void STM32F103Cpu_SysTick()
{
  HAL_IncTick();
  if(STM32F103Cpu::getCpu() != nullptr)
  {
    STM32F103Cpu::getCpu()->tick();
  }
}

/**
 * @brief ISR for Thread Tick every 10ms
 */
CCEXTERNC void STM32F103Cpu_ThreadTick()
{
  if(STM32F103Cpu::getCpu() != nullptr)
  {
    STM32F103Cpu::getCpu()->changeThread();
  }
  NVIC_ClearPendingIRQ(USART2_IRQn);}

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
  __asm volatile("  stmdb r0!, {r4-r11, r14}       \n"); // Backup Registers to stack of current thread
  __asm volatile("  str r0, [r2]                   \n"); // Backup new stack pointer in thread context
  __asm volatile("                                 \n");
  __asm volatile("  stmdb sp!, {r0, r3}            \n"); // Backup current register state on Main Stack Pointer
  __asm volatile("  mov r0, #0                     \n"); // Disable exceptions
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  dsb                            \n");
  __asm volatile("  isb                            \n");

  __asm volatile("  bl STM32F103Cpu_SysTick        \n");  // Publish tick to kernel, it could change thread context too.

  __asm volatile("  mov r0, #0                     \n");
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  ldmia sp!, {r0, r3}            \n"); // Restore registers from MSP
  __asm volatile("                                 \n");
  __asm volatile("  ldr r1, [r3]                   \n"); // Get back thread context
  __asm volatile("  ldr r0, [r1]                   \n"); // Get back stack pointer form thread context
  __asm volatile("  ldmia r0!, {r4-r11, r14}       \n"); // Get back registers from stack of thread
  __asm volatile("                                 \n");
  __asm volatile("  msr psp, r0                    \n"); // Load stack pointer of thread context
  __asm volatile("  bx r14                         \n"); // continue execution.
  __asm volatile("                                 \n");
  __asm volatile("  .align 4                       \n");
  __asm volatile("pCurrentThreadDataConst: .word pCurrentThreadData  \n");
}

/**
 * @brief ISR for Thread tick with saving thread program state for
 *        changing to next thread if required.
 */
CCEXTERNC void USART2_IRQHandler( void )
{
  __asm volatile("  mrs r0, psp                    \n"); // Load Process Stack Pointer, here we are storing our stack
  __asm volatile("  isb                            \n");
  __asm volatile("                                 \n");
  __asm volatile("  ldr  r3, pCurrentThreadDataConst2\n"); // Load current thread context
  __asm volatile("  ldr  r2, [r3]                  \n"); // Write address of first context to r2
  __asm volatile("                                 \n");
  __asm volatile("  stmdb r0!, {r4-r11, r14}       \n"); // Backup Registers to stack of current thread
  __asm volatile("  str r0, [r2]                   \n"); // Backup new stack pointer in thread context
  __asm volatile("                                 \n");
  __asm volatile("  stmdb sp!, {r0, r3}            \n"); // Backup current register state on Main Stack Pointer
  __asm volatile("  mov r0, #0                     \n"); // Disable exceptions
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  dsb                            \n");
  __asm volatile("  isb                            \n");

  __asm volatile("  bl STM32F103Cpu_ThreadTick     \n");  // Publish tick to kernel, it could change thread context too.

  __asm volatile("  mov r0, #0                     \n");
  __asm volatile("  msr basepri, r0                \n");
  __asm volatile("  ldmia sp!, {r0, r3}            \n"); // Restore registers from MSP
  __asm volatile("                                 \n");
  __asm volatile("  ldr r1, [r3]                   \n"); // Get back thread context
  __asm volatile("  ldr r0, [r1]                   \n"); // Get back stack pointer form thread context
  __asm volatile("  ldmia r0!, {r4-r11, r14}       \n"); // Get back registers from stack of thread
  __asm volatile("                                 \n");
  __asm volatile("  msr psp, r0                    \n"); // Load stack pointer of thread context
  __asm volatile("  bx r14                         \n"); // continue execution.
  __asm volatile("                                 \n");
  __asm volatile("  .align 4                       \n");
  __asm volatile("pCurrentThreadDataConst2: .word pCurrentThreadData  \n");
}

STM32F103Cpu::STM32F103Cpu() :
  oCpuThreadContext(&oCpuThread, nullptr),
  oCpuThreadData(&oCpuThreadContext)
{
  pCpu = this;
  oCpuThreadContext.setData(&oCpuThreadData);
  pCurrentThreadContext    = &oCpuThreadContext;
  pCurrentThreadData       = &oCpuThreadData;
  CcGenericThreadManager::getInstance()->oThreadsRunning.append(&oCpuThreadContext);
  enterCriticalSection();
  leaveCriticalSection();
  startSysClock();
  NVIC_EnableIRQ(USART2_IRQn);
}

STM32F103Cpu::~STM32F103Cpu()
{
}

size_t STM32F103Cpu::coreNumber()
{
  return 1;
}

CcThreadContext* STM32F103Cpu::mainThread()
{
  return &oCpuThreadContext;
}

CcThreadContext* STM32F103Cpu::createThread(IThread* pTargetThread)
{
  CCNEWTYPE(pReturn, CcThreadContext, pTargetThread, nullptr);
  CCNEW(pReturn->pData, CcThreadData, pReturn);
  return pReturn;
}

void  STM32F103Cpu::loadThread(CcThreadContext* pTargetThread)
{
  if(pCurrentThreadContext->pData != nullptr)
  {
    pCurrentThreadContext = pTargetThread;
    pCurrentThreadData    = static_cast<CcThreadData*>(pTargetThread->pData);
  }
}

void  STM32F103Cpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error);
  }
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void STM32F103Cpu::nextThread()
{
  // Do not change thread in isr!
  if(!isInIsr())
  {
    NVIC_SetPendingIRQ(USART2_IRQn);
  }
}

CcThreadContext* STM32F103Cpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool STM32F103Cpu::checkOverflow()
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

void STM32F103Cpu::enterCriticalSection()
{
  __malloc_lock(nullptr);
}

void STM32F103Cpu::leaveCriticalSection()
{
  __malloc_unlock(nullptr);
}

bool STM32F103Cpu::isInIsr()
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

CcStatus STM32F103Cpu::startSysClock()
{
  CcStatus oStatus(false);
  
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
  {
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) == HAL_OK)
    {
      PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
      PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) == HAL_OK)
      {
        oStatus = true;
      }
    }
  }

  return oStatus;
}
