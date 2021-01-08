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
 * @brief     Implementation of class STM32F303VCT6Cpu
 **/
#include <stm32f3xx_hal.h>
#include "Driver/CPU/Common/CcThreadData.h"
#include "STM32F303VCT6Cpu.h"
#include "STM32F303VCT6Driver.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcGenericThreadHelper.h"
#include "CcStatic.h"
#include "IThread.h"
#include <stdlib.h>

typedef void(*TaskFunction_t)(void* pParam);

class STM32F303VCT6Cpu::CPrivate
{
private:
  class STM32F303VCT6CpuThread : public IThread
  {
  public:
    STM32F303VCT6CpuThread() :
      IThread(CcGlobalStrings::CcOS)
      {enterState(EThreadState::Running);}
    virtual void run() override
      {}
    virtual size_t getStackSize() override
      { return 4; }
  };
public:
  CPrivate() :
    oCpuThreadContext(&oCpuThread, nullptr),
    oCpuThreadData(&oCpuThreadContext)
  {
  }

public:
  STM32F303VCT6CpuThread    oCpuThread;
  CcThreadContext         oCpuThreadContext;
  CcThreadData            oCpuThreadData;
  static STM32F303VCT6Cpu*  pCpu;
  #ifdef THREADHELPER
  static CcGenericThreadHelper oThreadHelper;
  #endif
};

STM32F303VCT6Cpu* STM32F303VCT6Cpu::CPrivate::pCpu = nullptr;
volatile CcThreadContext* pCurrentThreadContext = nullptr;
volatile CcThreadData* pCurrentThreadData       = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;
#ifdef THREADHELPER
CcGenericThreadHelper STM32F303VCT6Cpu::CPrivate::oThreadHelper;
#endif

CCEXTERNC void STM32F303VCT6Cpu_SysTick()
{
  HAL_IncTick();
  if(STM32F303VCT6Cpu::CPrivate::pCpu != nullptr)
  {
    STM32F303VCT6Cpu::CPrivate::pCpu->tick();
  }
}

CCEXTERNC void STM32F303VCT6Cpu_ThreadTick()
{
  NVIC_ClearPendingIRQ(USART3_IRQn);
  if(STM32F303VCT6Cpu::CPrivate::pCpu != nullptr)
  {
    STM32F303VCT6Cpu::CPrivate::pCpu->changeThread();
  }
}

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

  __asm volatile("  bl STM32F303VCT6Cpu_SysTick       \n");  // Publish tick to kernel, it could change thread context too.

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

CCEXTERNC void USART3_IRQHandler( void )
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

  __asm volatile("  bl STM32F303VCT6Cpu_ThreadTick    \n");  // Publish tick to kernel, it could change thread context too.

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

STM32F303VCT6Cpu::STM32F303VCT6Cpu()
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->pCpu = this;
  m_pPrivate->oCpuThreadContext.setData(&m_pPrivate->oCpuThreadData);
  pCurrentThreadContext    = &m_pPrivate->oCpuThreadContext;
  pCurrentThreadData       = &m_pPrivate->oCpuThreadData;
  enterCriticalSection();
  leaveCriticalSection();
  startSysClock();
  NVIC_EnableIRQ(USART3_IRQn);
}

STM32F303VCT6Cpu::~STM32F303VCT6Cpu()
{
  CCDELETE(m_pPrivate);
}

size_t STM32F303VCT6Cpu::coreNumber()
{
  return 1;
}

CcThreadContext* STM32F303VCT6Cpu::mainThread()
{
  return &m_pPrivate->oCpuThreadContext;
}

CcThreadContext* STM32F303VCT6Cpu::createThread(IThread* pTargetThread)
{
  CCNEWTYPE(pReturn, CcThreadContext, pTargetThread, nullptr);
  CCNEW(pReturn->pData, CcThreadData, pReturn);
  return pReturn;
}

void  STM32F303VCT6Cpu::loadThread(CcThreadContext* pTargetThread)
{
  if(pCurrentThreadContext->pData != nullptr)
  {
    pCurrentThreadContext = pTargetThread;
    pCurrentThreadData    = static_cast<CcThreadData*>(pTargetThread->pData);
  }
}

void  STM32F303VCT6Cpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error);
  }
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void STM32F303VCT6Cpu::nextThread()
{
  // Do not change thread in isr!
  if(!isInIsr())
  {
    NVIC_SetPendingIRQ(USART3_IRQn);
  }
}

CcThreadContext* STM32F303VCT6Cpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool STM32F303VCT6Cpu::checkOverflow()
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

void STM32F303VCT6Cpu::enterCriticalSection()
{
  __malloc_lock(nullptr);
}

void STM32F303VCT6Cpu::leaveCriticalSection()
{
  __malloc_unlock(nullptr);
}

bool STM32F303VCT6Cpu::isInIsr()
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
CcStatus STM32F303VCT6Cpu::startSysClock()
{
  CcStatus oStatus(false);
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  clocks dividers */
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
      HAL_SYSTICK_Config(SYSTEM_CLOCK_SPEED / 1000);
      oStatus = true;
    }
  }

  return oStatus;
}
