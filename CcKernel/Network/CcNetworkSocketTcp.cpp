/**
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @brief     Implementation of Class CcNetworkSocketTcp
 */
#include "CcNetworkSocketTcp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcSocketAddressInfo.h"
#include "Network/CcNetworkPacket.h"
#include "Network/CcNetworkStack.h"
#include "Network/NCommonTypes.h"
#include "Network/CcTcpProtocol.h"
#include "CcMutex.h"

class CcNetworkSocketTcp::CPrivate
{
public:
  enum class EState
  {
    Syncing,
    SyncingWait,
    Receiving,
    ReceivingWait,
    Finishing,
    Stopped
  };
  CcTcpProtocol* pTcpProtocol = nullptr;
  CcList<CcNetworkPacket*> oReadQueue;
  CcMutex oReadQueueMutex;
  EState ePeerState    = EState::Syncing;
  EState eLocalState   = EState::Syncing;
  uint32 uiSequence    = 0;
  uint32 uiAcknowledge = 0;
  uint32 uiExpectedAcknowledge = 0;
  uint32 uiReadQueueMax = 0;
  CcNetworkSocketTcp* pParent = nullptr;
  CcList<CcNetworkSocketTcp*> oChildList;
  CcMutex oChildListMutex;
};

CcNetworkSocketTcp::CcNetworkSocketTcp(CcNetworkStack* pStack) :
  INetworkSocket(pStack, ESocketType::TCP)
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcNetworkSocketTcp::~CcNetworkSocketTcp()
{
  close();
  if(m_pPrivate->pParent != nullptr)
  {
    m_pPrivate->pParent->m_pPrivate->oChildListMutex.lock();
    m_pPrivate->pParent->m_pPrivate->oChildList.removeItem(this);
    m_pPrivate->pParent->m_pPrivate->oChildListMutex.unlock();
  }
  m_pPrivate->oReadQueueMutex.lock();
  while(m_pPrivate->oReadQueue.size() > 0)
  {
    CCDELETE(m_pPrivate->oReadQueue[0]);
    m_pPrivate->oReadQueue.remove(0);
  }
  m_pPrivate->oReadQueueMutex.unlock();
  CCDELETE(m_pPrivate);
}

CcStatus CcNetworkSocketTcp::bind()
{
  CcStatus oResult(false);
  if(open())
  {
    oResult = m_pPrivate->pTcpProtocol->registerSocket(this);
  }
  return oResult;
}

CcStatus CcNetworkSocketTcp::connect()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus CcNetworkSocketTcp::listen()
{
  CcStatus oRet(true);
  m_pPrivate->uiReadQueueMax = 5;
  return oRet;
}

