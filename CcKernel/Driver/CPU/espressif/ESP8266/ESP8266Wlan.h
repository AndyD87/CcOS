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
 *
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class ESP8266Wlan
 */
#pragma once

#include "CcBase.h"
#include "Devices/IWlan.h"
#include "Network/CcMacAddress.h"

class CcMacAddress;
class ESP8266WlanAccessPoint;
class ESP8266WlanClient;

/**
 * @brief Wlan device of ESP8266. It manages Client and AP on ESP8266
 */
class ESP8266Wlan : public IWlan
{
public:
  ESP8266Wlan();
  virtual ~ESP8266Wlan();

  virtual IWlanAccessPoint* getAccessPoint() override;
  virtual IWlanClient* getClient() override;
  virtual CCapabilities getCapabilities() override;

  //! @return Get Physical adddress of adapter
  const CcMacAddress& getMacAddress()
  { return m_oMacAddress; }

  /**
   * @brief Event from ISR subsystem
   * @param event: Event data
   * @return
   */
  bool event(void *event);

  /**
   * @brief Set mode of wlan device
   * @param iMode: Mode as integer
   * @return True if Mode was set successfully
   */
  bool setMode(int iMode);

  /**
   * @brief Remove mode from wlan device
   * @param iMode: Mode as integer
   * @return True if Mode was set successfully
   */
  bool removeMode(int iMode);
private:
  ESP8266WlanAccessPoint* m_pAccessPoint = nullptr;
  ESP8266WlanClient* m_pClient = nullptr;
  CcMacAddress m_oMacAddress;
};
