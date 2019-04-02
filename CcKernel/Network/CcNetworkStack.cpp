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
#include <Network/CcEthernetProtocol.h>
#include <Network/CcIpSettings.h>
#include <Network/CcNetworkStack.h>
#include "CcStringList.h"
#include "CcDeviceList.h"
#include "CcKernel.h"
#include "Devices/INetwork.h"
#include "CcList.h"
#include "CcVector.h"

class CcNetworkStack::CPrivate : public IThread
{
private:
  virtual void run() override
  {
    while(getThreadState() == EThreadState::Running)
    {
      while(oReceiveQueue.size() > 0)
      {
        CcNetworkPacket* pBufferList = oReceiveQueue[0];
        oReceiveQueue.remove(0);
        pBufferList->setPosition(0);
        pParent->receive(pBufferList);
        CCDELETE(pBufferList);
      }
      arpCleanup();
      CcKernel::delayMs(0);
    }
  }

  void arpCleanup()
  {
    CcDateTime oCurrentTime = CcKernel::getDateTime();
    while(oArpList.size() > 0 &&
          oArpList[0].oLease < oCurrentTime)
    {
      oArpList.remove(0);
    }
  }

public:
  CPrivate(CcNetworkStack *pParent) : pParent(pParent)
  {}

  virtual ~CPrivate()
  { for(CcBufferList* pBuffer : oReceiveQueue) CCDELETE(pBuffer); }

public: // Types
  typedef struct
  {
    CcIp          oIp;
    CcMacAddress  oMac;
    CcDateTime    oLease;
  } SArpEntry;
  typedef struct
  {
    INetwork* pInterface;
    CcVector<CcIpSettings> oIpSettings;
  } SInterface;
public:
  CcNetworkStack *pParent;
  CcList<CcNetworkPacket*> oReceiveQueue;
  CcList<SArpEntry> oArpList;
  CcList<SInterface> oInterfaceList;
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

bool CcNetworkStack::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  if(pPacket->pInterface != nullptr)
  {
    if(pPacket->pInterface->writeFrame(*pPacket))
    {
      delete pPacket;
    }
  }
  return bSuccess;
}

bool CcNetworkStack::receive(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  for(INetworkProtocol* pProtocol : *this)
  {
    pProtocol->receive(pPacket);
  }
  return bSuccess;
}

void CcNetworkStack::onReceive(CcNetworkPacket* pBuffer)
{
  if(m_pPrivate->oReceiveQueue.size() < 10)
  {
    m_pPrivate->oReceiveQueue.append(pBuffer);
  }
}

bool CcNetworkStack::isInterfaceIpMatching(INetwork* pInterface, const CcIp& oIp)
{
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    if(oInterface.pInterface == pInterface)
    {
      for(CcIpSettings& oIpSetting : oInterface.oIpSettings)
      {
        if(oIpSetting.oIpAddress == oIp)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void CcNetworkStack::addNetworkDevice(INetwork* pNetworkDevice)
{
  CcNetworkStack::CPrivate::SInterface oInterface;
  oInterface.pInterface = pNetworkDevice;
  CcIpSettings oIpSettings;
  oIpSettings.oIpAddress.setIpV4(192, 168, 1, 93);
  oInterface.oIpSettings.append(oIpSettings);
  pNetworkDevice->registerOnReceive(NewCcEvent(CcNetworkStack,CcNetworkPacket,CcNetworkStack::onReceive,this));
  m_pPrivate->oInterfaceList.append(oInterface);
}

void CcNetworkStack::arpInsert(const CcIp& oIp, const CcMacAddress& oMac)
{
  m_pPrivate->oArpList.append({oIp, oMac, CcKernel::getDateTime().addSeconds(300)});
}

const CcMacAddress* CcNetworkStack::arpGetMacFromIp(const CcIp& oIp)
{
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    for(CcIpSettings& oIpSettings : oInterface.oIpSettings)
    {
      if(oIpSettings.oIpAddress == oIp)
      {
        return &oInterface.pInterface->getMacAddress();
      }
      else
      {
        return nullptr;
      }
    }
  }
  for(CcNetworkStack::CPrivate::SArpEntry& oEntry : m_pPrivate->oArpList)
  {
    if(oEntry.oIp == oIp)
    {
      return &oEntry.oMac;
    }
  }
  return nullptr;
}

const CcIp* CcNetworkStack::arpGetIpFromMac(const CcMacAddress& oMac)
{
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    if(oInterface.pInterface->getMacAddress() == oMac)
    {
      if(oInterface.oIpSettings.size() > 0)
      {
        return &oInterface.oIpSettings[0].oIpAddress;
      }
      else
      {
        return nullptr;
      }
    }
  }
  for(CcNetworkStack::CPrivate::SArpEntry& oEntry : m_pPrivate->oArpList)
  {
    if(oEntry.oMac == oMac)
    {
      return &oEntry.oIp;
    }
  }
  return nullptr;
}

bool CcNetworkStack::initDefaults()
{
  bool bSuccess = true;
  CcDeviceList oNetwork = CcKernel::getDevices(EDeviceType::Network);
  for(CcDeviceHandle& rDevice : oNetwork)
  {
    addNetworkDevice(rDevice.cast<INetwork>().ptr());
  }
  CcEthernetProtocol* pProtocol = new CcEthernetProtocol(this);
  pProtocol->initDefaults();
  INetworkProtocol::append(pProtocol);
  return bSuccess;
}