ISocket* CcNetworkSocketTcp::accept()
{
  CcNetworkSocketTcp* pNewTcpConnection = nullptr;
  m_pPrivate->eLocalState = CPrivate::EState::Receiving;
  while(m_pPrivate->eLocalState == CPrivate::EState::Receiving)
  {
    while(m_pPrivate->oReadQueue.size() == 0) CcKernel::delayMs(0);
    if(m_pPrivate->oReadQueue.size() > 0)
    {
      m_pPrivate->oReadQueueMutex.lock();
      CcNetworkPacket* pPacket = m_pPrivate->oReadQueue[0];
      m_pPrivate->oReadQueue.remove(0);
      m_pPrivate->oReadQueueMutex.unlock();
      CcTcpProtocol::CHeader* pTcpHeader = static_cast<CcTcpProtocol::CHeader*>(pPacket->getCurrentBuffer());
      uint8 uiFlags = pTcpHeader->getFlags();
      if(IS_FLAG_SET(uiFlags, CcTcpProtocol::CHeader::SYN))
      {
        pNewTcpConnection = new CcNetworkSocketTcp(m_pStack, m_pPrivate->pTcpProtocol, this);
        pNewTcpConnection->m_oPeerInfo.setIp(pPacket->oSourceIp);
        pNewTcpConnection->m_oPeerInfo.setPort(pPacket->uiSourcePort);
        pNewTcpConnection->m_oConnectionInfo = m_oConnectionInfo;
        pNewTcpConnection->m_pPrivate->eLocalState            = CPrivate::EState::ReceivingWait;
        pNewTcpConnection->m_pPrivate->ePeerState             = CPrivate::EState::Receiving;
        pNewTcpConnection->m_pPrivate->uiAcknowledge          = pTcpHeader->getSequence() + 1;
        pNewTcpConnection->m_pPrivate->uiSequence             = 0;
        pNewTcpConnection->m_pPrivate->uiExpectedAcknowledge  = 1 + pNewTcpConnection->m_pPrivate->uiSequence;
        pNewTcpConnection->m_pPrivate->pTcpProtocol->sendSynAck(pNewTcpConnection->genNetworkPaket(), pNewTcpConnection->m_pPrivate->uiSequence, pNewTcpConnection->m_pPrivate->uiAcknowledge);
        m_pPrivate->oChildListMutex.lock();
        m_pPrivate->oChildList.append(pNewTcpConnection);
        m_pPrivate->oChildListMutex.unlock();
        m_pPrivate->eLocalState = CPrivate::EState::ReceivingWait;
      }
      CCDELETE(pPacket);
    }
  }
  return pNewTcpConnection;
}

size_t CcNetworkSocketTcp::read(void *pBuffer, size_t uiBufferSize)
{
  return readTimeout(pBuffer, uiBufferSize, 0);
}

