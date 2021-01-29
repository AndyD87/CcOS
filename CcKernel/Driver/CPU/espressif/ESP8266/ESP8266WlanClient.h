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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ESP8266WlanClient
 */
#pragma once

#include "CcBase.h"
#include "Devices/IWlanClient.h"

class ESP8266Wlan;

/**
 * @brief Wlan client interfaces on ESP8266 boards
 */
class ESP8266WlanClient : public IWlanClient
{
public:
  /**
   * @brief Initialize device with connection to parent interface.
   * @param pAdapter: Parent wlan adapter
   */
  ESP8266WlanClient(ESP8266Wlan* pAdapter);
  virtual ~ESP8266WlanClient() override;
  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;
  virtual CcStatus setState(EState eState) override;
  virtual CcStatus login(const CcString& sSsid, const CcString& sPassord) override;

  /**
   * @brief Incomming event from parent.
   * @param event: Event to parse from client
   * @return True if event was handled successfully.
   */
  bool event(void *event);
private:
  class CPrivate;
private:
  CPrivate*     m_pPrivate = nullptr;
  ESP8266Wlan*  m_pAdapter;
};
