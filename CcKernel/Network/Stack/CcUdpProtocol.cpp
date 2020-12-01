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
 * @brief     Implementation of class CcUdpProtocol
 */
#include "Network/Stack/CcUdpProtocol.h"
#include "Network/Stack/CcNetworkSocketUdp.h"
#include "Network/Stack/CcNetworkStack.h"
#include "Network/Stack/CcIpProtocol.h"
#include "Network/NCommonTypes.h"
#include "Network/CcIpInterface.h"
#include "Devices/INetwork.h"
#include "CcList.h"

void CcUdpProtocol::CHeader::generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp)
{
  // uint16 checksum calculation by addition of each step.
  // - Fake ip header with 32bit source ip + 32bit dest ip + 16bit protocol (0x11) + length 0x0a
  // - UdpHeader
  // - UdpData
  // - add carry to current checksum, if carry again, do it again.
  // - create complement -> do checksum
  // Each step must swap
  // Here the example:
  uint32 uiTmpChecksum = 0;
  uiTmpChecksum += (oDestIp.getIpV4_3() << 8) + oDestIp.getIpV4_2();
  uiTmpChecksum += (oDestIp.getIpV4_1() << 8) + oDestIp.getIpV4_0();
  uiTmpChecksum += (oSourceIp.getIpV4_3() << 8) + oSourceIp.getIpV4_2();
  uiTmpChecksum += (oSourceIp.getIpV4_1() << 8) + oSourceIp.getIpV4_0();
  uiTmpChecksum += NCommonTypes::NNetwork::NEthernet::NIp::UDP;
  uint32 uiUdpSize = getLength();
  uiTmpChecksum += uiUdpSize;
  uint16* pUdpFrame = CCVOIDPTRCAST(uint16*, this);

  uiTmpChecksum += static_cast<uint16>(~CcIpProtocol::generateChecksumSwapped(pUdpFrame, uiUdpSize));

  while (uiTmpChecksum & 0xffff0000)
  {
    uiTmpChecksum = (uiTmpChecksum & 0xffff) + (uiTmpChecksum >> 16);
  }
  uiTmpChecksum = ~uiTmpChecksum;
  uiChecksum = uiTmpChecksum & 0xffff;
}

class CcUdpProtocol::CPrivate
{
public:
  CcVector<CcNetworkSocketUdp*> oSockets;
};

CcUdpProtocol::CcUdpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
  CCNEW(m_pPrivate, CPrivate);
}

CcUdpProtocol::~CcUdpProtocol()
{
  CCDELETE(m_pPrivate);
}

uint16 CcUdpProtocol::getProtocolType() const
{
  return NCommonTypes::NNetwork::NEthernet::NIp::UDP;
}

bool CcUdpProtocol::transmit(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CcIpInterface* pIpSettings;
  if((pIpSettings = getNetworkStack()->getInterfaceForIp(pPacket->oTargetIp)) != nullptr )
  {
    const CcMacAddress* pMacAddress = getNetworkStack()->arpGetMacFromIp(pPacket->oTargetIp, true);
    if(pMacAddress != nullptr)
    {
      pPacket->oTargetMac = *pMacAddress;
      pPacket->pInterface = pIpSettings->pInterface;
      pPacket->oSourceIp = pIpSettings->oIpAddress;
      pPacket->oSourceMac = pIpSettings->pInterface->getMacAddress();
      CCNEWTYPE(pUdpHeader, CHeader);
      pUdpHeader->setDestinationPort(pPacket->uiTargetPort);
      pUdpHeader->setSourcePort(pPacket->uiSourcePort);
      pUdpHeader->setLength(static_cast<uint16>(pPacket->size() + sizeof(CHeader)));
      if( pPacket->pInterface == nullptr &&
          IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::uTCP))
      {
        pUdpHeader->generateChecksum(pPacket->oSourceIp, pPacket->oTargetIp);
      }
      pPacket->transferBegin(pUdpHeader, sizeof(CHeader));
      pPacket->uiProtocolType = getProtocolType();
      m_pParentProtocol->transmit(pPacket);
    }
  }
  return bSuccess;
}

bool CcUdpProtocol::receive(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  if(pHeader != nullptr)
  {
    pPacket->addPosition(sizeof(CHeader));
    pPacket->uiSourcePort = pHeader->getSourcePort();
    pPacket->uiTargetPort = pHeader->getDestinationPort();
    pPacket->uiSize       = pHeader->getLength() - sizeof(CHeader);
    for(CcNetworkSocketUdp* pSocket : m_pPrivate->oSockets)
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

CcStatus CcUdpProtocol::registerSocket(CcNetworkSocketUdp* pSocket)
{
  CcStatus oStatus;
  for (CcNetworkSocketUdp* pExistingSocket : m_pPrivate->oSockets)
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

bool CcUdpProtocol::removeSocket(CcNetworkSocketUdp* pSocket)
{
  return m_pPrivate->oSockets.removeItem(pSocket);
}

bool CcUdpProtocol::init()
{
  bool bSuccess = true;
  return bSuccess;
}