size_t CcNetworkSocketTcp::write(const void* pBuffer, size_t uiBufferSize)
{
  size_t uiRet = SIZE_MAX;
  if(m_pPrivate->pParent != NULL)
  {
    uiRet = 0;
    while(uiBufferSize > 0 &&
          m_pPrivate->ePeerState == CPrivate::EState::Receiving)
    {
      uint32 uiTempSize;
      if(uiBufferSize > 1400)
      {
        uiTempSize = 1400;
      }
      else
      {
        uiTempSize = static_cast<uint32>(uiBufferSize);
      }
      uiBufferSize -= uiTempSize;
      CcNetworkPacket* pPacket = genNetworkPaket();
      pPacket->write(static_cast<const char*>(pBuffer) + uiRet, uiTempSize);
      m_pPrivate->uiExpectedAcknowledge = m_pPrivate->uiSequence + uiTempSize;
      m_pPrivate->eLocalState = CPrivate::EState::ReceivingWait;
      m_pPrivate->pTcpProtocol->sendPshAck(pPacket, m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
      m_pPrivate->uiSequence = m_pPrivate->uiExpectedAcknowledge;
      while(m_pPrivate->eLocalState == CPrivate::EState::ReceivingWait)
        CcKernel::delayMs(0);
      uiRet += uiTempSize;
    }
  }
  return uiRet;
}

CcStatus CcNetworkSocketTcp::open(EOpenFlags eFlags)
{
  bool bSuccess = false;
  CCUNUSED(eFlags);
  if(m_pPrivate->pTcpProtocol == nullptr)
  {
    if(m_pStack != nullptr)
    {
      INetworkProtocol* pIpProtocol = m_pStack->findProtocol(NCommonTypes::NNetwork::NEthernet::IP);
      if(pIpProtocol != nullptr)
      {
        m_pPrivate->pTcpProtocol = static_cast<CcTcpProtocol*>(pIpProtocol->findProtocol(NCommonTypes::NNetwork::NEthernet::NIp::TCP));
        if(m_pPrivate->pTcpProtocol != nullptr)
        {
          bSuccess = true;
        }
      }
    }
  }
  else
  {
    bSuccess = true;
  }
  return bSuccess;
}

CcStatus CcNetworkSocketTcp::close()
{
  CcStatus oRet=true;
  if(m_pPrivate->pTcpProtocol != nullptr)
  {
    m_pPrivate->eLocalState = CPrivate::EState::Finishing;
    m_pPrivate->pTcpProtocol->sendFinAck(genNetworkPaket(), m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
    while(m_pPrivate->eLocalState == CPrivate::EState::Finishing)
      CcKernel::delayMs(0);
    m_pPrivate->pTcpProtocol->removeSocket(this);
    m_pPrivate->pTcpProtocol = nullptr;
  }
  return oRet;
}

CcStatus CcNetworkSocketTcp::cancel()
{
  CcStatus oRet(false);
  m_pPrivate->eLocalState = CPrivate::EState::Stopped;
  m_pPrivate->ePeerState = CPrivate::EState::Stopped;
  return oRet;
}

size_t CcNetworkSocketTcp::readTimeout(void *pBuffer, size_t uiBufferSize, const CcDateTime& oTimeout)
{
  size_t uiRet = SIZE_MAX;
  size_t uiDataRead = 0;
  size_t uiDataLeft = uiBufferSize;
  CcDateTime oRunTime;
  bool bReadDone = false;
  while (uiDataRead < uiBufferSize &&
          bReadDone == false &&
          m_pPrivate->ePeerState == CPrivate::EState::Receiving)
  {
    if (m_pPrivate->oReadQueue.size() > 0)
    {
      m_pPrivate->oReadQueueMutex.lock();
      CcNetworkPacket* pPacket = m_pPrivate->oReadQueue[0];
      m_pPrivate->oReadQueue.remove(0);
      // Parse Packet and extract data if parsing returns true
      uint16 uiReadSize = pPacket->uiSize;
      if(uiReadSize > 0)
      {
        if (pPacket->uiSize <= uiDataLeft)
        {
          uiDataRead += pPacket->uiSize;
          pPacket->read(pBuffer, pPacket->uiSize);
          CCDELETE(pPacket);
          bReadDone = true;
        }
        else
        {
          uiDataRead += uiDataLeft;
          pPacket->write(pBuffer, uiDataLeft);
        }
      }
      else
      {
        CCDELETE(pPacket);
      }
      m_pPrivate->oReadQueueMutex.unlock();
    }
    else
    {
      if(oRunTime < oTimeout)
      {
        oRunTime.addMSeconds(1);
        CcKernel::delayMs(1);
      }
      else if(oTimeout.getTimestampMs() == 0)
      {
        CcKernel::delayMs(0);
      }
      else
      {
        // timed out
        break;
      }
    }
  }
  if (uiDataRead > 0)
  {
    uiRet = uiDataRead;
  }
  return uiRet;
}

bool CcNetworkSocketTcp::insertPacket(CcNetworkPacket* pPacket)
{
  bool bSuccess = false;
  CcTcpProtocol::CHeader* pTcpHeader = static_cast<CcTcpProtocol::CHeader*>(pPacket->getCurrentBuffer());
  // Swap all data
  if(m_pPrivate->pParent == nullptr)
  {
    m_pPrivate->oChildListMutex.lock();
    for (CcNetworkSocketTcp* pListPacket : m_pPrivate->oChildList)
    {
      if(pListPacket->m_oPeerInfo.getIp() == pPacket->oSourceIp &&
          pListPacket->m_oPeerInfo.getPort() ==  pTcpHeader->getSourcePort())
      {
        pListPacket->insertPacket(pPacket);
        bSuccess = true;
        break;
      }
    };
    m_pPrivate->oChildListMutex.unlock();
    if(bSuccess == false)
    {
      if(m_pPrivate->eLocalState <= CPrivate::EState::Finishing)
      {
        m_pPrivate->oReadQueueMutex.lock();
        m_pPrivate->oReadQueue.append(pPacket);
        m_pPrivate->oReadQueueMutex.unlock();
        pPacket->bInUse = true;
        bSuccess = true;
      }
    }
  }
  else
  {
    parseNetworkPacket(pPacket);
    bSuccess = true;
  }
  return bSuccess;
}

CcNetworkPacket* CcNetworkSocketTcp::genNetworkPaket()
{
  CcNetworkPacket* pPacket = new CcNetworkPacket();
  CCMONITORNEW(pPacket);
  pPacket->oSourceIp = getConnectionInfo().getIp();
  pPacket->uiSourcePort = getConnectionInfo().getPort();
  pPacket->oTargetIp = getPeerInfo().getIp();
  pPacket->uiTargetPort = getPeerInfo().getPort();
  return pPacket;
}

uint16 CcNetworkSocketTcp::parseNetworkPacket(CcNetworkPacket* pPacket)
{
  uint16 uiRead = 0;
  CcTcpProtocol::CHeader* pTcpHeader = static_cast<CcTcpProtocol::CHeader*>(pPacket->getCurrentBuffer());
  uint8 uiFlags = pTcpHeader->getFlags();
  switch(uiFlags)
  {
    case CcTcpProtocol::CHeader::ACK:
      if(m_pPrivate->eLocalState == CPrivate::EState::ReceivingWait)
      {
        if( m_pPrivate->uiExpectedAcknowledge == pTcpHeader->getAcknowledge() &&
            m_pPrivate->uiAcknowledge == pTcpHeader->getSequence())
        {
          m_pPrivate->uiSequence = m_pPrivate->uiExpectedAcknowledge;
          m_pPrivate->uiExpectedAcknowledge = 0;
          m_pPrivate->eLocalState = CPrivate::EState::Receiving;
        }
        else
        {
          // Send error? or fin?
        }
      }
      else if(m_pPrivate->eLocalState == CPrivate::EState::Finishing)
      {
        m_pPrivate->eLocalState = CPrivate::EState::Stopped;
      }
      else
      {
        // Send error? or fin?
      }
      break;
    case CcTcpProtocol::CHeader::ACK | CcTcpProtocol::CHeader::PSH:
    {
      if( m_pPrivate->eLocalState == CPrivate::EState::Receiving)
      {
        if(  m_pPrivate->uiSequence == pTcpHeader->getAcknowledge() &&
            m_pPrivate->uiAcknowledge == pTcpHeader->getSequence())
        {
          uiRead = pPacket->uiSize - pTcpHeader->getHeaderLength();
          CcNetworkPacket* pResponse = genNetworkPaket();
          m_pPrivate->uiAcknowledge += uiRead;
          m_pPrivate->pTcpProtocol->sendAck(pResponse, m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
          pPacket->addPosition(pTcpHeader->getHeaderLength());
          pPacket->uiSize = uiRead;
          pPacket->bInUse = true;
          m_pPrivate->oReadQueueMutex.lock();
          m_pPrivate->oReadQueue.append(pPacket);
          m_pPrivate->oReadQueueMutex.unlock();
        }
        else
        {
          // Send error? or fin?
        }
      }
      else
      {
        // Send error? or fin?
      }
      break;
    }
    case CcTcpProtocol::CHeader::ACK | CcTcpProtocol::CHeader::FIN:
    {
      m_pPrivate->ePeerState = CPrivate::EState::Stopped;
      m_pPrivate->pTcpProtocol->sendAck(genNetworkPaket(), m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
      if( m_pPrivate->eLocalState == CPrivate::EState::Finishing)
      {
        m_pPrivate->eLocalState = CPrivate::EState::Stopped;
      }
      else
      {
        m_pPrivate->eLocalState = CPrivate::EState::Finishing;
        m_pPrivate->pTcpProtocol->sendFinAck(genNetworkPaket(), m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
      }
      break;
    }
  }
  return uiRead;
}

CcNetworkSocketTcp::CcNetworkSocketTcp(CcNetworkStack* pStack, CcTcpProtocol* pProtocol, CcNetworkSocketTcp* pParent) :
  INetworkSocket(pStack, ESocketType::TCP)
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->pTcpProtocol = pProtocol;
  m_pPrivate->pParent = pParent;
}
