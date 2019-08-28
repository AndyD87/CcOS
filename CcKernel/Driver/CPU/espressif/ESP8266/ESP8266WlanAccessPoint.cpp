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
#include "CcStringUtil.h"
CCEXTERNC_BEGIN
#include <esp_event.h>
#include <esp_wifi.h>
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
#define EXAMPLE_WIFI_SSID "TestSsid"
#define EXAMPLE_WIFI_PASS "TestPassword"

void ESP8266WlanAccessPoint::init()
{
  /* Build WiFi configuration for AP mode */
  wifi_config_t oWifiConfig;
  CcStatic_memsetZeroObject(oWifiConfig);
  oWifiConfig.ap.max_connection = 5;

  CcStatic::memcpy(oWifiConfig.ap.ssid, EXAMPLE_WIFI_SSID, sizeof(EXAMPLE_WIFI_SSID));
  CCDEBUG(CcString("SSID:    ") + CCVOIDPTRCONSTCAST(char*, oWifiConfig.ap.ssid));

  CcStatic::memcpy(oWifiConfig.ap.password, EXAMPLE_WIFI_PASS, sizeof(EXAMPLE_WIFI_PASS));
  CCDEBUG(CcString("Passord: ") + CCVOIDPTRCONSTCAST(char*, oWifiConfig.ap.password));

  oWifiConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
  oWifiConfig.ap.ssid_len = CcStringUtil::strlen(EXAMPLE_WIFI_SSID);

  /* Start WiFi in AP mode with configuration built above */
  if (esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK)
  {
    CCERROR("Failed to set WiFi mode");
  }
  else
  {
    if (ESP_OK != esp_wifi_set_config(ESP_IF_WIFI_AP, &oWifiConfig))
    {
      CCERROR("Failed to set WiFi config");
    }
    else
    {
      if (ESP_OK != esp_wifi_start())
      {
        CCERROR("Failed to start WiFi");
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
      CCDEBUG("SYSTEM_EVENT_STA_START");
      esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      CCDEBUG("SYSTEM_EVENT_STA_GOT_IP");
      CCDEBUG(CcString("Got IP: '%s'") + ip4addr_ntoa(&pEvent->event_info.got_ip.ip_info.ip));

      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      CCDEBUG("SYSTEM_EVENT_STA_DISCONNECTED");
      CCDEBUG(CcString("Disconnect reason : ") + CcString::fromNumber(pEvent->event_info.disconnected.reason));
      break;
    default:
      bHandled = false;
      break;
  }
  return bHandled;
}
