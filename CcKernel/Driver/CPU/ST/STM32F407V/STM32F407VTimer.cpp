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
 * @brief     Implementation of class STM32F407VTimer
 **/
#include <STM32F407VTimer.h>
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include <STM32F407VDriver.h>

class STM32F407VTimerPrivate
{
public:
  STM32F407VTimerPrivate(STM32F407VTimer* pParent)
    { m_pParent=pParent; s_Instance = this;}
  ~STM32F407VTimerPrivate()
    { m_pParent=nullptr; }
  TIM_HandleTypeDef hTimer;
  static void tick()
  {
    s_Instance->m_pParent->timeout();
  }
  static STM32F407VTimerPrivate* s_Instance;
private:
  STM32F407VTimer* m_pParent;
};

STM32F407VTimerPrivate* STM32F407VTimerPrivate::s_Instance(nullptr);

CCEXTERNC void TIM2_IRQHandler()
{
  if(STM32F407VTimerPrivate::s_Instance != nullptr) HAL_TIM_IRQHandler(&STM32F407VTimerPrivate::s_Instance->hTimer);
  STM32F407VTimerPrivate::tick();
}

STM32F407VTimer::STM32F407VTimer()
{
  m_pPrivate = new STM32F407VTimerPrivate(this);
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

STM32F407VTimer::~STM32F407VTimer()
{
  HAL_TIM_Base_DeInit(&m_pPrivate->hTimer);
  CCDELETE(m_pPrivate);
}

CcStatus STM32F407VTimer::setTimeout(const CcDateTime& oTimeout)
{
  CcStatus oState;
  m_pPrivate->hTimer.Init.Period = oTimeout.getUSecond();
  return oState;
}

CcStatus STM32F407VTimer::start()
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

CcStatus STM32F407VTimer::stop()
{
  CcStatus oState(false);
  if(HAL_TIM_Base_Stop(&m_pPrivate->hTimer) == HAL_OK)
  {
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
    oState = true;
  }
  return oState;
}

bool STM32F407VTimer::timeout()
{
  bool bReady = ITimer::timeout();
  if(bReady) stop();
  return bReady;
}
