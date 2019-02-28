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
#include <stm32f4xx_hal.h>

class STM32F407VSystemTimerPrivate
{
public:
  STM32F407VSystemTimerPrivate(STM32F407VSystemTimer* pParent)
    { s_pParent=pParent; }
  ~STM32F407VSystemTimerPrivate()
    { s_pParent=nullptr; }
  TIM_HandleTypeDef hTimer;
  void tick()
  {
    if(s_pParent != nullptr) s_pParent->timeout();
  }
private:
  static STM32F407VSystemTimer* s_pParent;
};

void TIM2_IRQHandler()
{
  CCDEBUG("DONE");
}

STM32F407VSystemTimer* STM32F407VSystemTimerPrivate::s_pParent(nullptr);

STM32F407VSystemTimer::STM32F407VSystemTimer()
{
  m_pPrivate = new STM32F407VSystemTimerPrivate(this);
  CCMONITORNEW(m_pPrivate);
  __TIM2_CLK_ENABLE();

  m_pPrivate->hTimer.Instance = TIM2;
  m_pPrivate->hTimer.Init.Prescaler = 40000;
  m_pPrivate->hTimer.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  m_pPrivate->hTimer.Init.Period = 500;
  m_pPrivate->hTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  m_pPrivate->hTimer.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&m_pPrivate->hTimer);
}

STM32F407VSystemTimer::~STM32F407VSystemTimer()
{
  HAL_TIM_Base_DeInit(&m_pPrivate->hTimer);
  CCDELETE(m_pPrivate);
}

CcStatus STM32F407VSystemTimer::setTimeout(const CcDateTime& oTimeout)
{
  CcStatus oState;
  m_pPrivate->hTimer.Init.Period = oTimeout.getUSecond();
  return oState;
}

CcStatus STM32F407VSystemTimer::start()
{
  CcStatus oState;
  HAL_TIM_Base_Start(&m_pPrivate->hTimer);
  return oState;
}

CcStatus STM32F407VSystemTimer::stop()
{
  CcStatus oState;
  HAL_TIM_Base_Stop(&m_pPrivate->hTimer);
  return oState;
}

bool STM32F407VSystemTimer::timeout()
{
  bool bReady = CcTimer::timeout();
  if(bReady) stop();
  return bReady;
}
