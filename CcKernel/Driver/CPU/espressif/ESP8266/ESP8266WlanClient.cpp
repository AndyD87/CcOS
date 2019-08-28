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
 * @brief     Implementation of class ESP8266WlanClient
 **/

#include "Driver/CPU/espressif/ESP8266/ESP8266WlanClient.h"
#include "Driver/CPU/espressif/ESP8266/ESP8266Wlan.h"
CCEXTERNC_BEGIN
#include <driver/gpio.h>
CCEXTERNC_END

ESP8266WlanClient::~ESP8266WlanClient()
{
}

const CcMacAddress& ESP8266WlanClient::getMacAddress()
{
  return m_pAdapter->getMacAddress();
}

bool ESP8266WlanClient::isConnected()
{
  return false;
}
