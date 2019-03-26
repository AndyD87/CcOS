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
 * @brief     Implementation of class STM32F207IGCpu
 **/
#include <STM32F2xx_hal.h>
#include "STM32F207IGCpu.h"
#include "STM32F207IGDriver.h"
#include "CcKernel.h"
#include "IThread.h"

#define STACK_SIZE 1024

typedef void(*TaskFunction_t)(void* pParam);

CCEXTERNC void CreateThread(void* pParam)
{
  IThread *pThreadObject = static_cast<IThread *>(pParam);
  if (pThreadObject->getThreadState() == EThreadState::Starting)
  {
    pThreadObject->enterState(EThreadState::Running);
    pThreadObject->run();
    pThreadObject->enterState(EThreadState::Stopped);
    pThreadObject->onStopped();
  }
  else
  {
    // Do net create threads wich are not in starting state
    pThreadObject->enterState(EThreadState::Stopped);
  }
  // @todo force thread switch
  while(1);
}

class CcThreadData
{
public:
  CcThreadData(IThread* pThread)
  {
    puiTopStack = aStack + STACK_SIZE - 1;
    pxPortInitialiseStack(pThread);
  }

  /*
   * See header file for description.
   */
  void  pxPortInitialiseStack(IThread* pThread)
  {
    /* Simulate the stack frame as it would be created by a context switch
    interrupt. */

    /* Offset added to account for the way the MCU uses the stack on entry/exit
    of interrupts, and to ensure alignment. */
    puiTopStack--;

    *puiTopStack = 0x01000000; /* xPSR */
    puiTopStack--;
    *puiTopStack = ( ( uint32 ) CreateThread ) & 0xfffffffe;  /* PC */
    puiTopStack--;
    *puiTopStack = ( uint32 ) CreateThread;  /* LR */

    /* Save code space by skipping register initialisation. */
    puiTopStack -= 5;  /* R12, R3, R2 and R1. */
    *puiTopStack = ( uint32 ) pThread; /* R0 */

    /* A save method is being used that requires each task to maintain its
    own exec return value. */
    puiTopStack--;
    *puiTopStack =  0xfffffffd;

    puiTopStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
  }

  volatile uint32* puiTopStack = nullptr;
  volatile uint32  aStack[STACK_SIZE];
};

/*-----------------------------------------------------------*/

class STM32F207IGCpu::STM32F207IGCpuPrivate
{
public:
  bool bThreadChanged = false;
  static CcThreadContext* pMainThreadContext;
  static STM32F207IGCpu* pCpu;
};

class STM32F207IGCpuThread : public IThread
{
public:
  STM32F207IGCpuThread()
    {enterState(EThreadState::Running);}
  virtual void run() override
    {}
};

CcThreadContext* STM32F207IGCpu::STM32F207IGCpuPrivate::pMainThreadContext = nullptr;
STM32F207IGCpu* STM32F207IGCpu::STM32F207IGCpuPrivate::pCpu = nullptr;
volatile CcThreadData* pCurrentThreadContext = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;

CCEXTERNC void STM32F207IGCpu_SysTick()
{
  if(STM32F207IGCpu::STM32F207IGCpuPrivate::pCpu != nullptr)
  {
    HAL_IncTick();
    STM32F207IGCpu::STM32F207IGCpuPrivate::pCpu->tick();
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

  __asm volatile("  bl STM32F207IGCpu_SysTick       \n");  // Publish tick to kernel, it could change thread context too.

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

STM32F207IGCpu::STM32F207IGCpu()
{
  m_pPrivate = new STM32F207IGCpuPrivate();
  CCMONITORNEW(m_pPrivate);
  STM32F207IGCpuPrivate::pCpu = this;
  STM32F207IGCpuPrivate::pMainThreadContext = new CcThreadContext();

  CCMONITORNEW(m_pPrivate->pMainThreadContext);
  STM32F207IGCpuPrivate::pMainThreadContext->pThreadObject = new STM32F207IGCpuThread();
  STM32F207IGCpuPrivate::pMainThreadContext->pContext= (void*)(new CcThreadData(STM32F207IGCpuPrivate::pMainThreadContext->pThreadObject));
  pCurrentThreadContext = (CcThreadData*)STM32F207IGCpuPrivate::pMainThreadContext->pContext;
  startSysClock();
}

STM32F207IGCpu::~STM32F207IGCpu()
{
  CCDELETE(m_pPrivate);
}

size_t STM32F207IGCpu::coreNumber()
{
  return 1;
}

CcThreadContext* STM32F207IGCpu::mainThread()
{
  return m_pPrivate->pMainThreadContext;
}

CcThreadContext* STM32F207IGCpu::createThread(IThread* pTargetThread)
{
  CcThreadContext* pReturn = new CcThreadContext();
  pReturn->pThreadObject = pTargetThread;
  CCMONITORNEW(pReturn);
  pReturn->pContext = new CcThreadData(pTargetThread);
  CCMONITORNEW(pReturn->pContext);
  return pReturn;
}

void  STM32F207IGCpu::loadThread(CcThreadContext* pTargetThread)
{
  pCurrentThreadContext = static_cast<CcThreadData*>(pTargetThread->pContext);
}

void  STM32F207IGCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pThreadData = static_cast<CcThreadData*>(pTargetThread->pContext);
  CCDELETE(pThreadData);
  CCDELETE(pTargetThread);
}

void STM32F207IGCpu::nextThread()
{
  // @todo Create an interrupt for changing thread
}

CcStatus STM32F207IGCpu::startSysClock()
{
  CcStatus oStatus(false);
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
      HAL_SYSTICK_Config(SYSTEM_CLOCK_SPEED / 1000);
      oStatus = true;
    }
  }

  return oStatus;
}
