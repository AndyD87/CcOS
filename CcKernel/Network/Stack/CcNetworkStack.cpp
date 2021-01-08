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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcNetworkStack
 */
#include "Network/CcIpInterface.h"
#include "Network/Stack/CcNetworkStack.h"
#include "CcStringList.h"
#include "CcDeviceList.h"
#include "CcKernel.h"
#include "Devices/INetwork.h"
#include "CcList.h"
#include "CcVector.h"
#include "CcMutex.h"
#include "CcStatic.h"
#include "CcNetworkSocketUdp.h"
#include "CcNetworkSocketTcp.h"
#include "Network/Stack/CcArpProtocol.h"
#include "Network/Stack/CcIpProtocol.h"
#include "Network/NCommonTypes.h"
#include "Hash/CcCrc32.h"

CcNetworkStack* CcNetworkStack::s_pInstance = nullptr;

class CcNetworkStack::CPrivate : public IThread
{
private:
  virtual void run() override
  {
    while(isRunning())
    {
      // Wait for next onReceive
      oReceiveWait.lock();
      oReceiveQueueLock.lock();
      for(CcNetworkPacket* pBufferList : oReceiveQueue)
      {
        pBufferList->setPosition(0);
        pParent->receive(pBufferList);
        CCDELETE(pBufferList);
      }
      oReceiveQueue.clear();
      oReceiveQueueLock.unlock();

      oReceiveQueue2Lock.lock();
      for(CcNetworkPacket* pBufferList : oReceiveQueue2)
      {
        pBufferList->setPosition(0);
        pParent->receive(pBufferList);
        CCDELETE(pBufferList);
      }
      oReceiveQueue2.clear();
      oReceiveQueue2Lock.unlock();
      arpCleanup();
      CcKernel::delayMs(1);
    }
  }

  void arpCleanup()
  {
    CcDateTime oCurrentTime = CcKernel::getUpTime();
    if (oArpListLock.tryLock() == true &&
        oArpList.size() > 0)
    {
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
      oArpPendingRequests.remove(0);
    }
  }

  virtual size_t getStackSize() override
  { return 16192; }
public:
  CPrivate(CcNetworkStack *pParent)  :
    IThread("CcNetworkStack"),
    pParent(pParent)
  {}

  virtual void onStop() override
  {
    for (CcBufferList* pBuffer : oReceiveQueue)
    {
      CCDELETE(pBuffer);
    }
    for (CcBufferList* pBuffer : oReceiveQueue2)
    {
      CCDELETE(pBuffer);
    }
    oReceiveQueueLock.unlock();
    oReceiveQueue2Lock.unlock();
    oReceiveWait.unlock();
  }

  virtual ~CPrivate()
  {
    // Save release all locks
    if(oReceiveQueueLock.tryLock())
      oReceiveQueueLock.unlock();
    if(oReceiveQueue2Lock.tryLock())
      oReceiveQueue2Lock.unlock();
    if(oReceiveWait.tryLock())
      oReceiveWait.unlock();
    CCDELETE(pIpProtocol);
    CCDELETE(pArpProtocol);
  }

public: // Types
  typedef struct _SArpEntry
  {
    CcIp          oIp;
    CcMacAddress  oMac;
    CcDateTime    oLease;
  } SArpEntry;

  typedef struct _SArpRequest
  {
    SArpEntry   oData;
    SArpEntry*  pEntry = nullptr;
  } SArpRequest;

  typedef struct _SInterface
  {
    INetwork*              pInterface = nullptr;
    CcVector<CcIpInterface> oIpSettings;
  } SInterface;
public:
  CcNetworkStack *pParent = nullptr;
  CcVector<CcNetworkPacket*> oReceiveQueue;
  CcVector<CcNetworkPacket*> oReceiveQueue2;
  CcMutex                  oReceiveQueueLock;
  CcMutex                  oReceiveQueue2Lock;
  CcMutex                  oReceiveWait;
  CcList<SArpEntry>     oArpList;
  CcMutex               oArpListLock;
  CcVector<SArpRequest*>  oArpPendingRequests;
  CcList<SInterface>    oInterfaceList;
  CcArpProtocol*  pArpProtocol = nullptr;
  CcIpProtocol*   pIpProtocol = nullptr;
};

