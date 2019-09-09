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
  m_pPrivate->oWifiConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
}

ESP8266WlanAccessPoint::~ESP8266WlanAccessPoint()
{
  CCDELETE(m_pPrivate);
}

CcStatus ESP8266WlanAccessPoint::setState(EState eState)
{
  CcStatus oStatus;
  switch(eState)
  {
    case EState::Run:
    {
      CCDEBUG("ESP8266WlanAccessPoint::Run");
      // Start WiFi in AP mode with configuration built above */
      if (m_pAdapter->setMode(WIFI_MODE_AP) == false)
      {
        oStatus = EStatus::ConfigError;
        CCERROR("WlanAccessPoint Failed to set WiFi mode");
      }
      else
      {
        if (ESP_OK != esp_wifi_set_config(ESP_IF_WIFI_AP, &m_pPrivate->oWifiConfig))
        {
          oStatus = EStatus::ConfigError;
          CCERROR("WlanAccessPoint Failed to set WiFi config");
        }
        else
        {
          if (ESP_OK != esp_wifi_start())
          {
            oStatus = EStatus::ConfigError;
            CCERROR("WlanAccessPoint Failed to start WiFi");
          }
        }
      }
      break;
    }
    case EState::Stop:
    {
      CCDEBUG("ESP8266WlanAccessPoint::Run");
      // Start WiFi in AP mode with configuration built above */
      if (m_pAdapter->removeMode(WIFI_MODE_AP) == false)
      {
        oStatus = EStatus::ConfigError;
        CCERROR("WlanAccessPoint Failed to set WiFi mode");
      }
      else
      {
        if (ESP_OK != esp_wifi_start())
        {
          oStatus = EStatus::ConfigError;
          CCERROR("WlanAccessPoint Failed to start WiFi");
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

const CcMacAddress& ESP8266WlanAccessPoint::getMacAddress()
{
  return m_pAdapter->getMacAddress();
}

bool ESP8266WlanAccessPoint::isConnected()
{
  return false;
}

CcStatus ESP8266WlanAccessPoint::setCredentials(const CcString& sSsid, const CcString& sPassord)
{
  CcStatic::memcpy(m_pPrivate->oWifiConfig.ap.ssid, sSsid.getCharString(), sSsid.length());
  m_pPrivate->oWifiConfig.ap.ssid_len = sSsid.length();
  CcStatic::memcpy(m_pPrivate->oWifiConfig.ap.password, sPassord.getCharString(), sPassord.length());
  return restart();
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
