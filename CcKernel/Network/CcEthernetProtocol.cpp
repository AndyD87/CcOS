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
 * @brief     Implementation of class CcEthernetProtocol
 */
#include <Network/CcArpProtocol.h>
#include <Network/CcEthernetProtocol.h>
#include <Network/CcIpProtocol.h>
#include <Devices/INetwork.h>
#include <Network/INetworkProtocol.h>

CcEthernetProtocol::CcEthernetProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcEthernetProtocol::~CcEthernetProtocol()
{
}

bool CcEthernetProtocol::initDefaults()
{
  bool bSuccess = false;
  CcIpProtocol* pIpProtocol = new CcIpProtocol(this);
  pIpProtocol->initDefaults();
  INetworkProtocol::append(pIpProtocol);
  CcArpProtocol* pArpProtocol = new CcArpProtocol(this);
  pArpProtocol->initDefaults();
  INetworkProtocol::append(pArpProtocol);
  return bSuccess;
}

uint16 CcEthernetProtocol::getProtocolType() const
{
  return UINT16_MAX;
}

bool CcEthernetProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = new CHeader();
  pHeader->puiEthernetPacketDest[0] = pPacket->oTargetMac.getMac()[5];
  pHeader->puiEthernetPacketDest[1] = pPacket->oTargetMac.getMac()[4];
  pHeader->puiEthernetPacketDest[2] = pPacket->oTargetMac.getMac()[3];
  pHeader->puiEthernetPacketDest[3] = pPacket->oTargetMac.getMac()[2];
  pHeader->puiEthernetPacketDest[4] = pPacket->oTargetMac.getMac()[1];
  pHeader->puiEthernetPacketDest[5] = pPacket->oTargetMac.getMac()[0];
  pHeader->puiEthernetPacketSrc[0] = pPacket->oSourceMac.getMac()[5];
  pHeader->puiEthernetPacketSrc[1] = pPacket->oSourceMac.getMac()[4];
  pHeader->puiEthernetPacketSrc[2] = pPacket->oSourceMac.getMac()[3];
  pHeader->puiEthernetPacketSrc[3] = pPacket->oSourceMac.getMac()[2];
  pHeader->puiEthernetPacketSrc[4] = pPacket->oSourceMac.getMac()[1];
  pHeader->puiEthernetPacketSrc[5] = pPacket->oSourceMac.getMac()[0];
  pHeader->uiProtocolType = CcStatic::swapUint16(pPacket->uiProtocolType);
  pPacket->transferBegin(pHeader, sizeof(CHeader));
  m_pParentProtocol->transmit(pPacket);
  return bSuccess;
}

bool CcEthernetProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  pPacket->oSourceMac.setMac(pHeader->puiEthernetPacketSrc, true);
  if(pPacket->oSourceMac == pPacket->pInterface->getMacAddress() ||
     pPacket->oSourceMac.isBroadcast())
  {
    pPacket->oTargetMac.setMac(pHeader->puiEthernetPacketDest, true);
    pPacket->setPosition(sizeof(CHeader));
    for(INetworkProtocol* pProtocol : *this)
    {
      uint16 uiType = CcStatic::swapUint16(pProtocol->getProtocolType());
      if (uiType == pHeader->uiProtocolType)
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
