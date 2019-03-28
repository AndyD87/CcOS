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
#include "Network/CcEthernetProtocol.h"
#include "CcStringList.h"
#include "Network/CcIpProtocol.h"

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
  return bSuccess;
}

uint16 CcEthernetProtocol::getProtocolType() const
{
  return UINT16_MAX;
}

bool CcEthernetProtocol::transmit(CcBufferList& oBuffer)
{
  bool bSuccess = false;
  CCUNUSED_TODO(oBuffer);
  return bSuccess;
}

bool CcEthernetProtocol::receive(CcBufferList& oBuffer)
{
  bool bSuccess = false;
  SHeader* pHeader = static_cast<SHeader*>(oBuffer.getBuffer());
  oBuffer.setPosition(sizeof(SHeader));
  for(INetworkProtocol* pProtocol : *this)
  {
    if (pProtocol->getProtocolType() == pHeader->uiProtocolType)
    {
      pProtocol->receive(oBuffer);
    }
  }
  return bSuccess;
}
