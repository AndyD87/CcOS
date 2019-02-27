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
#include "stm32f4xx_hal.h"

STM32F407VSystemTimer *g_SystemTimer;
extern "C" void SysTick_Handler(void)
{
  g_SystemTimer->tick();
  //Trigger Hardware for timeouts
}

STM32F407VSystemTimer::STM32F407VSystemTimer(void):
  m_SystemTime(0)
{
  g_SystemTimer = this;
  m_CountDown = 0;
}

STM32F407VSystemTimer::~STM32F407VSystemTimer(void ){

}

time_t STM32F407VSystemTimer::getTime(void) {
  return m_SystemTime;
}

void STM32F407VSystemTimer::tick(void){
  m_SystemTime++;
}

