/*
 *
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
 * @brief     Class CWlanDevice
 */
#pragma once

#include "../ILinuxDbus.h"
#include "Devices/IWlan.h"

class CcStringList;

namespace NLinuxDbus
{
class CNetworkManager;

/**
 * @brief Wlan device on linux dbus system
 */
class CWlanDevice : public IWlan
{
public:
  /**
   * @brief Wlan dbus manager with connection to network manager on dbus
   * @param pNetworkManager:  Parent network manager on dbus
   * @param sPath:            Path on dbus
   */
  CWlanDevice(CNetworkManager* pNetworkManager, const CcString& sPath);
  virtual ~CWlanDevice();

  virtual IWlanAccessPoint* getAccessPoint() override;
  virtual IWlanClient* getClient() override;
  virtual CCapabilities getCapabilities() override;

  //! @return Get list of available accesspoints to connect to.
  CcStringList getAccessPoints();

private:
  class CPrivate;
  CPrivate* m_pPrivate;
};
}
