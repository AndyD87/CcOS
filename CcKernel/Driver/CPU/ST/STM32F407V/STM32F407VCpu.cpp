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
#include "CcGenericThreadHelper.h"
#include "CcStatic.h"
#include "IThread.h"
#include <stdlib.h>

#define STACK_SIZE              8192
#define STACK_OVERFLOW_SPACE      64
#define STACK_OVERFLOW_PATTERN  0xcc

typedef void(*TaskFunction_t)(void* pParam);

class CcThreadData
{
public:
  CcThreadData(IThread* pThread)
  {
    size_t uiStackSize = (STACK_SIZE > pThread->getStackSize()) ? STACK_SIZE : pThread->getStackSize();
    uiStackSize += STACK_OVERFLOW_SPACE;
    puiStack = static_cast<uint32_t*>(malloc(static_cast<int>(uiStackSize)));
    CCMONITORNEW(puiStack);
    CcStatic::memset(puiStack, STACK_OVERFLOW_PATTERN, STACK_OVERFLOW_SPACE);
    uiStackSize >>= 2;
    uiStackSize--;
    puiTopStack = const_cast<volatile uint32*>(puiStack + uiStackSize);
    initStack(pThread);
  }

  ~CcThreadData()
  {
    CCMONITORDELETE(pStack);
    free(puiStack);
  }

  bool isOverflowDetected() volatile
  {
    bool bOverflow = false;
    if(puiStack + (STACK_OVERFLOW_SPACE >> 2) > puiTopStack)
    {
      bOverflow = true;
    }
    else
    {
      unsigned char* pucBuffer = CCVOIDPTRCAST(unsigned char*, puiStack);
      for(size_t uiPos = 0; uiPos < STACK_OVERFLOW_SPACE; uiPos++)
      {
        if(STACK_OVERFLOW_PATTERN != pucBuffer[uiPos])
          bOverflow = true;
      }
    }
    return bOverflow;
  }

  /*
   * See header file for description.
   */
  void initStack(IThread* pThread)
  {
    /* Simulate the stack frame as it would be created by a context switch
    interrupt. */

    /* Offset added to account for the way the MCU uses the stack on entry/exit
    of interrupts, and to ensure alignment. */
    puiTopStack--;

    *puiTopStack = 0x01000000; /* xPSR */
    puiTopStack--;
    *puiTopStack = ( ( uint32 ) ICpu::CreateThread ) & 0xfffffffe;  /* PC */
    puiTopStack--;
    *puiTopStack = ( uint32 ) ICpu::CreateThread;  /* LR */

    /* Save code space by skipping register initialisation. */
    puiTopStack -= 5;  /* R12, R3, R2 and R1. */
    *puiTopStack = ( uint32 ) pThread; /* R0 */

    /* A save method is being used that requires each task to maintain its
    own exec return value. */
    puiTopStack--;
    *puiTopStack =  0xfffffffd;

    puiTopStack -= 8;  /* R11, R10, R9, R8, R7, R6, R5 and R4. */
  }

  volatile uint32*  puiTopStack = nullptr;
  uint32*  puiStack    = nullptr;
};

/*-----------------------------------------------------------*/

class STM32F407VCpu::CPrivate
{
public:
  static STM32F407VCpu* pCpu;
  #ifdef THREADHELPER
  static CcGenericThreadHelper oThreadHelper;
  #endif
};

class STM32F407VCpuThread : public IThread
{
public:
  STM32F407VCpuThread() :
    IThread("CcOS")
    {enterState(EThreadState::Running);}
  virtual void run() override
    {}
  virtual size_t getStackSize() override
    { return 4; }
};

STM32F407VCpu* STM32F407VCpu::CPrivate::pCpu = nullptr;
STM32F407VCpuThread g_oCpuThread;
CcThreadData        g_oCpuThreadData(&g_oCpuThread);
CcThreadContext     g_oCpuThreadContext(&g_oCpuThread, &g_oCpuThreadData);
volatile CcThreadContext* pCurrentThreadContext = &g_oCpuThreadContext;
volatile CcThreadData* pCurrentThreadData       = &g_oCpuThreadData;
const uint8 ucMaxSyscallInterruptPriority = 0;
#ifdef THREADHELPER
CcGenericThreadHelper STM32F407VCpu::CPrivate::oThreadHelper;
#endif

CCEXTERNC void STM32F407VCpu_SysTick()
{
  HAL_IncTick();
  if(STM32F407VCpu::CPrivate::pCpu != nullptr)
  {
    STM32F407VCpu::CPrivate::pCpu->m_bIsrActive = true;
    STM32F407VCpu::CPrivate::pCpu->tick();
    STM32F407VCpu::CPrivate::pCpu->m_bIsrActive = false;
  }
}

CCEXTERNC void STM32F407VCpu_ThreadTick()
{
  NVIC_ClearPendingIRQ(USART3_IRQn);
  if(STM32F407VCpu::CPrivate::pCpu != nullptr)
  {
    STM32F407VCpu::CPrivate::pCpu->m_bIsrActive = true;
    STM32F407VCpu::CPrivate::pCpu->changeThread();
    STM32F407VCpu::CPrivate::pCpu->m_bIsrActive = false;  
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
  __asm volatile("pCurrentThreadContextConst: .word pCurrentThreadData  \n");
}

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

  __asm volatile("  bl STM32F407VCpu_ThreadTick    \n");  // Publish tick to kernel, it could change thread context too.

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

STM32F407VCpu::STM32F407VCpu()
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
  CPrivate::pCpu = this;

  startSysClock();
  NVIC_EnableIRQ(USART3_IRQn);
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
  return &g_oCpuThreadContext;
}

CcThreadContext* STM32F407VCpu::createThread(IThread* pTargetThread)
{
  CcThreadContext* pReturn = new CcThreadContext();
  pReturn->pThreadObject = pTargetThread;
  CCMONITORNEW(pReturn);
  pReturn->pData = new CcThreadData(pTargetThread);
  CCMONITORNEW(pReturn->pData);
  #ifdef THREADHELPER
  CPrivate::oThreadHelper.insert((void*)pReturn, (void*)((CcThreadData*)pReturn->pContext)->puiTopStack, "Name");
  #endif
  return pReturn;
}

void  STM32F407VCpu::loadThread(CcThreadContext* pTargetThread)
{
  pCurrentThreadContext = pTargetThread;
  pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  #ifdef THREADHELPER
  CPrivate::oThreadHelper.current(pTargetThread);
  #endif
}

void  STM32F407VCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error, "Stack Overflow");
  }
  #ifdef THREADHELPER
  CPrivate::oThreadHelper.current(pTargetThread);
  #endif
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void STM32F407VCpu::nextThread()
{
  if(m_bIsrActive == false)
  	NVIC_SetPendingIRQ(USART3_IRQn);
}

CcThreadContext* STM32F407VCpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool STM32F407VCpu::checkOverflow()
{
  bool bSuccess = true;
  if(pCurrentThreadData->isOverflowDetected())
  {
    bSuccess = false;
  }
  return bSuccess;
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
