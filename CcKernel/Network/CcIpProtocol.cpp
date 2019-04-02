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
#include <Devices/INetwork.h>
#include <Network/CcIpProtocol.h>
#include <Network/CcNetworkStack.h>
#include <Network/CcTcpProtocol.h>
#include <Network/CcUdpProtocol.h>
#include <Network/CcIcmpProtocol.h>
#include <Network/CcNetworkPacket.h>
#include "CcStringList.h"

uint16 CcIpProtocol::s_uiId(0);

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
  if(pPacket != nullptr && m_pParentProtocol != nullptr)
  {
    bSuccess = true;
    CHeader* pIpHeader = new CHeader();
    uint16 uiHeaderSize = sizeof(CHeader);
    pIpHeader->uiVersionAndIpHeaderLength = 0x40 | (uiHeaderSize >> 2);
    pIpHeader->uiTypeOfService = 0;
    pIpHeader->uiTotalLength = CcStatic::swapUint16(uiHeaderSize + pPacket->size());
    pIpHeader->uiIdOfFragment = CcStatic::swapUint16(s_uiId++);
    pIpHeader->uiFragmentOffset = 0x0040;
    pIpHeader->uiTimeToLive     = 100;
    pIpHeader->uiProtocol       = static_cast<uint8>(pPacket->uiProtocolType);
    CcStatic::memcpySwapped(pIpHeader->puiDestAddress, pPacket->oTargetIp.getIpV4(), 4);
    CcStatic::memcpySwapped(pIpHeader->puiSourceAddress, pPacket->oSourceIp.getIpV4(), 4);
    pIpHeader->uiHeaderCksum = 0;
    if(IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::IP))
    {
      pIpHeader->generateChecksum();
    }
    pPacket->transferBegin(pIpHeader, uiHeaderSize);

    pPacket->uiProtocolType = getProtocolType();
    m_pParentProtocol->transmit(pPacket);
;  }
  return bSuccess;
}

bool CcIpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  pPacket->oTargetIp.setIpV4(pHeader->puiDestAddress, true);
  if(!pPacket->oTargetIp.isNullIp() &&
     ( getNetworkStack()->isInterfaceIpMatching(pPacket->pInterface, pPacket->oTargetIp) ||
       pPacket->oTargetIp.isMulticastIp()))
  {
    pPacket->oSourceIp.setIpV4(pHeader->puiSourceAddress, true);
    pPacket->setPosition( pPacket->getPosition() + pHeader->getHeaderLength());
    uint8 uiProtocol = pHeader->getProtocol();
    for(INetworkProtocol* pProtocol : *this)
    {
      // For types look at https://de.wikipedia.org/wiki/Protokoll_(IP)
      uint16 uiType = pProtocol->getProtocolType();
      if (uiType == uiProtocol)
      {
        if(pProtocol->receive(pPacket))
        {
          bSuccess = true;
          break;
        }
      }
    }
  }
  return bSuccess;
}

uint16 CcIpProtocol::generateChecksum(uint16* pData, size_t uiSize)
{
  uint16 uiHeaderCksum = 0;
  uint32 uiTempChecksum = 0;
  uint16 uiSizeOfIpHeader = uiSize >> 1;
  for (uint16 uiIpHeaderIterator = 0; uiIpHeaderIterator < uiSizeOfIpHeader; uiIpHeaderIterator++)
  {
    uiTempChecksum += pData[uiIpHeaderIterator];
  }
  // Adding all overflows at the end
  uiTempChecksum = (uiTempChecksum & 0xffff) + (uiTempChecksum >> 16);
  uint16 uiChecksum = static_cast<uint16>((uiTempChecksum & 0xffff) + (uiTempChecksum >> 16));
  // Invert Checksum and write to header
  uiHeaderCksum = ~uiChecksum;
  return uiHeaderCksum;
}

void CcIpProtocol::CHeader::generateChecksum()
{
  uiHeaderCksum = CcIpProtocol::generateChecksum(CCVOIDPTRCAST(uint16*, this), getHeaderLength());
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
  CcIcmpProtocol* pIcmpProtocol = new CcIcmpProtocol(this);
  append(pIcmpProtocol);
  return bSuccess;
}
