/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CNetworkManager
 */
#include "CNetworkManager.h"
#include "CcKernel.h"
#include "CcByteArray.h"
#include "IThread.h"

#include <iostream>
#include <stdio.h>
#include <dbus/dbus.h>
#include <NetworkManager/NetworkManager.h>

namespace NLinuxDbus
{
class CNetworkManager::CPrivate : public IThread
{
public:
  CPrivate() :
    IThread("CNetworkManager"),
    sPath("/org/freedesktop/NetworkManager"),
    sDbus("org.freedesktop.NetworkManager"),
    sInterface("org.freedesktop.NetworkManager"),
    sMethod("GetAllDevices")
  {}
  virtual ~CPrivate() = default;

  virtual void run() override
  {
  }

  CcString sPath;
  CcString sDbus;
  CcString sInterface;
  CcString sMethod;
};

CNetworkManager::CNetworkManager() :
  ILinuxDbus("org.freedesktop.NetworkManager",
             "/org/freedesktop/NetworkManager",
             "org.freedesktop.NetworkManager")
{
  CCNEW(m_pPrivate, CPrivate);
  connect();
}

CNetworkManager::~CNetworkManager()
{
  CCDELETE(m_pPrivate);
}

CcStringList CNetworkManager::getDevices()
{
  CcStringList oDevices;
  CcLinuxDbusArguments oResult;
  if(call("GetAllDevices", CcLinuxDbusArguments(), oResult))
  {
    if(oResult.size() > 0 &&
       oResult[0].getType() == CcVariant::EType::VariantList)
    {
      CcVariantList oList = oResult[0].getVariantList();
      for(CcVariant& oItem : oList)
      {
        oDevices.append(oItem.getString());
      }
    }
  }
  return oDevices;
}

int CNetworkManager::getDeviceType(const CcString& sDevice)
{
  int iType = 0;
  CcVariant oValue = property("DeviceType", ".Device", sDevice);
  if(!oValue.isInt())
  {
    iType = oValue.getInt();
  }
  return iType;
}

CcStringList CNetworkManager::getWifiAccessPoints(const CcString& sDevice)
{
  CcStringList oDevices;
  CcLinuxDbusArguments oResult;
  if(call("GetAllAccessPoints", CcLinuxDbusArguments(), oResult, sDevice, ".Device.Wireless"))
  {
    for(CcVariant& oItem : oResult)
    {
      oDevices.append(oItem.getString());
    }
  }
  return oDevices;
}
}
