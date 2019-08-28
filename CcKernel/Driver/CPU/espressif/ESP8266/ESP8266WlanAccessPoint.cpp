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
 * @brief     Implementation of class ESP8266WlanAccessPoint
 **/

#include <Driver/CPU/espressif/ESP8266/ESP8266WlanAccessPoint.h>
#include <Driver/CPU/espressif/ESP8266/ESP8266Wlan.h>
#include "CcStatic.h"
CCEXTERNC_BEGIN
#include <esp_wifi.h>
#include <esp_log.h>
CCEXTERNC_END

ESP8266WlanAccessPoint::~ESP8266WlanAccessPoint()
{
}

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 * The examples use simple WiFi configuration that you can set via
 * 'make menuconfig'.
 * If you'd rather not, just change the below entries to strings
 * with the config you want -
 * ie. #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

void ESP8266WlanAccessPoint::init()
{
  wifi_config_t oWifiConfig;
  if (esp_wifi_get_config(ESP_IF_WIFI_STA, &oWifiConfig) != ESP_OK)
  {
    CCERROR("esp_wifi_get_config failed");
  }
  else
  {
    CcStatic::memcpy(oWifiConfig.sta.ssid, EXAMPLE_WIFI_SSID, sizeof(EXAMPLE_WIFI_SSID));
    CCDEBUG(CcString("SSID:    ") + CCVOIDPTRCONSTCAST(char*, oWifiConfig.sta.ssid));

    CcStatic::memcpy(oWifiConfig.sta.password, EXAMPLE_WIFI_PASS, sizeof(EXAMPLE_WIFI_PASS));
    CCDEBUG(CcString("Passord: ") + CCVOIDPTRCONSTCAST(char*, oWifiConfig.sta.password));
    if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK)
    {
      CCERROR("WIFI Failed to set WiFi mode");
    }
    else
    {
      /* Configure WiFi station with host credentials
       * provided during provisioning */
      if (esp_wifi_set_config(ESP_IF_WIFI_STA, &oWifiConfig) != ESP_OK)
      {
        CCERROR("WIFI Failed to set WiFi configuration");
      }
      else
      {
        /* Restart WiFi */
        if (esp_wifi_start() != ESP_OK)
        {
          CCERROR("WIFI Failed to restart WiFi");
        }
        else
        {
          ///* Connect to AP */
          //if (esp_wifi_connect() != ESP_OK)
          //{
          //  CCERROR("WIFI Failed to connect WiFi");
          //}
        }
      }
    }
  }
}

const CcMacAddress& ESP8266WlanAccessPoint::getMacAddress()
{
  return m_pAdapter->getMacAddress();
}

bool ESP8266WlanAccessPoint::isConnected()
{
  return false;
}

bool ESP8266WlanAccessPoint::event(void *event)
{
  bool bHandled = true;
  system_event_t* pEvent = static_cast<system_event_t*>(event);
  switch(pEvent->event_id)
  {
    case SYSTEM_EVENT_STA_START:
      ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "SYSTEM_EVENT_STA_START");
      esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "SYSTEM_EVENT_STA_GOT_IP");
      ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "Got IP: '%s'", ip4addr_ntoa(&pEvent->event_info.got_ip.ip_info.ip));

      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      ESP_LOGI(CcGlobalStrings::CcOS.getCharString(), "SYSTEM_EVENT_STA_DISCONNECTED");
      ESP_LOGE(CcGlobalStrings::CcOS.getCharString(), "Disconnect reason : %d", pEvent->event_info.disconnected.reason);
      break;
    default:
      bHandled = false;
      break;
  }
  return bHandled;
}
