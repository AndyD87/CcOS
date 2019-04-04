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
 * @brief     Implementation of class CcTcpProtocol
 */
#include <Network/CcTcpProtocol.h>
#include "NCommonTypes.h"
#include "CcStringList.h"

CcTcpProtocol::CcTcpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcTcpProtocol::~CcTcpProtocol()
{
}

uint16 CcTcpProtocol::getProtocolType() const
{
  return NCommonTypes::NEthernet::NIp::TCP;
}

bool CcTcpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CCUNUSED_TODO(pPacket);
  return bSuccess;
}

bool CcTcpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  for(INetworkProtocol* pProtocol : *this)
  {
    pProtocol->receive(pPacket);
  }
  return bSuccess;
}

bool CcTcpProtocol::initDefaults()
{
  bool bSuccess = true;
  return bSuccess;
}
