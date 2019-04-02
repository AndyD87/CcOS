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
 * @brief     Implementation of class CcIcmpProtocol
 */
#include <Devices/INetwork.h>
#include <Network/CcNetworkPacket.h>
#include <Network/CcIcmpProtocol.h>
#include <Network/CcTcpProtocol.h>
#include <Network/CcUdpProtocol.h>
#include <Network/CcIpProtocol.h>
#include "CcStringList.h"

CcIcmpProtocol::CcIcmpProtocol(INetworkProtocol* pParentProtocol) :
  INetworkProtocol(pParentProtocol)
{
}

CcIcmpProtocol::~CcIcmpProtocol()
{
}

uint16 CcIcmpProtocol::getProtocolType() const
{
  return 1;
}

bool CcIcmpProtocol::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  if(m_pParentProtocol != nullptr)
  {
    pPacket->uiProtocolType = getProtocolType();
    CHeader* pHeader = CCVOIDPTRCAST(CHeader*, pPacket->getBuffer());
    pHeader->uiChecksum = 0;
    if(IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::ICMP))
    {
      pHeader->uiChecksum = CcIpProtocol::generateChecksum(CCVOIDPTRCAST(uint16*, pHeader), pPacket->size());
    }
    m_pParentProtocol->transmit(pPacket);
  }
  return bSuccess;
}

bool CcIcmpProtocol::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  if(!pPacket->oTargetIp.isMulticastIp())
  {
    CHeader* pHeader = CCVOIDPTRCAST(CHeader*, pPacket->getCurrentBuffer());
    if(pHeader != nullptr)
    {
      CcNetworkPacket* pResponse = pPacket->createAndRetarget();
      switch(pHeader->getType())
      {
        case EType::EchoRequest:
        {
          CHeader* pIcmpHeader = CCVOIDPTRCAST(CHeader*, new char[pPacket->getCurrentSize()]);
          CcStatic::memcpy(pIcmpHeader, pPacket->getCurrentBuffer(), pPacket->getCurrentSize());
          pIcmpHeader->setType(EType::Echo);
          pResponse->transfer(pIcmpHeader, pPacket->getCurrentSize());
          break;
        }
        default:
          pResponse->clear();
      }
      if(pResponse->size() > 0)
      {
        transmit(pResponse);
      }
      else
      {
        delete pResponse;
      }
      bSuccess = true;
    }
  }
  return bSuccess;
}
