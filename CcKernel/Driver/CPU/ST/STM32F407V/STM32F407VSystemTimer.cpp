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
  static int s_iInterruptMeasurement;
  static int s_iStackPointer;
private:
  STM32F407VSystemTimer* m_pParent;
};

STM32F407VSystemTimer::STM32F407VSystemTimerPrivate* STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_Instance(nullptr);
int STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_iInterruptMeasurement(0);

#define __ASM __asm /*!< asm keyword for GNU Compiler */
#define __INLINE inline /*!< inline keyword for GNU Compiler */
#define __STATIC_INLINE static inline

/**
 * @brief Get Link Register
 * @details Returns the current value of the Link Register (LR).
 * @return LR Register value
 * @https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2017/03/22/how_to_read_the_link-Z7Bq
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t __get_LR(void)
{
  register uint32_t result;
  __ASM volatile ("MOV %0, LR\n" : "=r" (result) );
  return(result);
}

//#define __get_PSP()                 (__arm_rsr("PSP"))

CCEXTERNC __attribute__ ((naked)) void SysTick_Handler()
{
  int iCallstackVar = 0;
  int* piCallstackPointer = &iCallstackVar;
  while(STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_iInterruptMeasurement)
  {
    if(*piCallstackPointer == STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_iInterruptMeasurement)
    {
      iCallstackVar = (&iCallstackVar) - piCallstackPointer;
      STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_iInterruptMeasurement = 0;
      STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::s_iStackPointer = __get_LR();
    }
  }
  STM32F407VSystemTimer::STM32F407VSystemTimerPrivate::tick();
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
        // Start Interrupt Stack Measurement
        m_pPrivate->s_iInterruptMeasurement = (int)&m_pPrivate->s_iInterruptMeasurement;
        while(m_pPrivate->s_iInterruptMeasurement);
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

CcStatus STM32F407VSystemTimer::setTimeout(const CcDateTime& oTimeout)
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
