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
 * @subpage   ESP8266Wlan
 *
 * @page      ESP8266Wlan
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ESP8266Wlan
 */
#ifndef H_ESP8266Wlan_H_
#define H_ESP8266Wlan_H_

#include "CcBase.h"
#include "Devices/IWlan.h"

class CcMacAddress;
class ESP8266WlanAccessPoint;
class ESP8266WlanClient;

class ESP8266Wlan : public IWlan
{
public:
  ESP8266Wlan();
  virtual ~ESP8266Wlan();

  virtual IWlanAccessPoint* getAccessPoint() override;
  virtual IWlanClient* getClient() override;
  virtual CCapabilities getCapabilities() override;

  const CcMacAddress& getMacAddress();
private:
  ESP8266WlanAccessPoint* m_pAccessPoint = nullptr;
  ESP8266WlanClient* m_pClient = nullptr;
};

#endif /* H_ESP8266Wlan_H_ */
