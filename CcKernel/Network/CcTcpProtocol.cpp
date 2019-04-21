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
#include "Network/CcTcpProtocol.h"
#include "Network/CcNetworkSocketTcp.h"
#include "Network/CcNetworkStack.h"
#include "Network/CcIpProtocol.h"
#include "Devices/INetwork.h"
#include "NCommonTypes.h"
#include "CcList.h"
#include "CcIpSettings.h"

void CcTcpProtocol::CHeader::generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp, uint16 uiLength, void* pData)
{
  uint32 uiTmpChecksum = 0;
  uint16* puData = static_cast<uint16*>(pData);
  uiTmpChecksum += (oDestIp.getIpV4_3() << 8) + oDestIp.getIpV4_2();
  uiTmpChecksum += (oDestIp.getIpV4_1() << 8) + oDestIp.getIpV4_0();
  uiTmpChecksum += (oSourceIp.getIpV4_3() << 8) + oSourceIp.getIpV4_2();
  uiTmpChecksum += (oSourceIp.getIpV4_1() << 8) + oSourceIp.getIpV4_0();
  uiTmpChecksum += NCommonTypes::NNetwork::NEthernet::NIp::UDP;
  uint32 uiUdpSize = getHeaderLength();
  uiTmpChecksum += uiUdpSize + uiLength;
  uint16* pUdpFrame = CCVOIDPTRCAST(uint16*, this);
  for (uint32 uiSize = 0; uiSize < uiUdpSize; uiSize += 2)
  {
    uiTmpChecksum += CcStatic::swapInt16(*pUdpFrame);
    pUdpFrame++;
  }

  for (uint32 uiSize = 0; uiSize < uiLength; uiSize += 2)
  {
    uiTmpChecksum += CcStatic::swapInt16(*puData);
    puData++;
  }

  if (uiLength & 1)
  {
    uiTmpChecksum += CCVOIDPTRCAST(uint8*, puData)[0];
  }

  while (uiTmpChecksum & 0xffff0000)
  {
    uiTmpChecksum = (uiTmpChecksum & 0xffff) + (uiTmpChecksum >> 16);
  }
  uiTmpChecksum = ~uiTmpChecksum;
  uiChecksum = uiTmpChecksum & 0xffff;
}

class CcTcpProtocol::CPrivate
{
public:
  CcList<CcNetworkSocketTcp*> oSockets;
};

CcTcpProtocol::CcTcpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcTcpProtocol::~CcTcpProtocol()
{
  CCDELETE(m_pPrivate);
}

bool CcTcpProtocol::initDefaults()
{
  bool bSuccess = true;
  return bSuccess;
}

uint16 CcTcpProtocol::getProtocolType() const
{
  return NCommonTypes::NNetwork::NEthernet::NIp::TCP;
}

void CcTcpProtocol::removeSocket(CcNetworkSocketTcp* pSocket)
{
  m_pPrivate->oSockets.removeItem(pSocket);
}

bool CcTcpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pTcpHeader = setupTcpHeader(pPacket);
  if(pTcpHeader != nullptr)
  {
    m_pParentProtocol->transmit(pPacket);
  }
  return bSuccess;
}

bool CcTcpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  // Socket will require the full TCP Packet with Header.
  //pPacket->addPosition(sizeof(CHeader));
  pPacket->uiSourcePort = pHeader->getSourcePort();
  pPacket->uiTargetPort = pHeader->getDestinationPort();
  pPacket->uiSize       = static_cast<uint16>(pPacket->getCurrentSize());
  for(CcNetworkSocketTcp* pSocket : m_pPrivate->oSockets)
  {
    if((pSocket->getConnectionInfo().getIp().isNullIp() ||
        pPacket->oTargetIp == pSocket->getConnectionInfo().getIp()) &&
       pPacket->uiTargetPort == pSocket->getConnectionInfo().getPort())
    {
      if(pSocket->insertPacket(pPacket))
      {
        bSuccess = true;
        break;
      }
    }
  }
  return bSuccess;
}

CcStatus CcTcpProtocol::registerSocket(CcNetworkSocketTcp* pSocket)
{
  CcStatus oStatus;
  for (CcNetworkSocketTcp* pExistingSocket : m_pPrivate->oSockets)
  {
    if( ( pExistingSocket->getConnectionInfo().getIp().isNullIp() ||
          pExistingSocket->getConnectionInfo().getIp() == pSocket->getConnectionInfo().getIp()) &&
          pExistingSocket->getConnectionInfo().getPort() == pSocket->getConnectionInfo().getPort()
      )
    {
      oStatus = EStatus::NetworkPortInUse;
    }
  }
  if (oStatus)
  {
    m_pPrivate->oSockets.append(pSocket);
  }
  return oStatus;
}

void CcTcpProtocol::sendSynAck(CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::ACK | CHeader::SYN, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendAck(CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::ACK, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendPshAck(CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::ACK | CHeader::PSH, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendFlags(uint16 uiFlags, CcNetworkPacket* pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  CHeader* pTcpHeader = setupTcpHeader(pPacket);
  if (pTcpHeader != nullptr)
  {
    pTcpHeader->uiHdrLenAndFlags = (uiFlags << 8) + ((sizeof(CHeader)>>2)<<4);
    pTcpHeader->uiAcknum = CcStatic::swapUint32(uiAcknoledge);
    pTcpHeader->uiSeqnum = CcStatic::swapUint32(uiSequence);
    if (pPacket->pInterface != nullptr &&
        IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::TCP))
    {
      pTcpHeader->generateChecksum(pPacket->oSourceIp, pPacket->oTargetIp, pPacket->size(), pPacket->getBuffer());
    }
    m_pParentProtocol->transmit(pPacket);
  }
}

CcTcpProtocol::CHeader* CcTcpProtocol::setupTcpHeader(CcNetworkPacket* pPacket)
{
  CHeader* pTcpHeader = nullptr;
  CcIpSettings* pIpSettings;
  if((pIpSettings = getNetworkStack()->getInterfaceForIp(pPacket->oTargetIp)) != nullptr )
  {
    const CcMacAddress* pMacAddress = getNetworkStack()->arpGetMacFromIp(pPacket->oTargetIp, true);
    if(pMacAddress != nullptr)
    {
      pPacket->oTargetMac = *pMacAddress;
      pPacket->pInterface = pIpSettings->pInterface;
      pPacket->oSourceIp = pIpSettings->oIpAddress;
      pPacket->oSourceMac = pIpSettings->pInterface->getMacAddress();
      pTcpHeader = new CHeader();
      CCMONITORNEW(pTcpHeader);
      pTcpHeader->setDestinationPort(pPacket->uiTargetPort);
      pTcpHeader->setSourcePort(pPacket->uiSourcePort);
      pTcpHeader->setHeaderLength(sizeof(CHeader));
      pTcpHeader->uiChecksum = 0;
      pPacket->transferBegin(pTcpHeader, sizeof(CHeader));
      pPacket->uiProtocolType = getProtocolType();
      pTcpHeader = static_cast<CHeader*>(pPacket->getLastBuffer());
    }
  }
  return pTcpHeader;
}
