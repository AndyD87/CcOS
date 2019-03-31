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
#include <Network/CcArpProtocol.h>
#include <Network/CcMacAddress.h>
#include <Network/CcNetworkStack.h>
#include <Devices/INetwork.h>
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
  return 0x0806;
}

bool CcArpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  m_pParentProtocol->transmit(pPacket);
  return bSuccess;
}

bool CcArpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CHeader* pHeader = static_cast<CHeader*>(pPacket->getCurrentBuffer());
  // Check for supported Headerinfo
  if( pHeader->uiProtocolType == 0x8 &&   // IPV4 network order
      pHeader->uiMacType == 0x100 &&      // Ethernet network order
      pHeader->uiProtocolSize == 4 &&     // IP with 4 bytes
      pHeader->uiMacSize == 6 &&          // MAC with 6 bytes
      getNetworkStack() != nullptr
      )
  {
    CcMacAddress oMacAddress(pHeader->puiSourceMac, true);
    const CcIp* pIp = getNetworkStack()->arpGetIpFromMac(oMacAddress);
    CcIp oIpAddress(pHeader->puiSourceIp, true);
    if(pIp == nullptr)
    {
      getNetworkStack()->arpInsert(oIpAddress, oMacAddress);
    }
    else if(*pIp != oIpAddress)
    {
      // Do update? Trusted source?
    }
    if(pHeader->uiOperation == 0x100) // request in networkorder
    {
      oMacAddress.setMac(pHeader->puiDestinationMac, true);
      oIpAddress.setIpV4(pHeader->puiDestinationIp, true);
      CHeader* pResponse = nullptr;
      if(!oIpAddress.isNullIp())
      {
        const CcMacAddress* pFoundMac = getNetworkStack()->arpGetMacFromIp(oIpAddress);
        if(pFoundMac != nullptr)
        {
          pResponse = new CHeader();
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
        // @Todo Transmit
      }
      else if(!oMacAddress.isNull())
      {
        const CcIp* pFoundIp = getNetworkStack()->arpGetIpFromMac(oMacAddress);
        if(pFoundIp != nullptr)
        {
          pResponse = new CHeader();
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
        CcNetworkPacket* pResponsePacket = new CcNetworkPacket();
        pResponsePacket->transfer(pResponse, sizeof(*pResponse));
        pResponsePacket->pInterface = pPacket->pInterface;
        pResponsePacket->oTargetMac = pPacket->oSourceMac;
        if(pResponsePacket->pInterface != nullptr)
        {
          pResponsePacket->oSourceMac = pResponsePacket->pInterface->getMacAddress();
        }
        pResponsePacket->uiProtocolType = 0x0806;
        transmit(pResponsePacket);
      }
    }
  }
  return bSuccess;
}

bool CcArpProtocol::initDefaults()
{
  bool bSuccess = false;
  return bSuccess;
}
