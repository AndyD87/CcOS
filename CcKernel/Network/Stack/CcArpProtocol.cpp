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
 * @brief     Implementation of class CcArpProtocol
 */
#include "Network/Stack/CcArpProtocol.h"
#include "Network/CcMacAddress.h"
#include "Network/Stack/CcNetworkStack.h"
#include "Network/CcIpInterface.h"
#include "Devices/INetwork.h"
#include "Network/NCommonTypes.h"
#include "CcIp.h"

CcArpProtocol::CcArpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcArpProtocol::~CcArpProtocol()
{
}

uint16 CcArpProtocol::getProtocolType() const
{
  return NCommonTypes::NNetwork::NEthernet::ARP;
}

bool CcArpProtocol::transmit(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  m_pParentProtocol->transmit(pPacket);
  return bSuccess;
}

bool CcArpProtocol::receive(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  // Check for supported Headerinfo
  if( pHeader != nullptr &&
      pHeader->uiProtocolType == 0x8 &&   // IPV4 network order
      pHeader->uiMacType == 0x100 &&      // Ethernet network order
      pHeader->uiProtocolSize == 4 &&     // IP with 4 bytes
      pHeader->uiMacSize == 6 &&          // MAC with 6 bytes
      getNetworkStack() != nullptr
      )
  {
    bSuccess = true;
    CcMacAddress oMacAddress(pHeader->puiSourceMac, true);
    const CcIp* pIp = getNetworkStack()->arpGetIpFromMac(oMacAddress, false);
    CcIp oIpAddress(pHeader->puiSourceIp, true);
    if(pIp == nullptr)
    {
      getNetworkStack()->arpInsert(oIpAddress, oMacAddress, false);
    }
    else if(*pIp != oIpAddress)
    {
      getNetworkStack()->arpInsert(oIpAddress, oMacAddress, false);
    }
    if(pHeader->uiOperation == 0x100) // request in networkorder
    {
      oMacAddress.setMac(pHeader->puiDestinationMac, true);
      oIpAddress.setIpV4(pHeader->puiDestinationIp, true);
      CHeader* pResponse = nullptr;
      if(!oIpAddress.isNullIp())
      {
        const CcMacAddress* pFoundMac = getNetworkStack()->arpGetMacFromIp(oIpAddress, false);
        if(pFoundMac != nullptr)
        {
          CCNEW(pResponse, CHeader);
          CcStatic::memcpy(pResponse->puiDestinationMac, pHeader->puiSourceMac, sizeof(pHeader->puiSourceMac));
          CcStatic::memcpy(pResponse->puiDestinationIp, pHeader->puiSourceIp, sizeof(pHeader->puiSourceIp));
          CcStatic::memcpy(pResponse->puiSourceIp, pHeader->puiDestinationIp, sizeof(pHeader->puiDestinationIp));
          pResponse->puiSourceMac[5]  = pFoundMac->getMac()[0];
          pResponse->puiSourceMac[4]  = pFoundMac->getMac()[1];
          pResponse->puiSourceMac[3]  = pFoundMac->getMac()[2];
          pResponse->puiSourceMac[2]  = pFoundMac->getMac()[3];
          pResponse->puiSourceMac[1]  = pFoundMac->getMac()[4];
          pResponse->puiSourceMac[0]  = pFoundMac->getMac()[5];
        }
      }
      else if(!oMacAddress.isNull())
      {
        const CcIp* pFoundIp = getNetworkStack()->arpGetIpFromMac(oMacAddress, false);
        if(pFoundIp != nullptr)
        {
          CCNEW(pResponse, CHeader);
          CcStatic::memcpy(pResponse->puiDestinationMac, pHeader->puiSourceMac, sizeof(pHeader->puiSourceMac));
          CcStatic::memcpy(pResponse->puiDestinationIp, pHeader->puiSourceIp, sizeof(pHeader->puiSourceIp));
          CcStatic::memcpy(pResponse->puiSourceMac, pHeader->puiDestinationMac, sizeof(pHeader->puiDestinationMac));
          pResponse->puiSourceIp[3]  = pFoundIp->getIpV4()[0];
          pResponse->puiSourceIp[2]  = pFoundIp->getIpV4()[1];
          pResponse->puiSourceIp[1]  = pFoundIp->getIpV4()[2];
          pResponse->puiSourceIp[0]  = pFoundIp->getIpV4()[3];
        }
      }
      if(pResponse != nullptr)
      {
        CCNEWTYPE(pResponsePacket, CcNetworkPacket);
        pResponsePacket->transfer(pResponse, sizeof(*pResponse));
        pResponse = nullptr;
        pResponsePacket->pInterface = pPacket->pInterface;
        pResponsePacket->oTargetMac = pPacket->oSourceMac;
        if(pResponsePacket->pInterface != nullptr)
        {
          pResponsePacket->oSourceMac = pResponsePacket->pInterface->getMacAddress();
        }
        pResponsePacket->uiProtocolType = 0x0806;
        transmit(pResponsePacket);
        CCDELETE(pResponsePacket);
      }
      CCDELETE(pResponse);
    }
    else if (pHeader->uiOperation == 0x200)
    {
      CcMacAddress oMac;
      oMacAddress.setMac(pHeader->puiDestinationMac, true);
      oIpAddress.setIpV4(pHeader->puiDestinationIp, true);
      getNetworkStack()->arpInsert(oIpAddress, oMac, true);
    }
  }
  return bSuccess;
}

void CcArpProtocol::queryMac(const CcIp& oQueryIp, const CcIpInterface& oInterface)
{
  CCNEWTYPE(pRequest,CHeader);
  CcStatic::memcpySwapped(pRequest->puiDestinationIp, oQueryIp.getIpV4(), sizeof(pRequest->puiDestinationIp));
  CcStatic::memcpySwapped(pRequest->puiSourceIp, oInterface.oIpAddress.getIpV4(), sizeof(pRequest->puiSourceIp));
  CcStatic::memcpySwapped(pRequest->puiSourceMac, oInterface.pInterface->getMacAddress().getMac(), sizeof(pRequest->puiSourceMac));
  CcStatic::memset(pRequest->puiDestinationMac, 0, sizeof(pRequest->puiDestinationMac));
  pRequest->uiOperation = 0x100; // Request in network byte order

  CCNEWTYPE(pPacket,CcNetworkPacket);
  pPacket->oTargetMac.setMac(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
  pPacket->oSourceMac = oInterface.pInterface->getMacAddress();
  pPacket->transferBegin(pRequest, sizeof(CHeader));
  transmit(pPacket);
  CCDELETE(pPacket);
}

bool CcArpProtocol::init()
{
  bool bSuccess = true;
  return bSuccess;
}
