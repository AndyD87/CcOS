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
 *
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F103Timer
 */
#pragma once

#include "CcBase.h"
#include "Devices/ITimer.h"
#include "STM32F103.h"

/**
 * @brief Timer device for STM32F103
 */
class STM32F103Timer : public ITimer
{
public: //methods
  STM32F103Timer();
  virtual ~STM32F103Timer();

  virtual CcStatus setState(EState eState) override;
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;

  virtual bool timeout() override;

  /**
   * @brief Incoming tick from system timer ISR
   */
  static void tick();
private: //member
  static STM32F103Timer*  s_pInstance;
  TIM_HandleTypeDef       m_hTimer;
};
