/**
 * @author     Andreas Dirmeier
 * @copyright  Andreas Dirmeier (c) 2015
 * @par       Language: C++11
 */
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class STM32F407VSystemTimer
 **/
#include <STM32F407VSystemTimer.h>
#include "CcKernel.h"
#include "CcStatic.h"
#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_pwr_ex.h>
#include <STM32F407VDriver.h>

class STM32F407VSystemTimer::STM32F407VSystemTimerPrivate
{
public:
  STM32F407VSystemTimerPrivate(STM32F407VSystemTimer* pParent)
    { m_pParent=pParent; s_Instance = this;}
  ~STM32F407VSystemTimerPrivate()
    { m_pParent=nullptr; }

  static void tick()
  {
    s_Instance->m_pParent->timeout();
  }
  static STM32F407VSystemTimerPrivate* s_Instance;
private:
  STM32F407VSystemTimer* m_pParent;
};

typedef struct
{
  volatile uint32* puiTopStack;
  uint32  aRegisters[256];
} SThreadContext;

volatile SThreadContext oCurrentThreadContext;
volatile SThreadContext* pCurrentThreadContext = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;

STM32F407VSystemTimer::STM32F407VSystemTimerPrivate* STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_Instance(nullptr);
CCEXTERNC void SwitchContext()
{
  STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::tick();
}

#define AV __asm volatile

CCEXTERNC void SysTick_Handler( void )
{
#if 1
  /* This is a naked function. */
    AV("  mrs r0, msp                    \n");
    AV("  isb                            \n");
    AV("                                 \n");
    AV("  ldr  r3, pCurrentThreadContextConst     \n"); /* Get the location of the current TCB. */
    AV("  ldr  r2, [r3]                  \n");
    AV("                                 \n");
    AV("  tst r14, #0x10                 \n"); /* Is the task using the FPU context?  If so, push high vfp registers. */
    AV("  it eq                          \n");
    AV("  vstmdbeq r0!, {s16-s31}        \n");
    AV("                                 \n");
    AV("  stmdb r0!, {r4-r11, r14}       \n"); /* Save the remaining registers. */
    AV("  str r0, [r2]                   \n"); /* Save the new top of stack into the first member of the TCB. */
    AV("                                 \n");
    AV("  stmdb sp!, {r0, r3}            \n");
    AV("  mov r0, #0                     \n");
    AV("  msr basepri, r0                \n");
    AV("  dsb                            \n");
    AV("  isb                            \n");
    AV("  bl SwitchContext               \n");  // Go back to main application.
    AV("  mov r0, #0                     \n");
    AV("  msr basepri, r0                \n");
    AV("  ldmia sp!, {r0, r3}            \n");
    AV("                                 \n"); /* Restore the context. */
    AV("  ldr r1, [r3]                   \n");
    AV("  ldr r0, [r1]                   \n"); /* The first item in the TCB is the task top of stack. */
    AV("  ldmia r0!, {r4-r11, r14}       \n"); /* Pop the registers that are not automatically saved on exception entry. */
    AV("                                 \n");
    AV("  tst r14, #0x10                 \n"); /* Is the task using the FPU context?  If so, pop the high vfp registers too. */
    AV("  it eq                          \n");
    AV("  vldmiaeq r0!, {s16-s31}        \n");
    AV("                                 \n");
    AV("  msr msp, r0                    \n");
    AV("  bx r14                         \n");
    AV("                                 \n");
    AV("  .align 4                       \n");
    AV("pCurrentThreadContextConst: .word pCurrentThreadContext  \n");
#else
  SwitchContext();
#endif
}

STM32F407VSystemTimer::STM32F407VSystemTimer()
{
  m_pPrivate = new STM32F407VSystemTimerPrivate(this);
  CCMONITORNEW(m_pPrivate);

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
        CcStatic_memsetZeroPointer(const_cast<SThreadContext*>(pCurrentThreadContext));
        pCurrentThreadContext = &oCurrentThreadContext;
        oCurrentThreadContext.puiTopStack = &oCurrentThreadContext.aRegisters[128];
        AV("ldr   r1, =_estack");
        AV("msr   psp, r1");
        AV("msr   msp, r1");
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
}

STM32F407VSystemTimer::~STM32F407VSystemTimer()
{
  CCDELETE(m_pPrivate);
}

CcStatus STM32F407VSystemTimer::setTimeout(const CcDateTime&)
{
  return false;
}

CcStatus STM32F407VSystemTimer::start()
{
  return false;
}

CcStatus STM32F407VSystemTimer::stop()
{
  return false;
}
