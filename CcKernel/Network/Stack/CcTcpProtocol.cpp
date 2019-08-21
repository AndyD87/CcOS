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
#include "Network/Stack/CcTcpProtocol.h"
#include "Network/Stack/CcNetworkSocketTcp.h"
#include "Network/Stack/CcNetworkStack.h"
#include "Network/Stack/CcIpProtocol.h"
#include "Network/NCommonTypes.h"
#include "Network/CcIpSettings.h"
#include "Devices/INetwork.h"
#include "CcList.h"

void CcTcpProtocol::CHeader::generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp, uint16 uiLength, void* pData)
{
  uint32 uiTmpChecksum = 0;
  uiChecksum = 0;

  // Generate Pseudo Header
  uiTmpChecksum += (oDestIp.getIpV4_3() << 8) + oDestIp.getIpV4_2();
  uiTmpChecksum += (oDestIp.getIpV4_1() << 8) + oDestIp.getIpV4_0();
  uiTmpChecksum += (oSourceIp.getIpV4_3() << 8) + oSourceIp.getIpV4_2();
  uiTmpChecksum += (oSourceIp.getIpV4_1() << 8) + oSourceIp.getIpV4_0();
  uiTmpChecksum += static_cast<uint16>(NCommonTypes::NNetwork::NEthernet::NIp::TCP);
  uiTmpChecksum += getHeaderLength() + uiLength;

  // Store temporary checksum in checksum field

  // Calculate Header
  uiTmpChecksum += static_cast<uint16>(~CcIpProtocol::generateChecksumSwapped(&uiSrcPort, getHeaderLength()));
  uiTmpChecksum += static_cast<uint16>(~CcIpProtocol::generateChecksumSwapped(static_cast<uint16*>(pData), uiLength));

  while (uiTmpChecksum & 0xffff0000)
  {
    uiTmpChecksum = (uiTmpChecksum & 0xffff) + (uiTmpChecksum >> 16);
  }

  uiChecksum = CcStatic::swapUint16(static_cast<uint16>(~uiTmpChecksum));
}

class CcTcpProtocol::CPrivate
{
public:
  CcVector<CcNetworkSocketTcp*> oSockets;
};

CcTcpProtocol::CcTcpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
  CCNEW(m_pPrivate, CPrivate);
}

CcTcpProtocol::~CcTcpProtocol()
{
  CCDELETE(m_pPrivate);
}

bool CcTcpProtocol::init()
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

bool CcTcpProtocol::transmit(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CHeader* pTcpHeader = setupTcpHeader(pPacket);
  if(pTcpHeader != nullptr)
  {
    m_pParentProtocol->transmit(pPacket);
    CCDELETE(pPacket);
  }
  return bSuccess;
}

bool CcTcpProtocol::receive(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  if(pHeader != nullptr)
  {
    // Socket will require the full TCP Packet with Header.
    //pPacket->addPosition(sizeof(CHeader));
    pPacket->uiSourcePort = pHeader->getSourcePort();
    pPacket->uiTargetPort = pHeader->getDestinationPort();
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

void CcTcpProtocol::sendSynAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::ACK | CHeader::SYN, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::ACK, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendFin(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::FIN, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendFinAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::FIN | CHeader::ACK, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendPshAck(CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  sendFlags(CHeader::ACK | CHeader::PSH, pPacket, uiSequence, uiAcknoledge);
}

void CcTcpProtocol::sendFlags(uint16 uiFlags, CcNetworkPacketRef pPacket, uint32 uiSequence, uint32 uiAcknoledge)
{
  void* pData = nullptr;
  CHeader* pTcpHeader = setupTcpHeader(pPacket);
  if (pTcpHeader != nullptr)
  {
    pTcpHeader->uiHdrLenAndFlags = (uiFlags << 8) + ((sizeof(CHeader)>>2)<<4);
    if(IS_FLAG_SET(uiFlags, CHeader::ACK))
      pTcpHeader->uiAcknum = CcStatic::swapUint32(uiAcknoledge);
    else
      pTcpHeader->uiAcknum = 0;
    pTcpHeader->uiSeqnum = CcStatic::swapUint32(uiSequence);
    if (pPacket->pInterface != nullptr &&
        IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::TCP))
    {
      uint16 uiDataSize = static_cast<uint16>(pPacket->size());
      if(uiDataSize > 0)
      {
        pData = pPacket->getBuffer();
      }
      pTcpHeader->generateChecksum(pPacket->oSourceIp, pPacket->oTargetIp, uiDataSize, pData);
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
      CCNEW(pTcpHeader, CHeader);
      pTcpHeader->setDestinationPort(pPacket->uiTargetPort);
      pTcpHeader->setSourcePort(pPacket->uiSourcePort);
      pTcpHeader->setHeaderLength(sizeof(CHeader));
      pTcpHeader->setWindow(1500);
      pTcpHeader->uiChecksum = 0;
      pPacket->transferBegin(pTcpHeader, sizeof(CHeader));
      pPacket->uiProtocolType = getProtocolType();
      pTcpHeader = static_cast<CHeader*>(pPacket->getFirstBuffer());
    }
  }
  return pTcpHeader;
}
