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
#include "CcGlobalStrings.h"

CCEXTERNC_BEGIN
#include <esp_wifi.h>
#include <esp_log.h>
CCEXTERNC_END

ESP8266Wlan::ESP8266Wlan()
{
  tcpip_adapter_init();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
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

bool ESP8266Wlan::event(void *event)
{
  bool bHandled = false;
  system_event_t* pEvent = static_cast<system_event_t*>(event);
  CCUNUSED(pEvent);
  if(m_pAccessPoint && m_pAccessPoint->event(event))
  {
    bHandled = true;
  }
  else if(m_pClient && m_pClient->event(event))
  {
    bHandled = true;
  }
  return bHandled;
}

bool ESP8266Wlan::setMode(int iMode)
{
  bool bSuccess = false;
  wifi_mode_t eCurrentMode;
  if(esp_wifi_get_mode(&eCurrentMode) == ESP_OK)
  {
    CCDEBUG("Current WLAN Mode: " + CcString::fromNumber(static_cast<uint32>(eCurrentMode)));
    int iCurrentMode = static_cast<int>(eCurrentMode);
    /* Start WiFi in AP mode with configuration built above */
    if (( iMode & iCurrentMode) != 0 ||
        esp_wifi_set_mode(static_cast<wifi_mode_t>(iCurrentMode | iMode)) == ESP_OK)
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}
