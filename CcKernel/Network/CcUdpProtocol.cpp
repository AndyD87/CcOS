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
#include <Network/CcUdpProtocol.h>
#include "NCommonTypes.h"
#include "CcNetworkSocketUdp.h"
#include "CcList.h"

uint16 CcUdpProtocol::CHeader::generateChecksum(const CcIp& oDestIp, const CcIp& oSourceIp)
{
  // uint16 checksum calculation by addition of each step.
  // - Fake ip header with 32bit source ip + 32bit dest ip + 16bit protocol (0x11) + length 0x0a
  // - UdpHeader
  // - UdpData
  // - add carry to current checksum, if carry again, do it again.
  // - create complement -> do checksum
  // Each step must swap
  // Here the example:
  uint32 uiChecksum = 0;
  uiChecksum += (oDestIp.getIpV4_3() << 8) + oDestIp.getIpV4_2();
  uiChecksum += (oDestIp.getIpV4_1() << 8) + oDestIp.getIpV4_0();
  uiChecksum += (oSourceIp.getIpV4_3() << 8) + oSourceIp.getIpV4_2();
  uiChecksum += (oSourceIp.getIpV4_1() << 8) + oSourceIp.getIpV4_0();
  uiChecksum += NCommonTypes::NEthernet::NIp::UDP;
  uiChecksum += uiLength;
  uint32 uiUdpSize = getLength();
  uint16* pUdpFrame = CCVOIDPTRCAST(uint16*, this);
  for (uint32 uiSize = 0; uiSize < uiUdpSize; uiSize += 2)
  {
    uiChecksum += CcStatic::swapInt16(*pUdpFrame);
    pUdpFrame++;
  }
  while (uiChecksum & 0xffff0000)
  {
    uiChecksum = (uiChecksum & 0xffff) + (uiChecksum >> 16);
  }
  uiChecksum = ~uiChecksum;
  return (uiChecksum & 0xffff);
}

class CcUdpProtocol::CcUdpProtocolPrivate
{
public:
  CcList<CcNetworkSocketUdp*> oSockets;
};

CcUdpProtocol::CcUdpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcUdpProtocol::~CcUdpProtocol()
{
}

uint16 CcUdpProtocol::getProtocolType() const
{
  return NCommonTypes::NEthernet::NIp::UDP;
}

bool CcUdpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CCUNUSED_TODO(pPacket);
  return bSuccess;
}

bool CcUdpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  for(CcNetworkSocketUdp* pSocket : m_pPrivate->oSockets)
  {
    if(pPacket->oTargetIp == pSocket->getConnectionInfo().getIp() &&
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

void CcUdpProtocol::registerSocket(CcNetworkSocketUdp* pSocket)
{
  m_pPrivate->oSockets.append(pSocket);
}

void CcUdpProtocol::removeSocket(CcNetworkSocketUdp* pSocket)
{
  m_pPrivate->oSockets.removeItem(pSocket);
}

bool CcUdpProtocol::initDefaults()
{
  bool bSuccess = true;
  return bSuccess;
}
