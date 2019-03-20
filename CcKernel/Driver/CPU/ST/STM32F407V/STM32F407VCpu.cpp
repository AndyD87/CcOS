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
 * @brief     Implementation of class STM32F407VCpu
 **/
#include <stm32f4xx_hal.h>
#include "STM32F407VCpu.h"
#include "STM32F407VDriver.h"
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

class STM32F407VCpu::STM32F407VCpuPrivate
{
public:
  bool bThreadChanged = false;
  static CcThreadContext* pMainThreadContext;
  static STM32F407VCpu* pCpu;
  static volatile CcThreadData oMainThreadContext;
};

class STM32F407VCpuThread : public IThread
{
public:
  STM32F407VCpuThread()
    {enterState(EThreadState::Running);}
  virtual void run() override
    {}
};

CcThreadContext* STM32F407VCpu::STM32F407VCpuPrivate::pMainThreadContext = nullptr;
STM32F407VCpu* STM32F407VCpu::STM32F407VCpuPrivate::pCpu = nullptr;
STM32F407VCpuThread oMainThread;
volatile CcThreadData STM32F407VCpu::STM32F407VCpuPrivate::oMainThreadContext(&oMainThread);

volatile CcThreadData* pCurrentThreadContext = &STM32F407VCpu::STM32F407VCpuPrivate::oMainThreadContext;
const uint8 ucMaxSyscallInterruptPriority = 0;

CCEXTERNC void STM32F407VCpu_SysTick()
{
  if(STM32F407VCpu::STM32F407VCpuPrivate::pCpu != nullptr)
  {
    STM32F407VCpu::STM32F407VCpuPrivate::pCpu->SystemTick();
  }
  HAL_IncTick();
}

CCEXTERNC void SysTick_Handler( void )
{
  __asm volatile("  mrs r0, psp                    \n"); // Load Process Stack Pointer, here we are storing our stack
  __asm volatile("  isb                            \n");
  __asm volatile("                                 \n");
  __asm volatile("  ldr  r3, pCurrentThreadContextConst\n"); // Load current thread context
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

  __asm volatile("  bl STM32F407VCpu_SysTick       \n");  // Publish tick to kernel, it could change thread context too.

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
  __asm volatile("pCurrentThreadContextConst: .word pCurrentThreadContext  \n");
}

STM32F407VCpu::STM32F407VCpu()
{
  m_pPrivate = new STM32F407VCpuPrivate();
  CCMONITORNEW(m_pPrivate);
  startSysClock();
  STM32F407VCpuPrivate::pCpu = this;
  STM32F407VCpuPrivate::pMainThreadContext = new CcThreadContext();
  CCMONITORNEW(m_pPrivate->pMainThreadContext);
  STM32F407VCpuPrivate::pMainThreadContext->pThreadObject = &oMainThread;
  STM32F407VCpuPrivate::pMainThreadContext->pContext= (void*)&m_pPrivate->oMainThreadContext;
}

STM32F407VCpu::~STM32F407VCpu()
{
  CCDELETE(m_pPrivate);
}

size_t STM32F407VCpu::coreNumber()
{
  return 1;
}

CcThreadContext* STM32F407VCpu::mainThread()
{
  return m_pPrivate->pMainThreadContext;
}

CcThreadContext* STM32F407VCpu::createThread(IThread* pTargetThread)
{
  CcThreadContext* pReturn = new CcThreadContext();
  pReturn->pThreadObject = pTargetThread;
  CCMONITORNEW(pReturn);
  pReturn->pContext = new CcThreadData(pTargetThread);
  CCMONITORNEW(pReturn->pContext);
  return pReturn;
}

void  STM32F407VCpu::loadThread(CcThreadContext* pTargetThread)
{
  pCurrentThreadContext = static_cast<CcThreadData*>(pTargetThread->pContext);
}

void  STM32F407VCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pThreadData = static_cast<CcThreadData*>(pTargetThread->pContext);
  CCDELETE(pThreadData);
  CCDELETE(pTargetThread);
}

CcStatus STM32F407VCpu::startSysClock()
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
  RCC_OscInitStruct.PLL.PLLM = 8;   // Crystal frequency in MHz
  RCC_OscInitStruct.PLL.PLLN = 336;
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
