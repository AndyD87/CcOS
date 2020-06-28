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
#include "CcDeviceHandle.h"
#include "Devices/IWlan.h"
#include "CWlanDevice.h"
#include "CcGlobalStrings.h"

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
  CcVector<CcDeviceHandle> oDevices;
};

CNetworkManager::CNetworkManager() :
  ILinuxDbus("org.freedesktop.NetworkManager",
             "/org/freedesktop/NetworkManager",
             "org.freedesktop.NetworkManager")
{
  CCNEW(m_pPrivate, CPrivate);
}

CNetworkManager::~CNetworkManager()
{
  while(m_pPrivate->oDevices.size())
  {
    IDevice* pDevice = m_pPrivate->oDevices[0].getDevice<IDevice>();
    CCDELETE(pDevice);
    m_pPrivate->oDevices.remove(0);
  }
  CCDELETE(m_pPrivate);
}

void CNetworkManager::init()
{
  connect();
  CcStringList oList = getDevices();
  for(CcString& sDevice : oList)
  {
    CcString sPath = sDevice;
    sPath.replace(getPath() + CcGlobalStrings::Seperators::Slash, "");
    switch(getDeviceType(sPath))
    {
      case NLinuxDbus::CNetworkManager::EDeviceType::NM_DEVICE_TYPE_GENERIC:
        break;
      case NLinuxDbus::CNetworkManager::EDeviceType::NM_DEVICE_TYPE_ETHERNET:
        break;
      case NLinuxDbus::CNetworkManager::EDeviceType::NM_DEVICE_TYPE_WIFI:
      {
        IWlan* pWlan = new CWlanDevice(this, sPath);
        m_pPrivate->oDevices.append(CcDeviceHandle(pWlan, ::EDeviceType::Wlan));
        CcKernel::addDevice(m_pPrivate->oDevices.last());
        break;
      }
      default:
        break;
    }
  }
}

void CNetworkManager::deinit()
{
  disconnect();
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

CNetworkManager::EDeviceType CNetworkManager::getDeviceType(const CcString& sDevice)
{
  EDeviceType iType = EDeviceType::NM_DEVICE_TYPE_UNKNOWN;
  CcLinuxDbusArguments oValue = property("DeviceType", ".Device", sDevice);
  if(oValue.size() > 0)
  {
    if(oValue[0].isUint())
    {
      iType = static_cast<EDeviceType>(oValue[0].getUint());
    }
  }
  return iType;
}

CcStringList CNetworkManager::getWifiAccessPoints(const CcString& sDevice)
{
  CcStringList oDevices;
  CcLinuxDbusArguments oResult;
  if(call("GetAllAccessPoints", CcLinuxDbusArguments(), oResult, sDevice, ".Device.Wireless"))
  {
    if(oResult.size() > 0 &&
       oResult[0].getType() == CcVariant::EType::VariantList)
    {
      for(CcVariant& oItem : oResult[0].getVariantList())
      {
        CcLinuxDbusArguments oSsid = property("Ssid", ".AccessPoint", oItem.getString());
        CcString sSsid;
        if(oSsid.size() > 0 &&
           oSsid[0].getType() == CcVariant::EType::VariantList)
        {
          for(CcVariant& rValue : oSsid[0].getVariantList())
          {
            sSsid.append(rValue.getInt8());
          }
        }
      }
    }
  }
  return oDevices;
}
}
