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
 * @brief     Implementation of class CcIpProtocol
 */
#include <Network/CcIpProtocol.h>
#include <Network/CcTcpProtocol.h>
#include <Network/CcUdpProtocol.h>
#include "CcStringList.h"

CcIpProtocol::CcIpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcIpProtocol::~CcIpProtocol()
{
}

uint16 CcIpProtocol::getProtocolType() const
{
  return 0x0800;
}

bool CcIpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CCUNUSED_TODO(pPacket);
  return bSuccess;
}

bool CcIpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  uint8 uiProtocol = getProtocol(pHeader);
  for(INetworkProtocol* pProtocol : *this)
  {
    uint16 uiType = pProtocol->getProtocolType();
    if (uiType == uiProtocol)
    {
      pProtocol->receive(pPacket);
      break;
    }
  }
  return bSuccess;
}

void CcIpProtocol::generateChecksum(CHeader* pHeader)
{
  uint16* pIpChecksumStart = (uint16*) pHeader;
  pHeader->uiHeaderCksum = 0;
  uint32 uiTempChecksum = 0;
  uint16 uiSizeOfIpHeader = getHeaderLength(pHeader);
  for (uint16 uiIpHeaderIterator = 0; uiIpHeaderIterator < uiSizeOfIpHeader / 2; uiIpHeaderIterator++)
  {
    uiTempChecksum += pIpChecksumStart[uiIpHeaderIterator];
  }
  // Adding all overflows at the end
  uiTempChecksum = (uiTempChecksum & 0xffff) + (uiTempChecksum >> 16);
  uint16 uiChecksum = (uint16) ((uiTempChecksum & 0xffff) + (uiTempChecksum >> 16));
  // Invert Checksum and write to header
  pHeader->uiHeaderCksum = ~uiChecksum;
}

bool CcIpProtocol::initDefaults()
{
  bool bSuccess = false;
  CcTcpProtocol* pTcpProtocol = new CcTcpProtocol(this);
  pTcpProtocol->initDefaults();
  append(pTcpProtocol);
  CcUdpProtocol* pUdpProtocol = new CcUdpProtocol(this);
  pUdpProtocol->initDefaults();
  append(pUdpProtocol);
  return bSuccess;
}
