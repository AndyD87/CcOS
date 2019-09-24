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
 * @brief     Implementation of class CcWindowsNetworkStack
 */
#include "CcWindowsNetworkStack.h"
#include "Network/ISocket.h"
#include "CcVector.h"
#include "CcStatic.h"
#include "CcWindowsSocketUdp.h"
#include "CcWindowsSocketTcp.h"

class CcWindowsNetworkStack::CPrivate
{
public:
  CPrivate(CcWindowsNetworkStack *pParent) :
    pParent(pParent)
  {}

  virtual ~CPrivate()
  {
  }

  CcWindowsNetworkStack *pParent = nullptr;
};

CcWindowsNetworkStack::CcWindowsNetworkStack()
{
  CCNEW(m_pPrivate, CPrivate, this);
}

CcWindowsNetworkStack::~CcWindowsNetworkStack()
{
  CCDELETE(m_pPrivate);
}

bool CcWindowsNetworkStack::init()
{
  return true;
}

CcIpInterface* CcWindowsNetworkStack::getInterfaceForIp(const CcIp& oIp)
{
  CcIpInterface* pIpSettings = nullptr;
  CCUNUSED(oIp);
  return pIpSettings;
}

CcVector<CcIpInterface> CcWindowsNetworkStack::getIpSettingsForInterface(const INetwork* pInterface)
{
  CCUNUSED(pInterface);
  return CcStatic::getNullRef<CcVector<CcIpInterface>>();
}


ISocket* CcWindowsNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch (eType)
  {
    case ESocketType::TCP:
      CCNEW(pSocket, CcWindowsSocketTcp);
      break;
    case ESocketType::UDP:
      CCNEW(pSocket, CcWindowsSocketUdp);
      break;
    default:
      // Do nothing
      break;
  }
  return pSocket;
}
