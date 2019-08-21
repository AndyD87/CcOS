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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class STM32F303VCT6Cpu
 **/
#include <stm32f3xx_hal.h>
#include "Driver/CPU/Common/CcThreadData.h"
#include "STM32F303VCT6Cpu.h"
#include "STM32F303VCT6Driver.h"
#include "CcKernel.h"
#include "IThread.h"

typedef void(*TaskFunction_t)(void* pParam);

/*-----------------------------------------------------------*/

class STM32F303VCT6Cpu::STM32F303VCT6CpuPrivate
{
public:
  bool bThreadChanged = false;
  static CcThreadContext* pMainThreadContext;
  static STM32F303VCT6Cpu* pCpu;
};

class STM32F303VCT6CpuThread : public IThread
{
public:
  STM32F303VCT6CpuThread()
    {enterState(EThreadState::Running);}
  virtual void run() override
    {}
};

CcThreadContext* STM32F303VCT6Cpu::STM32F303VCT6CpuPrivate::pMainThreadContext = nullptr;
STM32F303VCT6Cpu* STM32F303VCT6Cpu::STM32F303VCT6CpuPrivate::pCpu = nullptr;
volatile CcThreadData* pCurrentThreadContext = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;

CCEXTERNC void STM32F303VCT6Cpu_SysTick()
{
  if(STM32F303VCT6Cpu::STM32F303VCT6CpuPrivate::pCpu != nullptr)
  {
    HAL_IncTick();
    STM32F303VCT6Cpu::STM32F303VCT6CpuPrivate::pCpu->tick();
  }
}

CCEXTERNC void SysTick_Handler( void )
{
  __asm volatile("  mrs r0, psp                    \n"); // Load Process Stack Pointer, here we are storing our stack
  __asm volatile("  isb                            \n");
  __asm volatile("                                 \n");
  __asm volatile("  ldr  r3, pCurrentThreadContextConst\n"); // Load current thread context
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
  __asm volatile("pCurrentThreadContextConst: .word pCurrentThreadContext  \n");
}

STM32F303VCT6Cpu::STM32F303VCT6Cpu()
{
  CCNEW(m_pPrivate, STM32F303VCT6CpuPrivate);
  STM32F303VCT6CpuPrivate::pCpu = this;

  CCNEW(m_pPrivate->pMainThreadContext, CcThreadContext);
  STM32F303VCT6CpuPrivate::pMainThreadContext->pThreadObject = new STM32F303VCT6CpuThread();
  STM32F303VCT6CpuPrivate::pMainThreadContext->pContext= (void*)(new CcThreadData(STM32F303VCT6CpuPrivate::pMainThreadContext->pThreadObject));
  pCurrentThreadContext = (CcThreadData*)STM32F303VCT6CpuPrivate::pMainThreadContext->pContext;
  startSysClock();
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
  return m_pPrivate->pMainThreadContext;
}

CcThreadContext* STM32F303VCT6Cpu::createThread(IThread* pTargetThread)
{
  CCNEWTYPE(pReturn, CcThreadContext);
  pReturn->pThreadObject = pTargetThread;
  CCNEW(pReturn->pContext, CcThreadData, pTargetThread);
  return pReturn;
}

void  STM32F303VCT6Cpu::loadThread(CcThreadContext* pTargetThread)
{
  pCurrentThreadContext = static_cast<CcThreadData*>(pTargetThread->pContext);
}

void  STM32F303VCT6Cpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pThreadData = static_cast<CcThreadData*>(pTargetThread->pContext);
  CCDELETE(pThreadData);
  CCDELETE(pTargetThread);
}

void STM32F303VCT6Cpu::nextThread()
{
  // @todo Create an interrupt for changing thread
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
