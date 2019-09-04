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
#include "CcStatic.h"
#include "CcStringUtil.h"
CCEXTERNC_BEGIN
#include <esp_event.h>
#include <esp_wifi.h>
CCEXTERNC_END

const char ESP8266WlanClient_DefaultSsid[]       = "TestAP";
const char  ESP8266WlanClient_DefaultPassword[]  = "TestPassword";

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

CcStatus ESP8266WlanClient::setState(EState eState)
{
  CcStatus oStatus;
  switch(eState)
  {
    case EState::Run:
    {
      CCDEBUG("ESP8266WlanClient::Run");
      wifi_config_t oWifiConfig;
      if (esp_wifi_get_config(ESP_IF_WIFI_STA, &oWifiConfig) != ESP_OK)
      {
        oStatus = EStatus::ConfigError;
        CCERROR("WlanClient esp_wifi_get_config failed");
      }
      else
      {
        CcStatic::memcpy(oWifiConfig.sta.ssid, ESP8266WlanClient_DefaultSsid, sizeof(ESP8266WlanClient_DefaultSsid));
        CCDEBUG(CcString("SSID:    ") + CCVOIDPTRCONSTCAST(char*, oWifiConfig.sta.ssid));

        CcStatic::memcpy(oWifiConfig.sta.password, ESP8266WlanClient_DefaultPassword, sizeof(ESP8266WlanClient_DefaultPassword));
        CCDEBUG(CcString("Passord: ") + CCVOIDPTRCONSTCAST(char*, oWifiConfig.sta.password));
        if (m_pAdapter->setMode(WIFI_MODE_STA) == false)
        {
          oStatus = EStatus::ConfigError;
          CCERROR("WlanClient Failed to set WiFi mode");
        }
        else
        {
          // Configure WiFi station with host credentials
          // provided during provisioning
          if (esp_wifi_set_config(ESP_IF_WIFI_STA, &oWifiConfig) != ESP_OK)
          {
            oStatus = EStatus::ConfigError;
            CCERROR("WlanClient Failed to set WiFi");
          }
          else
          {
            // Restart WiFi
            if (esp_wifi_start() != ESP_OK)
            {
              oStatus = EStatus::ConfigError;
              CCERROR("WlanClient Failed to restart WiFi");
            }
            else
            {
              // Connect to AP
              if (esp_wifi_connect() != ESP_OK)
              {
                oStatus = EStatus::ConfigError;
                CCERROR("WlanClient Failed to connect WiFi");
              }
            }
          }
        }
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

bool ESP8266WlanClient::event(void *event)
{
  bool bRet = true;
  system_event_t* pEvent = static_cast<system_event_t*>(event);
  switch(pEvent->event_id)
  {
    case SYSTEM_EVENT_STA_START:
      CCDEBUG("WlanClient started");
      break;
    case SYSTEM_EVENT_STA_STOP:
      CCDEBUG("WlanClient stopping");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      CCDEBUG("WlanClient connected");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      CCDEBUG("WlanClient disconnected");
      break;
    default:
      bRet = true;
  }
  return bRet;
}
