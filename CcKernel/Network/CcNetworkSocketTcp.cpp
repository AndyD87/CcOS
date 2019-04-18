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
  CcTcpProtocol* pTcpProtocol = nullptr;
  CcList<CcNetworkPacket*> oPacketsQueue;
  CcMutex oPacketsQueueMutex;
  bool bInProgress = false;
  uint32 uiSequence    = 0;
  uint32 uiAcknowledge = 0;
  uint32 uiPacketsQueueMax = 0;
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
  m_pPrivate->uiPacketsQueueMax = 5;
  return oRet;
}

ISocket* CcNetworkSocketTcp::accept()
{
  CcNetworkSocketTcp* pNewTcpConnection = nullptr;
  m_pPrivate->bInProgress = true;
  while(m_pPrivate->bInProgress != false)
  {
    while(m_pPrivate->oPacketsQueue.size() == 0) CcKernel::delayMs(0);
    if(m_pPrivate->oPacketsQueue.size() > 0)
    {
      m_pPrivate->oPacketsQueueMutex.lock();
      CcNetworkPacket* pPacket = m_pPrivate->oPacketsQueue[0];

      m_pPrivate->oPacketsQueueMutex.unlock();
      CcTcpProtocol::CHeader* pTcpHeader = static_cast<CcTcpProtocol::CHeader*>(pPacket->getCurrentBuffer());
      uint8 uiFlags = pTcpHeader->getFlags();
      if(IS_FLAG_SET(uiFlags, CcTcpProtocol::CHeader::SYN))
      {
        pNewTcpConnection = new CcNetworkSocketTcp(m_pStack, m_pPrivate->pTcpProtocol, this);
        pNewTcpConnection->m_oPeerInfo.setIp(pPacket->oSourceIp);
        pNewTcpConnection->m_oPeerInfo.setPort(pPacket->uiSourcePort);
        pNewTcpConnection->m_oConnectionInfo = m_oConnectionInfo;
        pNewTcpConnection->m_pPrivate->uiAcknowledge   = pTcpHeader->uiAcknum+1;
        pNewTcpConnection->m_pPrivate->uiSequence      = pTcpHeader->uiSeqnum;
        pNewTcpConnection->m_pPrivate->uiSequence++;
        pNewTcpConnection->m_pPrivate->pTcpProtocol->sendSynAck(genNetworkPaket(), m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
        m_pPrivate->pParent->m_pPrivate->oChildListMutex.lock();
        m_pPrivate->oChildList.append(pNewTcpConnection);
        m_pPrivate->pParent->m_pPrivate->oChildListMutex.unlock();
        m_pPrivate->bInProgress = false;
      }
      delete pPacket;
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
  if(open())
  {
    uiRet = 0;
    while(uiBufferSize > 0)
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
      m_pPrivate->pTcpProtocol->sendPshAck(pPacket, m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
      m_pPrivate->uiSequence += uiTempSize;
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
    m_pPrivate->pTcpProtocol->removeSocket(this);
    m_pPrivate->pTcpProtocol = nullptr;
  }
  return oRet;
}

CcStatus CcNetworkSocketTcp::cancel()
{
  CcStatus oRet(false);
  m_pPrivate->bInProgress = false;
  return oRet;
}

size_t CcNetworkSocketTcp::readTimeout(void *pBuffer, size_t uiBufferSize, const CcDateTime& oTimeout)
{
  size_t uiRet = SIZE_MAX;
  size_t uiDataRead = 0;
  size_t uiDataLeft = uiBufferSize;
  CcDateTime oRunTime;
  bool bReadDone = false;
  m_pPrivate->bInProgress = true;
  while (uiDataRead < uiBufferSize &&
          bReadDone == false &&
          m_pPrivate->bInProgress == true)
  {
    if (m_pPrivate->oPacketsQueue.size() > 0)
    {
      bReadDone = true;
      m_pPrivate->oPacketsQueueMutex.lock();
      CcNetworkPacket* pPacket = m_pPrivate->oPacketsQueue[0];
      // Parse Packet and extract data if parsing returns true
      uint16 uiReadSize = parseNetworkPacket(pPacket);
      if(uiReadSize > 0)
      {
        if (pPacket->uiSize <= uiDataLeft)
        {
          uiDataRead += pPacket->uiSize;
          pPacket->read(pBuffer, pPacket->uiSize);
          CCDELETE(pPacket);
          m_pPrivate->oPacketsQueue.remove(0);
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
        m_pPrivate->oPacketsQueue.remove(0);
      }
      m_pPrivate->oPacketsQueueMutex.unlock();
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
  m_pPrivate->bInProgress = false;
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
  pTcpHeader->uiSrcPort = pTcpHeader->getSourcePort();
  pTcpHeader->uiDestPort = pTcpHeader->getDestinationPort();
  pTcpHeader->uiSeqnum = pTcpHeader->getSequence();
  pTcpHeader->uiAcknum = pTcpHeader->getAcknowledge();
  pTcpHeader->uiChecksum = pTcpHeader->getChecksum();   
  if(m_pPrivate->pParent == nullptr)
  {
    m_pPrivate->oChildListMutex.lock();
    for (CcNetworkSocketTcp* pListPacket : m_pPrivate->oChildList)
    {
      if(pListPacket->m_oPeerInfo.getIp() == pPacket->oSourceIp &&
          pListPacket->m_oPeerInfo.getPort() ==  pTcpHeader->getSourcePort())
      {
        bSuccess = pListPacket->insertPacket(pPacket);
        break;
      }
    };
    m_pPrivate->oChildListMutex.unlock();
    if(bSuccess == false)
    {
      m_pPrivate->oPacketsQueue.append(pPacket);
    }
    pPacket->bInUse = true;
  }
  else
  {
    if (pTcpHeader->uiSeqnum > m_pPrivate->uiSequence)
    {
      size_t i = 0;
      m_pPrivate->oPacketsQueueMutex.lock();
      for (CcNetworkPacket* pListPacket : m_pPrivate->oPacketsQueue)
      {
        CcTcpProtocol::CHeader* pListHeader = static_cast<CcTcpProtocol::CHeader*>(pListPacket->getCurrentBuffer());
        if (pListHeader->uiSeqnum > pTcpHeader->uiSeqnum)
        {
          if(m_pPrivate->oPacketsQueue.size() < m_pPrivate->uiPacketsQueueMax)
          {
            m_pPrivate->oPacketsQueue.insert(i, pPacket);
            pPacket->bInUse = true;
            bSuccess = true;
          }
          else
          {
            //! @todo send bufferoverflow
          }
          break;
        }
        else if (pListHeader->uiSeqnum == pTcpHeader->uiSeqnum)
        {
          // We have it, throw it away
          break;
        }
        i++;
      }
      m_pPrivate->oPacketsQueueMutex.unlock();
    }
  }
  return bSuccess;
}

CcNetworkPacket* CcNetworkSocketTcp::genNetworkPaket()
{
  CcNetworkPacket* pPacket = new CcNetworkPacket();
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
  uint8 uiFlags = (pTcpHeader->uiHdrLenAndFlags & 0x3f00) >> 8;
  switch(uiFlags)
  {
    case CcTcpProtocol::CHeader::ACK:
      if( m_pPrivate->uiSequence == pTcpHeader->uiSeqnum &&
          m_pPrivate->uiAcknowledge == pTcpHeader->uiAcknum)
      {
         // ACK succeeded
      }
      else
      {
        // Send error? or fin?
      }
      break;
    case CcTcpProtocol::CHeader::ACK | CcTcpProtocol::CHeader::PSH:
    {
      if( m_pPrivate->uiSequence == pTcpHeader->uiSeqnum &&
          m_pPrivate->uiAcknowledge == pTcpHeader->uiAcknum)
      {
        uiRead = pPacket->uiSize - pTcpHeader->getHeaderLength();
        CcNetworkPacket* pResponse = genNetworkPaket();
        m_pPrivate->uiAcknowledge += uiRead;
        m_pPrivate->pTcpProtocol->sendAck(pResponse, m_pPrivate->uiSequence, m_pPrivate->uiAcknowledge);
        pPacket->addPosition(pTcpHeader->getHeaderLength());
        pPacket->uiSize = uiRead;
      }
      else
      {
        // Send error? or fin?
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
