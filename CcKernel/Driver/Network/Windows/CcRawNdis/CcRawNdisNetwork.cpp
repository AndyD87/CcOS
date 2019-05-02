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
#include "CcWString.h"

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
  CcMacAddress    oMacAddress = CcMacAddress(0x00, 0x00, 0x12, 0x34, 0x56, 0x78);
};

CcRawNdisNetwork::CcRawNdisNetwork(unsigned long uiIndex)
{
  m_pPrivate = new CPrivate(this);
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->start();
  if (m_pPrivate->oNdisAccess.open(uiIndex))
  {
    m_pPrivate->oNdisAccess.setMacAddress(m_pPrivate->oMacAddress.getMac());
  }
}

CcRawNdisNetwork::CcRawNdisNetwork(const CcString& sName)
{
  m_pPrivate = new CPrivate(this);
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->start();
  CcWString wsName(sName);
  if (m_pPrivate->oNdisAccess.open(std::wstring(wsName.getWcharString())))
  {
    m_pPrivate->oNdisAccess.setMacAddress(m_pPrivate->oMacAddress.getMac());
  }
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
  size_t uiSize = m_pPrivate->oNdisAccess.read(oBuffer.getArray(), static_cast<uint16>(oBuffer.size()));
  if (uiSize <= oBuffer.size())
  {
    CcNetworkPacket* pPacket = new CcNetworkPacket();
    pPacket->pInterface = this;
    pPacket->write(oBuffer.getArray(), uiSize);
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
  return m_pPrivate->oNdisAccess.write(const_cast<CcNetworkPacket&>(oFrame).getBuffer(), static_cast<uint16>(oFrame.size())) <= oFrame.size();
}

unsigned long CcRawNdisNetwork::getAdapterCount()
{
  if (m_pPrivate->oNdisAccess.open())
  {
    return m_pPrivate->oNdisAccess.getAdapterCount();
  }
  return 0;
}

bool CcRawNdisNetwork::isNdisAvailable()
{
  return m_pPrivate->oNdisAccess.open();
}