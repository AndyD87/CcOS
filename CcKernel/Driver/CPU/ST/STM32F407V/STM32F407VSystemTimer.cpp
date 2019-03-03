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
#include <STM32F407VDriver.h>

class STM32F407VSystemTimerPrivate
{
public:
  STM32F407VSystemTimerPrivate(STM32F407VSystemTimer* pParent)
    { m_pParent=pParent; s_Instance = this;}
  ~STM32F407VSystemTimerPrivate()
    { m_pParent=nullptr; }
  TIM_HandleTypeDef hTimer;
  static void tick()
  {
    s_Instance->m_pParent->timeout();
  }
  static STM32F407VSystemTimerPrivate* s_Instance;
private:
  STM32F407VSystemTimer* m_pParent;
};

STM32F407VSystemTimerPrivate* STM32F407VSystemTimerPrivate::s_Instance(nullptr);

extern "C" void TIM2_IRQHandler()
{
  if(STM32F407VSystemTimerPrivate::s_Instance != nullptr) HAL_TIM_IRQHandler(&STM32F407VSystemTimerPrivate::s_Instance->hTimer);
  STM32F407VSystemTimerPrivate::tick();
}

STM32F407VSystemTimer::STM32F407VSystemTimer()
{
  m_pPrivate = new STM32F407VSystemTimerPrivate(this);
  CCMONITORNEW(m_pPrivate);
  __TIM2_CLK_ENABLE();

  m_pPrivate->hTimer.Instance = TIM2;
  m_pPrivate->hTimer.Init.Prescaler = 1024;
  m_pPrivate->hTimer.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  m_pPrivate->hTimer.Init.Period = (SYSTEM_CLOCK_SPEED / 1024) / 1000;
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
  CcStatus oState(false);
  if(HAL_TIM_Base_Start_IT(&m_pPrivate->hTimer) == HAL_OK)
  {
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    oState = true;
  }
  return oState;
}

CcStatus STM32F407VSystemTimer::stop()
{
  CcStatus oState(false);
  if(HAL_TIM_Base_Stop(&m_pPrivate->hTimer) == HAL_OK)
  {
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
    oState = true;
  }
  return oState;
}

bool STM32F407VSystemTimer::timeout()
{
  bool bReady = ITimer::timeout();
  if(bReady) stop();
  return bReady;
}
