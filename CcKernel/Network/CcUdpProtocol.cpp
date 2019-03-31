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
#include "CcStringList.h"

const uint16 CcUdpProtocol::c_uiProtocolType = 17;

class CcUdpProtocol::CcUdpProtocolPrivate
{
public:
  class CCondition
  {
  public:
    CCondition() = default;
    CCondition(const CcIp& oIp, uint16 uiPort) : oIp(oIp), uiPort(uiPort)
    {}
    CcIp    oIp;
    uint16  uiPort = 0;

    CCondition(const CCondition& oToCopy)
    {
      oIp = oToCopy.oIp;
      uiPort = oToCopy.uiPort;
    }

    bool operator==(const CCondition& oToCompare)
    {
      return (uiPort == oToCompare.uiPort && oIp == oToCompare.oIp);
    }
  };
  CcEventHandleMap<CCondition> oReceiver;
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
  return c_uiProtocolType;
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
  for(INetworkProtocol* pProtocol : *this)
  {
    pProtocol->receive(pPacket);
  }
  return bSuccess;
}

bool CcUdpProtocol::registerOnReceive(const CcIp& oIp, uint16 uiPort, IEvent* pEvent)
{
  m_pPrivate->oReceiver.append(CcUdpProtocolPrivate::CCondition(oIp, uiPort), pEvent);
  return true;
}

uint16 CcUdpProtocol::generateChecksum(CHeader* pHeader, const CcIp& oDestIp, const CcIp& oSourceIp)
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
  uiChecksum += c_uiProtocolType;
  uiChecksum += CcStatic::swapInt16(pHeader->uiLength);;
  uint32 uiUdpSize = getLength(pHeader);
  uint16* pUdpFrame = CCVOIDPTRCAST(uint16*, pHeader);
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

bool CcUdpProtocol::initDefaults()
{
  bool bSuccess = false;
  return bSuccess;
}
