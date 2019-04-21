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
#include "Network/CcIpSettings.h"
#include "Network/CcNetworkStack.h"
#include "CcStringList.h"
#include "CcDeviceList.h"
#include "CcKernel.h"
#include "Devices/INetwork.h"
#include "CcList.h"
#include "CcVector.h"
#include "CcMutex.h"
#include "CcNetworkSocketUdp.h"
#include "CcNetworkSocketTcp.h"
#include "Network/CcArpProtocol.h"
#include "Network/CcIpProtocol.h"
#include "Network/NCommonTypes.h"
#include "Hash/CcCrc32.h"

class CcNetworkStack::CPrivate : public IThread
{
private:
  virtual void run() override
  {
    while(getThreadState() == EThreadState::Running)
    {
      while(oReceiveQueue.size() > 0)
      {
        oReceiveQueueLock.lock();
        CcNetworkPacket* pBufferList = oReceiveQueue[0];
        oReceiveQueue.remove(0);
        oReceiveQueueLock.unlock();
        pBufferList->setPosition(0);
        pParent->receive(pBufferList);
        if(pBufferList->bInUse == false)
          CCDELETE(pBufferList);
      }
      arpCleanup();
      CcKernel::delayMs(0);
    }
    arpCleanup();
  }

  void arpCleanup()
  {
    CcDateTime oCurrentTime = CcKernel::getDateTime();
    if (oArpListLock.isLocked() == false)
    {
      oArpListLock.lock();
      while (oArpList.size() > 0 &&
        oArpList[0].oLease < oCurrentTime)
      {
        oArpList.remove(0);
      }
      oArpListLock.unlock();
    }
    while (oArpPendingRequests.size() > 0 &&
           oArpPendingRequests[0]->oData.oLease < oCurrentTime)
    {
      CcStatic_memsetZeroPointer(oArpPendingRequests[0]);
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
    SArpEntry   oData;
    SArpEntry*  pEntry = nullptr;
  } SArpRequest;

  typedef struct
  {
    INetwork*              pInterface = nullptr;
    CcVector<CcIpSettings> oIpSettings;
  } SInterface;
public:
  CcNetworkStack *pParent = nullptr;
  CcList<CcNetworkPacket*> oReceiveQueue;
  CcMutex                  oReceiveQueueLock;
  CcList<SArpEntry>     oArpList;
  CcMutex               oArpListLock;
  CcList<SArpRequest*>  oArpPendingRequests;
  CcList<SInterface>    oInterfaceList;
  CcArpProtocol*  pArpProtocol = nullptr;
  CcIpProtocol*   pIpProtocol = nullptr;
};

CcNetworkStack::CcNetworkStack() :
  INetworkProtocol(nullptr)
{
  m_pPrivate = new CcNetworkStack::CPrivate(this);
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->start();
}

CcNetworkStack::~CcNetworkStack()
{
  m_pPrivate->stop();
  m_pPrivate->waitForState(EThreadState::Stopped);
  while (m_pPrivate->oInterfaceList.size() > 0)
    removeNetworkDevice(m_pPrivate->oInterfaceList[0].pInterface);
  CCDELETE(m_pPrivate->pArpProtocol);
  CCDELETE(m_pPrivate->pIpProtocol);
  CCDELETE(m_pPrivate);
}

uint16 CcNetworkStack::getProtocolType() const
{
  return NCommonTypes::NNetwork::Ethernet;
}

bool CcNetworkStack::transmit(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  if(pPacket->pInterface != nullptr)
  {
    CEthernetHeader* pHeader = new CEthernetHeader();
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
    pPacket->transferBegin(pHeader, sizeof(CEthernetHeader));
    if (IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::ETH))
    {
      if (pPacket->size() > 1000)
        return false;
      uint32* pFcsBuffer = new uint32;
      CCMONITORNEW(pFcsBuffer);
      *pFcsBuffer = CcStatic::swapUint32( pPacket->getCrc32());
      pPacket->transfer(pFcsBuffer, 4);

    }
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
  CEthernetHeader* pHeader = static_cast<CEthernetHeader*>(pPacket->getCurrentBuffer());
  pPacket->oTargetMac.setMac(pHeader->puiEthernetPacketDest, true);
  if (pPacket->oTargetMac == pPacket->pInterface->getMacAddress() ||
    pPacket->oTargetMac.isBroadcast())
  {
    pPacket->oSourceMac.setMac(pHeader->puiEthernetPacketSrc, true);
    pPacket->setPosition(sizeof(CEthernetHeader));
    for (INetworkProtocol* pProtocol : *this)
    {
      uint16 uiType = CcStatic::swapUint16(pProtocol->getProtocolType());
      if (uiType == pHeader->uiProtocolType)
      {
        if (pProtocol->receive(pPacket))
        {
          bSuccess = true;
          break;
        }
      }
    }
  }
  return bSuccess;
}

void CcNetworkStack::onReceive(CcNetworkPacket* pBuffer)
{
  m_pPrivate->oReceiveQueueLock.lock();
  if(m_pPrivate->oReceiveQueue.size() < 10)
  {
    m_pPrivate->oReceiveQueue.append(pBuffer);
  }
  else
  {
    // Avoid overflow 
    delete pBuffer;
  }
  m_pPrivate->oReceiveQueueLock.unlock();
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

ISocket* CcNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch(eType)
  {
    case ESocketType::UDP:
      pSocket = new CcNetworkSocketUdp(this);
      break;
    case ESocketType::TCP:
      pSocket = new CcNetworkSocketTcp(this);
      break;
    default:
      pSocket = nullptr;
  }
  return pSocket;
}

void CcNetworkStack::addNetworkDevice(INetwork* pNetworkDevice)
{
  CcNetworkStack::CPrivate::SInterface oInterface;
  oInterface.pInterface = pNetworkDevice;
  CcIpSettings oIpSettings;
  oIpSettings.pInterface = pNetworkDevice;
  oIpSettings.oIpAddress.setIpV4(192, 168, 0, 2);
  oInterface.oIpSettings.append(oIpSettings);
  pNetworkDevice->registerOnReceive(NewCcEvent(CcNetworkStack,CcNetworkPacket,CcNetworkStack::onReceive,this));
  m_pPrivate->oInterfaceList.append(oInterface);
}

void CcNetworkStack::removeNetworkDevice(INetwork* pNetworkDevice)
{
  size_t uiIndex = 0;
  for (CcNetworkStack::CPrivate::SInterface& rIpSettings : m_pPrivate->oInterfaceList)
  {
    if (rIpSettings.pInterface == pNetworkDevice)
    {
      rIpSettings.pInterface->removeOnReceive();
      break;
    }
    uiIndex++;
  }
  if (uiIndex < m_pPrivate->oInterfaceList.size())
  {
    m_pPrivate->oInterfaceList.remove(uiIndex);
  }
}

size_t CcNetworkStack::getAdapterCount()
{
  return m_pPrivate->oInterfaceList.size();
}

void CcNetworkStack::arpInsert(const CcIp& oIp, const CcMacAddress& oMac, bool bWasReply)
{
  m_pPrivate->oArpListLock.lock();
  m_pPrivate->oArpList.append({oIp, oMac, CcKernel::getDateTime().addSeconds(300)});
  if (bWasReply)
  {
    CPrivate::SArpEntry* pEntry = &m_pPrivate->oArpList.last();
    for (CPrivate::SArpRequest* pRequest : m_pPrivate->oArpPendingRequests)
    {
      if (pRequest->oData.oIp.isNullIp() && pRequest->oData.oMac == oMac)
      {
        pRequest->pEntry = pEntry;
      }
      else if (pRequest->oData.oMac.isNull() && pRequest->oData.oIp == oIp)
      {
        pRequest->pEntry = pEntry;
      }
    }
  }
  m_pPrivate->oArpListLock.unlock();
}

const CcMacAddress* CcNetworkStack::arpGetMacFromIp(const CcIp& oIp, bool bDoRequest) const
{
  for(const CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    for(const CcIpSettings& oIpSettings : oInterface.oIpSettings)
    {
      if(oIpSettings.oIpAddress == oIp)
      {
        return &oInterface.pInterface->getMacAddress();
      }
    }
  }
  m_pPrivate->oArpListLock.lock();
  for(const CcNetworkStack::CPrivate::SArpEntry& oEntry : m_pPrivate->oArpList)
  {
    if(oEntry.oIp == oIp)
    {
      m_pPrivate->oArpListLock.unlock();
      return &oEntry.oMac;
    }
  }
  m_pPrivate->oArpListLock.unlock();
  // do arp Request
  if (m_pPrivate->pArpProtocol != nullptr &&
      bDoRequest)
  {
    CPrivate::SArpRequest* pArpRequest = new CPrivate::SArpRequest();
    pArpRequest->oData.oIp = oIp;
    pArpRequest->oData.oLease = CcKernel::getDateTime();
    pArpRequest->oData.oLease.addMSeconds(100);
    m_pPrivate->oArpPendingRequests.append(pArpRequest);
    for (CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
    {
      for (CcIpSettings& oIpSetting : oInterface.oIpSettings)
      {
        m_pPrivate->pArpProtocol->queryMac(oIp, oIpSetting);
      }
    }
    while (pArpRequest->oData.oIp.isNullIp() == false)
    {
      if (pArpRequest->pEntry != nullptr)
      {
        return &pArpRequest->pEntry->oMac;
      }
    }
  }
  return nullptr;
}

CcIpSettings* CcNetworkStack::getInterfaceForIp(const CcIp& oIp)
{
  CcIpSettings* pIpSettings = nullptr;
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    for(CcIpSettings& oIpSetting : oInterface.oIpSettings)
    {
      if(oIpSetting.isInSubnet(oIp))
      {
        pIpSettings = &oIpSetting;
      }
    }
  }
  return pIpSettings;
}

const CcIp* CcNetworkStack::arpGetIpFromMac(const CcMacAddress& oMac, bool bDoRequest) const
{
  for(const CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
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
  m_pPrivate->oArpListLock.lock();
  for(CcNetworkStack::CPrivate::SArpEntry& oEntry : m_pPrivate->oArpList)
  {
    if(oEntry.oMac == oMac)
    {
      m_pPrivate->oArpListLock.unlock();
      return &oEntry.oIp;
    }
  }
  m_pPrivate->oArpListLock.unlock();
  // do arp Request
  if (m_pPrivate->pArpProtocol != nullptr &&
      bDoRequest)
  {
    CPrivate::SArpRequest* pArpRequest = new CPrivate::SArpRequest();
    pArpRequest->oData.oMac = oMac;
    pArpRequest->oData.oLease = CcKernel::getDateTime();
    pArpRequest->oData.oLease.addMSeconds(100);
    m_pPrivate->oArpPendingRequests.append(pArpRequest);
    while (pArpRequest->oData.oMac.isNull() == false)
    {
      if (pArpRequest->pEntry != nullptr)
      {
        return &pArpRequest->pEntry->oIp;
      }
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
  m_pPrivate->pIpProtocol = new CcIpProtocol(this);
  bSuccess &= m_pPrivate->pIpProtocol->initDefaults();
  INetworkProtocol::append(m_pPrivate->pIpProtocol);
  m_pPrivate->pArpProtocol = new CcArpProtocol(this);
  bSuccess &= m_pPrivate->pArpProtocol->initDefaults();
  INetworkProtocol::append(m_pPrivate->pArpProtocol);
  return bSuccess;
}