CcNetworkStack::CcNetworkStack() :
  INetworkProtocol(nullptr)
{
  CCNEW(m_pPrivate, CPrivate, this);
  s_pInstance = this;
}

CcNetworkStack::~CcNetworkStack()
{
  m_pPrivate->stop();
  m_pPrivate->waitForExit();
  while (m_pPrivate->oInterfaceList.size() > 0)
    removeNetworkDevice(m_pPrivate->oInterfaceList[0].pInterface);
  CCDELETE(m_pPrivate);
}

uint16 CcNetworkStack::getProtocolType() const
{
  return NCommonTypes::NNetwork::Ethernet;
}

bool CcNetworkStack::transmit(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  if(pPacket->pInterface != nullptr)
  {
    CCNEWTYPE(pHeader, CEthernetHeader);
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
    if (IS_FLAG_NOT_SET(pPacket->pInterface->getChecksumCapabilities(), INetwork::CChecksumCapabilities::uETH))
    {
      CCNEWTYPE(pFcsBuffer, uint32);
      *pFcsBuffer = CcStatic::swapUint32( pPacket->getCrc32());
      pPacket->transfer(pFcsBuffer, 4);
    }
    pPacket->pInterface->writeFrame(pPacket);
  }
  return bSuccess;
}

bool CcNetworkStack::receive(CcNetworkPacketRef pPacket)
{
  bool bSuccess = false;
  CEthernetHeader* pHeader = static_cast<CEthernetHeader*>(pPacket->getCurrentBuffer());
  if(pHeader != nullptr)
  {
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
  }
  return bSuccess;
}

