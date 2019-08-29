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

const char ESP8266WlanAccessPoint_DefaultSsid[]       = "TestSsid";
const char  ESP8266WlanAccessPoint_DefaultPassword[]  = "TestPassword";

class ESP8266WlanAccessPoint::CPrivate
{
public:
  wifi_config_t oWifiConfig;
};

ESP8266WlanAccessPoint::ESP8266WlanAccessPoint(ESP8266Wlan* pAdapter) : m_pAdapter(pAdapter)
{
  CCNEW(m_pPrivate, CPrivate);
  CcStatic_memsetZeroObject(m_pPrivate->oWifiConfig);
  m_pPrivate->oWifiConfig.ap.max_connection = 5;

  CcStatic::memcpy(m_pPrivate->oWifiConfig.ap.ssid, ESP8266WlanAccessPoint_DefaultSsid, sizeof(ESP8266WlanAccessPoint_DefaultSsid));
  CCDEBUG(CcString("SSID:    ") + CCVOIDPTRCONSTCAST(char*, m_pPrivate->oWifiConfig.ap.ssid));

  CcStatic::memcpy(m_pPrivate->oWifiConfig.ap.password, ESP8266WlanAccessPoint_DefaultPassword, sizeof(ESP8266WlanAccessPoint_DefaultPassword));
  CCDEBUG(CcString("Passord: ") + CCVOIDPTRCONSTCAST(char*, m_pPrivate->oWifiConfig.ap.password));

  m_pPrivate->oWifiConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
  m_pPrivate->oWifiConfig.ap.ssid_len = CcStringUtil::strlen(ESP8266WlanAccessPoint_DefaultSsid);

}

ESP8266WlanAccessPoint::~ESP8266WlanAccessPoint()
{
  CCDELETE(m_pPrivate);
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
  /* Start WiFi in AP mode with configuration built above */
  if (esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK)
  {
    CCERROR("Failed to set WiFi mode");
  }
  else
  {
    if (ESP_OK != esp_wifi_set_config(ESP_IF_WIFI_AP, &m_pPrivate->oWifiConfig))
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
    case SYSTEM_EVENT_AP_START:
    {
      CNetworkEvent oEvent(CNetworkEvent::EType::Started, this);
      callNetworkEventHandler(&oEvent);
      break;
    }
    case SYSTEM_EVENT_AP_STOP:
    {
      CNetworkEvent oEvent(CNetworkEvent::EType::Stopped, this);
      callNetworkEventHandler(&oEvent);
      break;
    }
    case SYSTEM_EVENT_AP_STACONNECTED:
    {
      CNetworkEvent oEvent(CNetworkEvent::EType::Connected, this);
      callNetworkEventHandler(&oEvent);
      break;
    }
    case SYSTEM_EVENT_AP_STADISCONNECTED:
    {
      CNetworkEvent oEvent(CNetworkEvent::EType::Disconnected, this);
      callNetworkEventHandler(&oEvent);
      break;
    }
    default:
      bHandled = false;
      break;
  }
  return bHandled;
}
