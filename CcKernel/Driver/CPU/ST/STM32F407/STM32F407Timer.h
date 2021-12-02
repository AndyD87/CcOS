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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407Timer
 */
#pragma once

#include "CcBase.h"
#include "Devices/ITimer.h"
#include "STM32F407.h"

/**
 * @brief Hardware timer on STM32F4
 */
class STM32F407Timer : public ITimer
{
public: //methods
  STM32F407Timer();
  virtual ~STM32F407Timer();

  virtual CcStatus onState(EState eState) override;
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;

  virtual bool timeout() override;

  /**
   * @brief Tick income from ISR
   */
  static void tick();

private: //member
  TIM_HandleTypeDef      m_hTimer;

  static STM32F407Timer* s_Instance;
};
