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
 * @brief     Implementation of class CcNetworkStack
 */
#include "Network/CcNetworkStack.h"
#include "Network/CcEthernetProtocol.h"
#include "CcStringList.h"
#include "CcDeviceList.h"
#include "CcKernel.h"
#include "Devices/INetwork.h"
#include "CcList.h"
#include "IThread.h"

class CcNetworkStack::CPrivate : public IThread
{
private:
  virtual void run() override
  {
    while(getThreadState() == EThreadState::Running)
    {
      if(oReceiveQueue.size() > 0)
      {
        CcBufferList* pBufferList = oReceiveQueue[0];
        oReceiveQueue.remove(0);
        pParent->receive(*pBufferList);
        CCDELETE(pBufferList);
      }
    }
  }
public:
  CPrivate(CcNetworkStack *pParent) : pParent(pParent)
  {}

  virtual ~CPrivate()
  { for(CcBufferList* pBuffer : oReceiveQueue) CCDELETE(pBuffer); }

public:
  CcNetworkStack *pParent;
  CcList<CcBufferList*> oReceiveQueue;
};



CcNetworkStack::CcNetworkStack() :
  INetworkProtocol(nullptr)
{
  m_pPrivate = new CcNetworkStack::CPrivate(this);
  m_pPrivate->start();
  CCMONITORNEW(m_pPrivate);
}

CcNetworkStack::~CcNetworkStack()
{
  CCDELETE(m_pPrivate);
}

uint16 CcNetworkStack::getProtocolType() const
{
  return UINT16_MAX;
}

bool CcNetworkStack::transmit(CcBufferList& oBuffer)
{
  bool bSuccess = false;
  CCUNUSED_TODO(oBuffer);
  return bSuccess;
}

bool CcNetworkStack::receive(CcBufferList& oBuffer)
{
  bool bSuccess = false;
  for(INetworkProtocol* pProtocol : *this)
  {
    pProtocol->receive(oBuffer);
  }
  return bSuccess;
}

void CcNetworkStack::onReceive(CcBufferList* pBuffer)
{
  m_pPrivate->oReceiveQueue.append(pBuffer);
}

bool CcNetworkStack::initDefaults()
{
  bool bSuccess = false;
  CcDeviceList oNetwork = CcKernel::getDevices(EDeviceType::Network);
  for(CcDeviceHandle& rDevice : oNetwork)
  {
    rDevice.cast<INetwork>()->registerOnReceive(NewCcEvent(CcNetworkStack,CcBufferList,CcNetworkStack::onReceive,this));
  }
  CcEthernetProtocol* pProtocol = new CcEthernetProtocol(this);
  pProtocol->initDefaults();
  INetworkProtocol::append(pProtocol);
  return bSuccess;
}
