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
 * @brief     Class ESP8266Timer
 */
#pragma once

#include "CcBase.h"
#include "CcDateTime.h"
#include "Devices/ITimer.h"

/**
 * @brief Timer module for ESP8266
 */
class ESP8266Timer : public ITimer
{
public:
  ESP8266Timer();
  virtual ~ESP8266Timer();

  virtual CcStatus onState(EState eState) override;
  virtual CcStatus setTimeout(const CcDateTime& oTimeout) override;

  /**
   * @brief Interrupt income event.
   * @param pArgv: Arguments with context from isr
   */
  static void timeoutEvent(void* pArgv);

private:
  CcDateTime m_oTimeout;
};
