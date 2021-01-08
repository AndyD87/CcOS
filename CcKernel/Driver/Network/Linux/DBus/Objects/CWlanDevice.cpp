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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CWlanDevice
 */
#include "CWlanDevice.h"
#include "CcKernel.h"
#include "CNetworkManager.h"

#include <iostream>
#include <stdio.h>
#include <dbus/dbus.h>
#include <NetworkManager/NetworkManager.h>

namespace NLinuxDbus
{
class CWlanDevice::CPrivate
{
public:
  CPrivate(CNetworkManager* pNetworkManager, const CcString& sPath) :
    pNetworkManager(pNetworkManager),
    sPath(sPath)
  {}

  CNetworkManager* pNetworkManager;
  CcString sPath;
};

CWlanDevice::CWlanDevice(CNetworkManager *pNetworkManager, const CcString& sPath)
{
  CCNEW(m_pPrivate, CPrivate, pNetworkManager, sPath);
}

CWlanDevice::~CWlanDevice()
{
  CCDELETE(m_pPrivate);
}

IWlanAccessPoint* CWlanDevice::getAccessPoint()
{
  return nullptr;
}

IWlanClient* CWlanDevice::getClient()
{
  return nullptr;
}

IWlan::CCapabilities CWlanDevice::getCapabilities()
{
  return CCapabilities();
}

CcStringList CWlanDevice::getAccessPoints()
{
  return m_pPrivate->pNetworkManager->getWifiAccessPoints(m_pPrivate->sPath);
}

}