bool CcNetworkStack::isInterfaceIpMatching(INetwork* pInterface, const CcIp& oIp)
{
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    if(oInterface.pInterface == pInterface)
    {
      for(CcIpInterface& oIpSetting : oInterface.oIpSettings)
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

CcIpInterface* CcNetworkStack::getInterfaceForIp(const CcIp& oIp)
{
  CcIpInterface* pIpSettings = nullptr;
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    for(CcIpInterface& oIpSetting : oInterface.oIpSettings)
    {
      if(oIpSetting.isInSubnet(oIp))
      {
        pIpSettings = &oIpSetting;
      }
    }
  }
  return pIpSettings;
}

CcVector<CcIpInterface> CcNetworkStack::getIpSettingsForInterface(const INetwork* pInterface)
{
  for(CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    if(oInterface.pInterface == pInterface)
    {
      return oInterface.oIpSettings;
    }
  }
  return CcStatic::getNullRef<CcVector<CcIpInterface>>();
}

void CcNetworkStack::onReceive(INetwork::CPacket* pBuffer)
{
  if(m_pPrivate->oReceiveQueueLock.isLocked() == false)
  {
    if(m_pPrivate->oReceiveQueue.size() < 10)
    {
      m_pPrivate->oReceiveQueue.append(pBuffer->pPacket);
      pBuffer->pPacket = nullptr;
    }
  }
  else
  {
    if(m_pPrivate->oReceiveQueue2.size() < 10)
    {
      m_pPrivate->oReceiveQueue2.append(pBuffer->pPacket);
      pBuffer->pPacket = nullptr;
    }
  }
  m_pPrivate->oReceiveWait.unlock();
}

void CcNetworkStack::onDeviceEvent(IDevice* pDevice)
{
  INetwork* pInterface = static_cast<INetwork*>(pDevice);
  if (pDevice->getState() == IDevice::EState::Running)
  {
    addNetworkDevice(pInterface);
  }
  else
  {
    removeNetworkDevice(pInterface);
  }
}

ISocket* CcNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch(eType)
  {
    case ESocketType::UDP:
      CCNEW(pSocket, CcNetworkSocketUdp, this);
      break;
    case ESocketType::TCP:
      CCNEW(pSocket, CcNetworkSocketTcp, this);
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
  CcIpInterface oIpSettings;
  oIpSettings.pInterface = pNetworkDevice;
  oIpSettings.oIpAddress.setIpV4(192, 168, 1, 92);
  oInterface.oIpSettings.append(oIpSettings);
  pNetworkDevice->registerOnReceive(NewCcEventType(CcNetworkStack,INetwork::CPacket,CcNetworkStack::onReceive,this));
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
  m_pPrivate->oArpList.append({oIp, oMac, CcKernel::getUpTime().addSeconds(300)});
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

void CcNetworkStack::arpUpdate(const CcIp& oIp, const CcMacAddress& oMac)
{
  m_pPrivate->oArpListLock.lock();
  for(CcNetworkStack::CPrivate::SArpEntry& oEntry : m_pPrivate->oArpList)
  {
    if(oEntry.oIp == oIp)
    {
      m_pPrivate->oArpListLock.unlock();
      oEntry.oMac = oMac;
    }
  }
  m_pPrivate->oArpListLock.unlock();
}

const CcMacAddress* CcNetworkStack::arpGetMacFromIp(const CcIp& oIp, bool bDoRequest) const
{
  for(const CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
  {
    for(const CcIpInterface& oIpSettings : oInterface.oIpSettings)
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
    CCNEWTYPE(pArpRequest, CPrivate::SArpRequest);
    pArpRequest->oData.oIp = oIp;
    pArpRequest->oData.oLease = CcKernel::getUpTime();
    pArpRequest->oData.oLease.addSeconds(10);
    m_pPrivate->oArpPendingRequests.append(pArpRequest);
    for (CcNetworkStack::CPrivate::SInterface& oInterface : m_pPrivate->oInterfaceList)
    {
      for (CcIpInterface& oIpSetting : oInterface.oIpSettings)
      {
        m_pPrivate->pArpProtocol->queryMac(oIp, oIpSetting);
      }
    }
    size_t uiMaxWaiting = 100;
    while (pArpRequest->oData.oIp.isNullIp() == false &&
           uiMaxWaiting > 0)
    {
      if (pArpRequest->pEntry != nullptr)
      {
        const CcMacAddress* pMac = &pArpRequest->pEntry->oMac;
        CCDELETE(pArpRequest);
        return pMac;
      }
      else
      {
        uiMaxWaiting--;
        CcKernel::delayMs(1);
      }
    }
    CCDELETE(pArpRequest);
  }
  return nullptr;
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
    CCNEWTYPE(pArpRequest, CPrivate::SArpRequest);
    pArpRequest->oData.oMac = oMac;
    pArpRequest->oData.oLease = CcKernel::getUpTime();
    pArpRequest->oData.oLease.addMSeconds(100);
    m_pPrivate->oArpPendingRequests.append(pArpRequest);
    while (pArpRequest->oData.oMac.isNull() == false)
    {
      if (pArpRequest->pEntry != nullptr)
      {
        const CcIp* pIp = &pArpRequest->pEntry->oIp;
        CCDELETE(pArpRequest);
        return pIp;
      }
    }
    CCDELETE(pArpRequest);
  }
  return nullptr;
}

bool CcNetworkStack::init()
{
  bool bSuccess = true;
  CcDeviceList oNetwork = CcKernel::getDevices(EDeviceType::Network);
  for(CcDevice& rDevice : oNetwork)
  {
    addNetworkDevice(rDevice.cast<INetwork>().ptr());
  }
  CcKernel::registerOnDevicePnpEvent(EDeviceType::Network, NewCcEventType(CcNetworkStack, IDevice, CcNetworkStack::onDeviceEvent, this));
  CCNEW(m_pPrivate->pIpProtocol, CcIpProtocol, this);
  bSuccess &= m_pPrivate->pIpProtocol->init();
  INetworkProtocol::append(m_pPrivate->pIpProtocol);
  CCNEW(m_pPrivate->pArpProtocol, CcArpProtocol, this);
  bSuccess &= m_pPrivate->pArpProtocol->init();
  INetworkProtocol::append(m_pPrivate->pArpProtocol);

  // Start stack thread
  m_pPrivate->start();
  return bSuccess;
}
