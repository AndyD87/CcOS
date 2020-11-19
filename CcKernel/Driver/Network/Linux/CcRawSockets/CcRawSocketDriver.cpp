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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcRawSocketDriver
 */

#include "CcRawSocketDriver.h"
#include "CcRawSocketNetwork.h"
#include "CcDevice.h"
#include "CcKernel.h"

class CcRawSocketDriver::CPrivate
{
public:
  CcRawSocketNetwork* pNetworkDevice = nullptr;
};

CcRawSocketDriver::CcRawSocketDriver ()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcRawSocketDriver::~CcRawSocketDriver ()
{
  CCDELETE(m_pPrivate->pNetworkDevice);
  CCDELETE(m_pPrivate);
}

CcStatus CcRawSocketDriver::entry()
{
  m_pPrivate->pNetworkDevice = new CcRawSocketNetwork("Qualcomm Atheros QCA61x4 Wireless Network Adapter");
  m_pPrivate->pNetworkDevice->getAdapterCount();
  CcKernel::addDevice(CcDevice(m_pPrivate->pNetworkDevice, EDeviceType::Network));
  return true;
}

CcStatus CcRawSocketDriver::unload()
{
  return true;
}
