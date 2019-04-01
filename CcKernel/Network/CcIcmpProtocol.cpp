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
 * @brief     Implementation of class CcIcmpProtocol
 */
#include <Network/CcIcmpProtocol.h>
#include <Network/CcTcpProtocol.h>
#include <Network/CcUdpProtocol.h>
#include "CcStringList.h"

CcIcmpProtocol::CcIcmpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcIcmpProtocol::~CcIcmpProtocol()
{
}

uint16 CcIcmpProtocol::getProtocolType() const
{
  return 1;
}

bool CcIcmpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CCUNUSED_TODO(pPacket);
  return bSuccess;
}

bool CcIcmpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  if(!pPacket->oTargetIp.isMulticastIp())
  {
    CHeader* pHeader = CCVOIDPTRCAST(CHeader*, pPacket);
    if(pHeader != nullptr)
    {
      bSuccess = true;
    }
  }
  return bSuccess;
}
