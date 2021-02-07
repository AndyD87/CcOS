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
 * @brief     Implementation of class STM32F103Timer
 **/
#include <STM32F103Timer.h>
#include "CcKernel.h"
#include <STM32F103Driver.h>

STM32F103Timer* STM32F103Timer::s_pInstance(nullptr);

/**
 * @brief Timer 2 ISR
 */
CCEXTERNC void TIM2_IRQHandler()
{
  STM32F103Timer::tick();
}

STM32F103Timer::STM32F103Timer()
{
  s_pInstance = this;
  // @todo enable clk required?
  m_hTimer.Instance = TIM2;
  m_hTimer.Init.Prescaler = 1024;
  m_hTimer.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  m_hTimer.Init.Period = (SYSTEM_CLOCK_SPEED / 1024) / 1000;
  m_hTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  m_hTimer.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&m_hTimer);

}

STM32F103Timer::~STM32F103Timer()
{
  HAL_TIM_Base_DeInit(&m_hTimer);
}

CcStatus STM32F103Timer::setTimeout(const CcDateTime& oTimeout)
{
  CcStatus oState;
  m_hTimer.Init.Period = oTimeout.getUSecond();
  return oState;
}

CcStatus STM32F103Timer::setState(EState eState)
{
  CcStatus oStatus(false);
  switch(eState)
  {
    case EState::Run:
    {
      if(HAL_TIM_Base_Start_IT(&m_hTimer) == HAL_OK)
      {
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        oStatus = true;
      }
      break;
    }
    case EState::Stop:
    {
      if(HAL_TIM_Base_Stop(&m_hTimer) == HAL_OK)
      {
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
        oStatus = true;
      }
      break;
    }
    default:
      break;
  }
  if(oStatus)
  {
    oStatus = IDevice::setState(eState);
  }
  return oStatus;
}

bool STM32F103Timer::timeout()
{
  bool bReady = ITimer::timeout();
  if(bReady) stop();
  return bReady;
}

void STM32F103Timer::tick()
{
  if(STM32F103Timer::s_pInstance != nullptr)
    HAL_TIM_IRQHandler(&STM32F103Timer::s_pInstance->m_hTimer);
  s_pInstance->timeout();
}
