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
 * @brief     Implementation of class ESP8266Timer
 **/

#include "ESP8266Timer.h"
#include "ESP8266GpioPort.h"
#include "ESP8266Driver.h"
#include "CcByteArray.h"
#include "CcStatic.h"

CCEXTERNC_BEGIN
#include "FreeRTOS.h"
#include "esp_system.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "driver/hw_timer.h"
#include "esp8266/pin_mux_register.h"
CCEXTERNC_END

ESP8266Timer::ESP8266Timer()
{
  hw_timer_init(ESP8266Timer::timeoutEvent, this);
}

ESP8266Timer::~ESP8266Timer()
{
  hw_timer_deinit();
}

CcStatus ESP8266Timer::onState(EState eState)
{
  CcStatus oStatus;
  switch(eState)
  {
    case EState::Run:
    {
      hw_timer_alarm_us(1000, true);
      hw_timer_enable(true);
      break;
    }
    case EState::Stop:
    {
      hw_timer_enable(false);
      break;
    }
    default:
      break;
  }
  return oStatus;
}

CcStatus ESP8266Timer::setTimeout(const CcDateTime& oTimeout)
{
  CcStatus oStatus;
  m_oTimeout = oTimeout;
  return oStatus;
}

void ESP8266Timer::timeoutEvent(void* pArgv)
{
  ESP8266Timer* pArg = static_cast<ESP8266Timer*>(pArgv);
  CCUNUSED(pArg);
  if(pArg)
    pArg->timeout();
}
