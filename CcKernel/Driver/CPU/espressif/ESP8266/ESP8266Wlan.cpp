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
 * @brief     Implementation of class ESP8266Wlan
 **/

#include "Driver/CPU/espressif/ESP8266/ESP8266Wlan.h"
#include "Driver/CPU/espressif/ESP8266/ESP8266WlanClient.h"
#include "Driver/CPU/espressif/ESP8266/ESP8266WlanAccessPoint.h"
CCEXTERNC_BEGIN
#include <driver/gpio.h>
CCEXTERNC_END


ESP8266Wlan::ESP8266Wlan()
{
}

ESP8266Wlan::~ESP8266Wlan()
{
  CCDELETE(m_pAccessPoint);
  CCDELETE(m_pClient);
}

IWlanAccessPoint* ESP8266Wlan::getAccessPoint()
{
  if(m_pAccessPoint == nullptr)
  {
    CCNEW(m_pAccessPoint, ESP8266WlanAccessPoint, this);
  }
  return m_pAccessPoint;
}

IWlanClient* ESP8266Wlan::getClient()
{
  if(m_pClient == nullptr)
  {
    CCNEW(m_pClient, ESP8266WlanClient, this);
  }
  return m_pClient;
}

IWlan::CCapabilities ESP8266Wlan::getCapabilities()
{
  return CCapabilities(CCapabilities::AccesssPoint | CCapabilities::Client);
}

const CcMacAddress& ESP8266Wlan::getMacAddress()
{
  return CcMacAddress();
}
