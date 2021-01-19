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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcLinuxNetworkStack
 */
#include "CcLinuxNetworkStack.h"
#include "Network/ISocket.h"
#include "CcVector.h"
#include "CcStatic.h"
#include "CcLinuxSocketUdp.h"
#include "CcLinuxSocketTcp.h"

CcLinuxNetworkStack::CcLinuxNetworkStack()
{
}

CcLinuxNetworkStack::~CcLinuxNetworkStack()
{
}

bool CcLinuxNetworkStack::init()
{
  return true;
}

CcIpInterface* CcLinuxNetworkStack::getInterfaceForIp(const CcIp& oIp)
{
  CcIpInterface* pIpSettings = nullptr;
  CCUNUSED(oIp);
  return pIpSettings;
}

ISocket* CcLinuxNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch (eType)
  {
    case ESocketType::TCP:
      CCNEW(pSocket, CcLinuxSocketTcp);
      break;
    case ESocketType::UDP:
      CCNEW(pSocket, CcLinuxSocketUdp);
      break;
    default:
      // Do nothing
      break;
  }
  return pSocket;
}
