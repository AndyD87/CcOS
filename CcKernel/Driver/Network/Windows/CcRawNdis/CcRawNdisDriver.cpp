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
 * @brief     Implementation of Class CcRawNdisDriver
 */

#include "CcRawNdisDriver.h"
#include "CcRawNdisNetwork.h"
#include "CcDevice.h"
#include "CcKernel.h"

class CcRawNdisDriver::CPrivate
{
public:
  CcRawNdisNetwork* pNetworkDevice = nullptr;
};

CcRawNdisDriver::CcRawNdisDriver ()
{
  CCNEW(m_pPrivate, CPrivate);
}

CcRawNdisDriver::~CcRawNdisDriver ()
{
  CCDELETE(m_pPrivate->pNetworkDevice);
  CCDELETE(m_pPrivate);
}

CcStatus CcRawNdisDriver::entry()
{
  CCNEW(m_pPrivate->pNetworkDevice, CcRawNdisNetwork("Intel(R) PRO/1000 MT Desktop Adapter"));
  if (!m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
    CCNEW(m_pPrivate->pNetworkDevice, CcRawNdisNetwork("Ethernet (Kernel Debugger)"));
  }
  if (!m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
    CCNEW(m_pPrivate->pNetworkDevice, CcRawNdisNetwork("Microsoft Kernel Debug Network Adapter"));
  }
  if (!m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
    CCNEW(m_pPrivate->pNetworkDevice, CcRawNdisNetwork("Intel(R) 82579LM Gigabit Network Connection"));
  }
  if (m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    m_pPrivate->pNetworkDevice->start();
    CcKernel::addDevice(CcDevice(m_pPrivate->pNetworkDevice, EDeviceType::Network));
  }
  else
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
  }
  return true;
}

CcStatus CcRawNdisDriver::unload()
{
  if (m_pPrivate->pNetworkDevice != nullptr)
  {
    m_pPrivate->pNetworkDevice->setState(EDeviceState::Stopping);
    CcKernel::removeDevice(CcDevice(m_pPrivate->pNetworkDevice, EDeviceType::Network));
    CCDELETE(m_pPrivate->pNetworkDevice);
  }
  return true;
}
