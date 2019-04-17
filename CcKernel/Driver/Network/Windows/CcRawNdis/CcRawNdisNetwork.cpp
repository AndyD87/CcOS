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
 * @brief     Implementation of class CcRawNdisNetwork
 **/

#include "CcRawNdisNetwork.h"
#include "CcRawNdisAccess.h"

class CcRawNdisNetwork::CPrivate
{
public:
  CPrivate()
    {}
  ~CPrivate()
    {}
  CcRawNdisAccess oNdisAccess;
};

CcRawNdisNetwork::CcRawNdisNetwork()
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);

}

CcRawNdisNetwork::~CcRawNdisNetwork()
{
  CCDELETE(m_pPrivate);
}

const CcMacAddress& CcRawNdisNetwork::getMacAddress()
{
  return CcMacAddress();
}

bool CcRawNdisNetwork::isConnected()
{
  bool bRet = false;
  return bRet;
}

uint32 CcRawNdisNetwork::getChecksumCapabilities()
{
  return 0;
}

void CcRawNdisNetwork::readFrame()
{
}

bool CcRawNdisNetwork::writeFrame(const CcNetworkPacket& oFrame)
{
  CCUNUSED(oFrame);
  return false;
}

bool CcRawNdisNetwork::isNdisAvailable()
{
  return m_pPrivate->oNdisAccess.open();
}