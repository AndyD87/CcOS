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
#include "CcKernel.h"

class CcRawNdisDriver::CPrivate
{
public:
  CcRawNdisNetwork* pNetworkDevice = nullptr;
};

CcRawNdisDriver::CcRawNdisDriver ()
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcRawNdisDriver::~CcRawNdisDriver ()
{
  CCDELETE(m_pPrivate->pNetworkDevice);
  CCDELETE(m_pPrivate);
}

CcStatus CcRawNdisDriver::entry()
{
  m_pPrivate->pNetworkDevice = new CcRawNdisNetwork("Intel(R) PRO/1000 MT Desktop Adapter");
  CCMONITORNEW(m_pPrivate->pNetworkDevice);
  if (!m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
    m_pPrivate->pNetworkDevice = new CcRawNdisNetwork("Ethernet (Kernel Debugger)");
    CCMONITORNEW(m_pPrivate->pNetworkDevice);
  }
  if (!m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
    m_pPrivate->pNetworkDevice = new CcRawNdisNetwork("Microsoft Kernel Debug Network Adapter");
    CCMONITORNEW(m_pPrivate->pNetworkDevice);
  }
  if (!m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    CCDELETE(m_pPrivate->pNetworkDevice);
      m_pPrivate->pNetworkDevice = new CcRawNdisNetwork("Intel(R) 82579LM Gigabit Network Connection");
    CCMONITORNEW(m_pPrivate->pNetworkDevice);
  }
  if (m_pPrivate->pNetworkDevice->isNdisAvailable())
  {
    m_pPrivate->pNetworkDevice->start();
    CcKernel::addDevice(CcDeviceHandle(m_pPrivate->pNetworkDevice, EDeviceType::Network));
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
    CcKernel::removeDevice(CcDeviceHandle(m_pPrivate->pNetworkDevice, EDeviceType::Network));
    CCDELETE(m_pPrivate->pNetworkDevice);
  }
  return true;
}
