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
 * @brief     Implementation of class CcIpProtocol
 */
#include "Devices/INetwork.h"
#include "Network/Stack/CcIpProtocol.h"
#include "Network/Stack/CcNetworkStack.h"
#include "Network/Stack/CcTcpProtocol.h"
#include "Network/Stack/CcUdpProtocol.h"
#include "Network/Stack/CcIcmpProtocol.h"
#include "Network/Stack/CcNetworkPacket.h"
#include "Network/NCommonTypes.h"
#include "CcStringList.h"
#include "CcKernel.h"

uint16 CcIpProtocol::s_uiId(0);

CcIpProtocol::CcIpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcIpProtocol::~CcIpProtocol()
{
  clear();
  CCDELETE(m_pIcmpProtocol);
  CCDELETE(m_pTcpProtocol);
  CCDELETE(m_pUdpProtocol);
}

uint16 CcIpProtocol::getProtocolType() const
{
  return NCommonTypes::NNetwork::NEthernet::IP;
}

bool CcIpProtocol::transmit(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  if(pPacket != nullptr && m_pParentProtocol != nullptr)
  {
    bSuccess = true;
    CCNEWTYPE(pIpHeader, CHeader);
    uint8 uiHeaderSize = sizeof(CHeader);
    pIpHeader->uiVersionAndIpHeaderLength = 0x40 | (uiHeaderSize >> 2);
    pIpHeader->uiTypeOfService = 0;
    pIpHeader->uiTotalLength = CcStatic::swapUint16(static_cast<uint16>(pPacket->size()) + uiHeaderSize);
    pIpHeader->uiIdOfFragment = CcStatic::swapUint16(s_uiId++);
    pIpHeader->uiFragmentOffset = 0x0040;
    pIpHeader->uiTimeToLive     = 100;
    pIpHeader->uiProtocol       = static_cast<uint8>(pPacket->uiProtocolType);
    CcStatic::memcpySwapped(pIpHeader->puiDestAddress, pPacket->oTargetIp.getIpV4(), 4);
    CcStatic::memcpySwapped(pIpHeader->puiSourceAddress, pPacket->oSourceIp.getIpV4(), 4);
    pIpHeader->uiHeaderCksum = 0;
    if(IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::uIP))
    {
      pIpHeader->generateChecksum();
    }
    pPacket->transferBegin(pIpHeader, uiHeaderSize);

    pPacket->uiProtocolType = getProtocolType();
    m_pParentProtocol->transmit(pPacket);
;  }
  return bSuccess;
}

bool CcIpProtocol::receive(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  if(pHeader != nullptr)
  {
    pPacket->setPosition(pPacket->getPosition() + pHeader->getHeaderLength());
    pPacket->uiSize = pHeader->getContentLength();
    size_t uiCurrentSize = pPacket->getCurrentSize();
    if (pHeader->getContentLength() > uiCurrentSize)
    {
      CcKernel::message(EMessage::Debug);
    }
    else
    {
      pPacket->oTargetIp.setIpV4(pHeader->puiDestAddress, true);
      if (!pPacket->oTargetIp.isNullIp() &&
          (getNetworkStack()->isInterfaceIpMatching(pPacket->pInterface, pPacket->oTargetIp) ||
          pPacket->oTargetIp.isMulticastIp()))
      {
        pPacket->oSourceIp.setIpV4(pHeader->puiSourceAddress, true);
        uint8 uiProtocol = pHeader->getProtocol();
        for (INetworkProtocol* pProtocol : *this)
        {
          // For types look at https://de.wikipedia.org/wiki/Protokoll_(IP)
          uint16 uiType = pProtocol->getProtocolType();
          if (uiType == uiProtocol)
          {
            if (pProtocol->receive(pPacket))
            {
              bSuccess = true;
              break;
            }
          }
        }
      }
    }
  }
  return bSuccess;
}

uint16 CcIpProtocol::generateChecksum(uint16* pData, size_t uiSize)
{
  uint32 uiTempChecksum = 0;
  // Get number of uint16 chunks from stored number of uint32 chunks in header
  uint16 uiSizeOfIpHeader = static_cast<uint16>(uiSize >> 1);
  for (uint16 uiIpHeaderIterator = 0; uiIpHeaderIterator < uiSizeOfIpHeader; uiIpHeaderIterator++)
  {
    uiTempChecksum += pData[uiIpHeaderIterator];
  }

  if (uiSize & 1)
  {
    uiTempChecksum += CCVOIDPTRCAST(uint8*, pData)[uiSize - 1];
  }

  // Adding all overflows at the end
  uiTempChecksum = (uiTempChecksum & 0xffff) + (uiTempChecksum >> 16);
  uint16 uiChecksum = static_cast<uint16>((uiTempChecksum & 0xffff) + (uiTempChecksum >> 16));
  // Invert Checksum and write to header
  uiChecksum = ~uiChecksum;
  return uiChecksum;
}

uint16 CcIpProtocol::generateChecksumSwapped(uint16* pData, size_t uiSize)
{
  uint32 uiTempChecksum = 0;
  // Get number of uint16 chunks from stored number of uint32 chunks in header
  uint16 uiSizeOfIpHeader = static_cast<uint16>(uiSize >> 1);
  for (uint16 uiIpHeaderIterator = 0; uiIpHeaderIterator < uiSizeOfIpHeader; uiIpHeaderIterator++)
  {
    uiTempChecksum += CcStatic::swapUint16(pData[uiIpHeaderIterator]);
  }

  if (uiSize & 1)
  {
    uiTempChecksum += (static_cast<uint16>(CCVOIDPTRCAST(uint8*, pData)[uiSize - 1]) << 8 );
  }

  // Adding all overflows at the end
  uiTempChecksum = (uiTempChecksum & 0xffff) + (uiTempChecksum >> 16);
  uint16 uiChecksum = static_cast<uint16>((uiTempChecksum & 0xffff) + (uiTempChecksum >> 16));
  // Invert Checksum and write to header
  uiChecksum = ~uiChecksum;
  return uiChecksum;
}

void CcIpProtocol::CHeader::generateChecksum()
{
  uiHeaderCksum = CcIpProtocol::generateChecksum(CCVOIDPTRCAST(uint16*, this), getHeaderLength());
}

bool CcIpProtocol::init()
{
  bool bSuccess = true;
  CCNEW(m_pTcpProtocol, CcTcpProtocol, this);
  bSuccess &= m_pTcpProtocol->init();
  append(m_pTcpProtocol);
  CCNEW(m_pUdpProtocol, CcUdpProtocol, this);
  bSuccess &= m_pUdpProtocol->init();
  append(m_pUdpProtocol);
  CCNEW(m_pIcmpProtocol, CcIcmpProtocol, this);
  append(m_pIcmpProtocol);
  return bSuccess;
}
