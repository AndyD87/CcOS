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
  NVIC_ClearPendingIRQ(USART2_IRQn);
  if(STM32F103Cpu::getCpu() != nullptr)
  {
    STM32F103Cpu::getCpu()->changeThread();
  }
}

/**
 * @brief ISR for System tick with saving thread program state for
 *        changing to next thread if required.
 */
CCEXTERNC void SysTick_Handler( void )
{
	__asm volatile("  mrs r0, psp            \n");
	__asm volatile("  isb                    \n");
	__asm volatile("  ldr	r3, pCurrentThreadContextConst\n");			/* Get the location of the current TCB. */
	__asm volatile("  ldr	r2, [r3]         \n");
    __asm volatile("                         \n");
	__asm volatile("  stmdb r0!, {r4-r11}	 \n");			/* Save the remaining registers. */
	__asm volatile("  str r0, [r2]			 \n");		/* Save the new top of stack into the first member of the TCB. */
    __asm volatile("                         \n");
	__asm volatile("  stmdb sp!, {r3, r14}   \n");
	__asm volatile("  mov r0, #0             \n");
	__asm volatile("  msr basepri, r0        \n");

	__asm volatile("  bl STM32F103Cpu_SysTick  \n");

	__asm volatile("  mov r0, #0             \n");
	__asm volatile("  msr basepri, r0        \n");
	__asm volatile("  ldmia sp!, {r3, r14}   \n");
    __asm volatile("                         \n");
	__asm volatile("  ldr r1, [r3]           \n");
	__asm volatile("  ldr r0, [r1]			 \n");		/* The first item in pxCurrentTCB is the task top of stack. */
	__asm volatile("  ldmia r0!, {r4-r11}	 \n");			/* Pop the registers. */
	__asm volatile("  msr psp, r0            \n");
	__asm volatile("  isb                    \n");
	__asm volatile("  bx r14                 \n");
	__asm volatile("\n");
	__asm volatile(".align 4\n");
    __asm volatile("pCurrentThreadContextConst: .word pCurrentThreadData  \n");
}

/**
 * @brief ISR for Thread tick with saving thread program state for
 *        changing to next thread if required.
 */
CCEXTERNC void USART2_IRQHandler( void )
{
	__asm volatile("  mrs r0, psp            \n");
	__asm volatile("  isb                    \n");
	__asm volatile("  ldr	r3, pCurrentThreadContextConst2\n");			/* Get the location of the current TCB. */
	__asm volatile("  ldr	r2, [r3]         \n");
    __asm volatile("                         \n");
	__asm volatile("  stmdb r0!, {r4-r11}	 \n");			/* Save the remaining registers. */
	__asm volatile("  str r0, [r2]			 \n");		/* Save the new top of stack into the first member of the TCB. */
    __asm volatile("                         \n");
	__asm volatile("  stmdb sp!, {r3, r14}   \n");
	__asm volatile("  mov r0, #0             \n");
	__asm volatile("  msr basepri, r0        \n");

	__asm volatile("  bl STM32F103Cpu_ThreadTick  \n");

	__asm volatile("  mov r0, #0             \n");
	__asm volatile("  msr basepri, r0        \n");
	__asm volatile("  ldmia sp!, {r3, r14}   \n");
    __asm volatile("                         \n");
	__asm volatile("  ldr r1, [r3]           \n");
	__asm volatile("  ldr r0, [r1]			 \n");		/* The first item in pxCurrentTCB is the task top of stack. */
	__asm volatile("  ldmia r0!, {r4-r11}	 \n");			/* Pop the registers. */
	__asm volatile("  msr psp, r0            \n");
	__asm volatile("  isb                    \n");
	__asm volatile("  bx r14                 \n");
	__asm volatile("\n");
	__asm volatile(".align 4\n");
    __asm volatile("pCurrentThreadContextConst2: .word pCurrentThreadData  \n");
}

STM32F103Cpu::STM32F103Cpu() :
  oCpuThreadContext(&oCpuThread, nullptr),
  oCpuThreadData(&oCpuThreadContext)
{
  CCNEW(m_pPrivate, CPrivate);
  pCpu = this;
  oCpuThreadContext.setData(&oCpuThreadData);
  pCurrentThreadContext    = &oCpuThreadContext;
  pCurrentThreadData       = &oCpuThreadData;
  enterCriticalSection();
  leaveCriticalSection();
  startSysClock();
  NVIC_EnableIRQ(USART2_IRQn);
}

STM32F103Cpu::~STM32F103Cpu()
{
  CCDELETE(m_pPrivate);
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
  uint32_t tmp = 0, pllmull = 0, pllsource = 0;

#if defined(STM32F105xC) || defined(STM32F107xC)
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F105xC */

#if defined(STM32F100xB) || defined(STM32F100xE)
  uint32_t prediv1factor = 0;
#endif /* STM32F100xB or STM32F100xE */

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

#if !defined(STM32F105xC) && !defined(STM32F107xC)
      pllmull = ( pllmull >> 18) + 2;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
 #if defined(STM32F100xB) || defined(STM32F100xE)
       prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
       /* HSE oscillator clock selected as PREDIV1 clock entry */
       SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;
 #else
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          SystemCoreClock = HSE_VALUE * pllmull;
        }
 #endif
      }
#else
      pllmull = pllmull >> 18;

      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL multiplication factor = PLL input clock * 6.5 */
        pllmull = 13 / 2;
      }

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {/* PREDIV1 selected as PLL clock entry */

        /* Get PREDIV1 clock source and division factor */
        prediv1source = RCC->CFGR2 & RCC_CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;

        if (prediv1source == 0)
        {
          /* HSE oscillator clock selected as PREDIV1 clock entry */
          SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;
        }
        else
        {/* PLL2 clock selected as PREDIV1 clock entry */

          /* Get PREDIV2 division factor and PLL2 multiplication factor */
          prediv2factor = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & RCC_CFGR2_PLL2MUL) >> 8 ) + 2;
          SystemCoreClock = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;
        }
      }
#endif /* STM32F105xC */
      break;

    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }

  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
  return oStatus;
}
