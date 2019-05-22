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
#include "CcGenericThreadHelper.h"
#include "CcStatic.h"
#include "IThread.h"
#include <stdlib.h>

#define STACK_SIZE              4096
#define STACK_OVERFLOW_SPACE      64
#define STACK_OVERFLOW_PATTERN  0xcc

typedef void(*TaskFunction_t)(void* pParam);

class CcThreadData
{
public:
  CcThreadData(CcThreadContext* pThreadContext)
  {
    size_t uiStackSize = (STACK_SIZE > pThreadContext->pThreadObject->getStackSize()) ? STACK_SIZE : pThreadContext->pThreadObject->getStackSize();
    uiStackSize += STACK_OVERFLOW_SPACE;
    puiStack = CCVOIDPTRCAST(uint32_t*, new char[uiStackSize]);
    CCMONITORNEW(puiStack);
    CcStatic::memset(puiStack, STACK_OVERFLOW_PATTERN, STACK_OVERFLOW_SPACE);
    uiStackSize >>= 2;
    uiStackSize--;
    puiTopStack = const_cast<volatile uint32*>(puiStack + uiStackSize);
    initStack(pThreadContext);
  }

  ~CcThreadData()
  {
    if(isOverflowDetectedEx())
    {
      CCCHECKNULL(nullptr);
    }
    CCDELETEARR(puiStack);
  }

  bool isOverflowDetected() volatile
  {
    bool bOverflow = false;
    if(puiStack + (STACK_OVERFLOW_SPACE >> 2) > puiTopStack)
    {
      bOverflow = true;
    }
    return bOverflow;
  }

  bool isOverflowDetectedEx() volatile
  {
    bool bOverflow = false;
    unsigned char* pucBuffer = CCVOIDPTRCAST(unsigned char*, puiStack);
    for(size_t uiPos = 0; uiPos < STACK_OVERFLOW_SPACE; uiPos++)
    {
      if(STACK_OVERFLOW_PATTERN != pucBuffer[uiPos])
        bOverflow = true;
    }
    return bOverflow;
  }

  /*
   * See header file for description.
   */
  void initStack(CcThreadContext* pThread)
  {
    /* Simulate the stack frame as it would be created by a context switch
    interrupt. */

    /* Offset added to account for the way the MCU uses the stack on entry/exit
    of interrupts, and to ensure alignment. */
    puiTopStack--;

    *puiTopStack = 0x01000000; /* xPSR */
    puiTopStack--;
    *puiTopStack = ( ( uint32 ) ICpu::CreateThreadMethod ) & 0xfffffffe;  /* PC */
    puiTopStack--;
    *puiTopStack = ( uint32 ) ICpu::CreateThreadMethod;  /* LR */

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

class STM32F207IGCpu::CPrivate
{
private:
  class STM32F207IGCpuThread : public IThread
  {
  public:
    STM32F207IGCpuThread() :
      IThread("CcOS")
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
  STM32F207IGCpuThread    oCpuThread;
  CcThreadContext         oCpuThreadContext;
  CcThreadData            oCpuThreadData;
  uint32                  uiPrimask = 0;
  static STM32F207IGCpu*  pCpu;
  #ifdef THREADHELPER
  static CcGenericThreadHelper oThreadHelper;
  #endif
};

STM32F207IGCpu* STM32F207IGCpu::CPrivate::pCpu = nullptr;
volatile CcThreadContext* pCurrentThreadContext = nullptr;
volatile CcThreadData* pCurrentThreadData       = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;
#ifdef THREADHELPER
CcGenericThreadHelper STM32F207IGCpu::CPrivate::oThreadHelper;
#endif

CCEXTERNC void STM32F207IGCpu_SysTick()
{
  HAL_IncTick();
  if(STM32F207IGCpu::CPrivate::pCpu != nullptr)
  {
    STM32F207IGCpu::CPrivate::pCpu->tick();
  }
}

CCEXTERNC void STM32F207IGCpu_ThreadTick()
{
  NVIC_ClearPendingIRQ(USART3_IRQn);
  if(STM32F207IGCpu::CPrivate::pCpu != nullptr)
  {
    STM32F207IGCpu::CPrivate::pCpu->changeThread();
  }
}

CCEXTERNC void __malloc_lock ( struct _reent *_r )
{
  CCUNUSED(_r);
  if(STM32F207IGCpu::CPrivate::pCpu != nullptr)
  {
    STM32F207IGCpu::CPrivate::pCpu->enterCriticalSection();
  }
}

CCEXTERNC void __malloc_unlock ( struct _reent *_r )
{
  CCUNUSED(_r);
  if(STM32F207IGCpu::CPrivate::pCpu != nullptr)
  {
    STM32F207IGCpu::CPrivate::pCpu->leaveCriticalSection();
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

  __asm volatile("  bl STM32F207IGCpu_ThreadTick    \n");  // Publish tick to kernel, it could change thread context too.

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

STM32F207IGCpu::STM32F207IGCpu()
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->pCpu = this;
  m_pPrivate->oCpuThreadContext.setData(&m_pPrivate->oCpuThreadData);
  pCurrentThreadContext    = &m_pPrivate->oCpuThreadContext;
  pCurrentThreadData       = &m_pPrivate->oCpuThreadData;

  startSysClock();
  NVIC_EnableIRQ(USART3_IRQn);
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
  return &m_pPrivate->oCpuThreadContext;
}

CcThreadContext* STM32F207IGCpu::createThread(IThread* pTargetThread)
{
  CcThreadContext* pReturn = new CcThreadContext(pTargetThread, nullptr);
  pReturn->pData = new CcThreadData(pReturn);
  CCMONITORNEW(pReturn);
  CCMONITORNEW(pReturn->pData);
  return pReturn;
}

void  STM32F207IGCpu::loadThread(CcThreadContext* pTargetThread)
{
  if(pCurrentThreadContext->pData != nullptr)
  {
    pCurrentThreadContext = pTargetThread;
    pCurrentThreadData    = static_cast<CcThreadData*>(pTargetThread->pData);
  }
}

void  STM32F207IGCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error, "Stack Overflow");
  }
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void STM32F207IGCpu::nextThread()
{
  // Do not change thread in isr!
  if(!isInIsr())
  {
    NVIC_SetPendingIRQ(USART3_IRQn);
  }
}

CcThreadContext* STM32F207IGCpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool STM32F207IGCpu::checkOverflow()
{
  bool bSuccess = true;
  if(pCurrentThreadData->isOverflowDetected())
  {
    bSuccess = false;
  }
  return bSuccess;
}

uint32 g_uiPrimask;
void STM32F207IGCpu::enterCriticalSection()
{
  // Save interrupt mask
  m_pPrivate->uiPrimask = __get_PRIMASK();
  // Disable interrupts
  __set_PRIMASK(1);
}

void STM32F207IGCpu::leaveCriticalSection()
{
  // Restore interrupt mask to possible reenable interrupt
  __set_PRIMASK(m_pPrivate->uiPrimask);
}

bool STM32F207IGCpu::isInIsr()
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

CcStatus STM32F207IGCpu::startSysClock()
{
  CcStatus oStatus(false);
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

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
