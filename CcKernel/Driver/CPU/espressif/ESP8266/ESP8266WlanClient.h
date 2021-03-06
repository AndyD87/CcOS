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
 * @page      ESP8266
 * @subpage   ESP8266WlanClient
 *
 * @page      ESP8266WlanClient
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ESP8266WlanClient
 */
#ifndef H_ESP8266WlanClient_H_
#define H_ESP8266WlanClient_H_

#include "CcBase.h"
#include "Devices/IWlanClient.h"

class ESP8266Wlan;

class ESP8266WlanClient : public IWlanClient
{
public:
  ESP8266WlanClient(ESP8266Wlan* pAdapter);
  virtual ~ESP8266WlanClient() override;
  virtual const CcMacAddress& getMacAddress() override;
  virtual bool isConnected() override;
  virtual CcStatus setState(EState eState) override;
  virtual CcStatus login(const CcString& sSsid, const CcString& sPassord) override;

  bool event(void *event);
private:
  class CPrivate;
private:
  CPrivate*     m_pPrivate = nullptr;
  ESP8266Wlan*  m_pAdapter;
};

#endif // H_ESP8266WlanClient_H_
