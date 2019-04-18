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
#include "IThread.h"
#include "CcKernel.h"

class CcRawNdisNetwork::CPrivate : public IThread
{
public:
  CPrivate(CcRawNdisNetwork* pParent) :
    pParent(pParent)
    {}
  ~CPrivate()
    {}

  virtual void run() override
  {
    while(getThreadState() == EThreadState::Running)
      pParent->readFrame();
  }

  CcRawNdisNetwork* pParent = nullptr;
  CcRawNdisAccess oNdisAccess;
  CcMacAddress    oMacAddress;
};

CcRawNdisNetwork::CcRawNdisNetwork()
{
  m_pPrivate = new CPrivate(this);
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->start();

}

CcRawNdisNetwork::~CcRawNdisNetwork()
{
  m_pPrivate->stop();
  CCDELETE(m_pPrivate);
}

const CcMacAddress& CcRawNdisNetwork::getMacAddress()
{
  return m_pPrivate->oMacAddress;
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
  CcByteArray oBuffer(2048);
  if (m_pPrivate->oNdisAccess.read(oBuffer.getArray(), static_cast<uint16>(oBuffer.size())))
  {
    CcNetworkPacket* pPacket = new CcNetworkPacket();
    pPacket->pInterface = this;
    pPacket->append(oBuffer);
    if (m_pReceiver != nullptr)
      m_pReceiver->call(pPacket);
  }
  else
  {
    CcKernel::delayMs(0);
  }
}

bool CcRawNdisNetwork::writeFrame(const CcNetworkPacket& oFrame)
{
  return m_pPrivate->oNdisAccess.write(const_cast<CcNetworkPacket&>(oFrame).getBuffer(), static_cast<uint16>(oFrame.size()));
}

bool CcRawNdisNetwork::isNdisAvailable()
{
  return m_pPrivate->oNdisAccess.open();
}