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

class ESP8266WlanClient::CPrivate
{
public:
  wifi_config_t oWifiConfig;
};

ESP8266WlanClient::ESP8266WlanClient(ESP8266Wlan* pAdapter) :
  m_pAdapter(pAdapter)
{
  CCNEW(m_pPrivate, CPrivate);
  CcStatic_memsetZeroObject(m_pPrivate->oWifiConfig);
}

ESP8266WlanClient::~ESP8266WlanClient()
{
  CCDELETE(m_pPrivate);
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
      if (m_pAdapter->setMode(WIFI_MODE_STA) == false)
      {
        oStatus = EStatus::ConfigError;
        CCERROR("WlanClient Failed to set WiFi mode");
      }
      else
      {
        // Configure WiFi station with host credentials
        // provided during provisioning
        if (esp_wifi_set_config(ESP_IF_WIFI_STA, &m_pPrivate->oWifiConfig) != ESP_OK)
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
            }
          }
        }
      }
      break;
    }
    case EState::Stop:
    {
      CCDEBUG("ESP8266WlanClient::Stop");
      if (m_pAdapter->removeMode(WIFI_MODE_STA) == false)
      {
        oStatus = EStatus::ConfigError;
        CCERROR("WlanClient Failed to set WiFi mode");
      }
      else
      {
        // Restart WiFi
        if (esp_wifi_start() != ESP_OK)
        {
          oStatus = EStatus::ConfigError;
          CCERROR("WlanClient Failed to restart WiFi");
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

CcStatus ESP8266WlanClient::login(const CcString& sSsid, const CcString& sPassord)
{
  CcStatic::memcpy(m_pPrivate->oWifiConfig.sta.ssid, sSsid.getCharString(), sSsid.length());
  CcStatic::memcpy(m_pPrivate->oWifiConfig.sta.password, sPassord.getCharString(), sPassord.length());
  return restart();
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
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      CCDEBUG("WlanClient ip received");
      tcpip_adapter_ip_info_t oIpInfo;
      if(ESP_OK == tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &oIpInfo))
      {
        m_oInterfaces.append(CcIpInterface());
        m_oInterfaces[0].oIpAddress.setIpV4(CCVOIDPTRCAST(uint8*,&oIpInfo.ip), true);
        m_oInterfaces[0].oGateway.setIpV4(CCVOIDPTRCAST(uint8*,&oIpInfo.gw), true);
        m_oInterfaces[0].setSubnet(CcIp(CCVOIDPTRCAST(uint8*,&oIpInfo.netmask), true));
        CCDEBUG("ESP8266WlanClient::Ip:      " + m_oInterfaces[0].oIpAddress.getString());
        CCDEBUG("ESP8266WlanClient::Gateway: " + m_oInterfaces[0].oGateway.getString());
        CCDEBUG("ESP8266WlanClient::Subnet:  " + m_oInterfaces[0].getSubnetIp().getString());
      }
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      CCDEBUG("WlanClient ip received");
      m_oInterfaces.clear();
      break;
    default:
      bRet = true;
  }
  return bRet;
}
