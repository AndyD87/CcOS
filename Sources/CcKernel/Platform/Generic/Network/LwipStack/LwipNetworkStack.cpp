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
 * @brief     Implementation of class LwipNetworkStack
 */
#include "Network/LwipStack/LwipNetworkStack.h"
#include "Network/ISocket.h"
#include "CcVector.h"
#include "CcStatic.h"
#include "Network/LwipStack/LwipSocketUdp.h"
#include "Network/LwipStack/LwipSocketTcp.h"

class LwipNetworkStack::CPrivate
{
public:
  CPrivate(LwipNetworkStack *pParent) :
    pParent(pParent)
  {}

  virtual ~CPrivate()
  {
  }

  LwipNetworkStack *pParent = nullptr;
};

LwipNetworkStack::LwipNetworkStack()
{
  CCNEW(m_pPrivate, CPrivate, this);
  lwip_socket_init();
}

LwipNetworkStack::~LwipNetworkStack()
{
  CCDELETE(m_pPrivate);
}

bool LwipNetworkStack::init()
{
  return true;
}

const CcIpInterface* LwipNetworkStack::getInterfaceForIp(const CcIp& oIp) const
{
  CcIpInterface* pIpSettings = nullptr;
  CCUNUSED(oIp);
  return pIpSettings;
}

ISocket* LwipNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch (eType)
  {
    case ESocketType::TCP:
      CCNEW(pSocket, LwipSocketTcp);
      break;
    case ESocketType::UDP:
      CCNEW(pSocket, LwipSocketUdp);
      break;
    default:
      // Do nothing
      break;
  }
  return pSocket;
}
