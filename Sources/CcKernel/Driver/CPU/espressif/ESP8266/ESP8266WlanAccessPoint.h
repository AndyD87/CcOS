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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class ESP8266WlanAccessPoint
 */
#pragma once

#include "CcBase.h"
#include "Devices/IWlanAccessPoint.h"

class ESP8266Wlan;

/**
 * @brief Manage wlan access point on ESP8266
 */
class ESP8266WlanAccessPoint : public IWlanAccessPoint
{
public:
  /**
   * @brief Create AP with handle to parent wlan device
   * @param pAdapter: Parent WLAN device
   */
  ESP8266WlanAccessPoint(ESP8266Wlan* pAdapter);
  virtual ~ESP8266WlanAccessPoint() override;

  virtual CcStatus onState(EState eState) override;
  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;
  virtual CcStatus setCredentials(const CcString& sSsid, const CcString& sPassord) override;

  /**
   * @brief Incoming event method from parent to filter for AP related events
   * @param event: Event to parse
   * @return True if event was handled.
   */
  bool event(void *event);
private: // Types
  class CPrivate;
private:
  CPrivate*     m_pPrivate = nullptr;
  ESP8266Wlan*  m_pAdapter;
};
