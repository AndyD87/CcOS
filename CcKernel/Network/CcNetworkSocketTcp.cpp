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

class CcNetworkSocketTcp::CPrivate
{
public:
  CcTcpProtocol* pTcpProtocol = nullptr;
  CcList<CcNetworkPacket*> pPacketsQueue;
  bool bInProgress = false;
  uint32 uiSequence = 0;
  uint32 uiAcknowledgement = 0;
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
  CcStatus oRet(false);
  return oRet;
}

ISocket* CcNetworkSocketTcp::accept()
{
  return nullptr;
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
    CcNetworkPacket* pPacket = new CcNetworkPacket();
    pPacket->oSourceIp = getConnectionInfo().getIp();
    pPacket->uiSourcePort = getConnectionInfo().getPort();
    pPacket->oTargetIp = getPeerInfo().getIp();
    pPacket->uiTargetPort = getPeerInfo().getPort();
    pPacket->write(pBuffer, uiBufferSize);
    if(m_pPrivate->pTcpProtocol->transmit(pPacket))
    {
      uiRet = uiBufferSize;
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
      INetworkProtocol* pEthernetProtocol = m_pStack->findProtocol(NCommonTypes::NNetwork::Ethernet);
      if(pEthernetProtocol != nullptr)
      {
        INetworkProtocol* pIpProtocol = pEthernetProtocol->findProtocol(NCommonTypes::NNetwork::NEthernet::IP);
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
    if (m_pPrivate->pPacketsQueue.size() > 0)
    {
      bReadDone = true;
      CcNetworkPacket* pPacket = m_pPrivate->pPacketsQueue[0];
      m_oPeerInfo.setIp(pPacket->oSourceIp);
      m_oPeerInfo.setPort(pPacket->uiSourcePort);
      if (pPacket->uiSize <= uiDataLeft)
      {
        uiDataRead += pPacket->uiSize;
        pPacket->read(pBuffer, pPacket->uiSize);
        CCDELETE(pPacket);
        m_pPrivate->pPacketsQueue.remove(0);
      }
      else
      {
        uiDataRead += uiDataLeft;
        pPacket->write(pBuffer, uiDataLeft);
      }
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
  CcTcpProtocol::CHeader* pTcpHeader = static_cast<CcTcpProtocol::CHeader*>(pPacket->getCurrentBuffer());
  // Swap all data
  pTcpHeader->uiSrcPort = pTcpHeader->getSourcePort();
  pTcpHeader->uiDestPort = pTcpHeader->getDestinationPort();
  pTcpHeader->uiSeqnum = pTcpHeader->getSequence();
  pTcpHeader->uiAcknum = pTcpHeader->getAcknowledge();
  pTcpHeader->uiChecksum = pTcpHeader->getChecksum();   
  if (pTcpHeader->uiSeqnum > m_pPrivate->uiSequence)
  {
    size_t i = 0;
    for (CcNetworkPacket* pListPacket : m_pPrivate->pPacketsQueue)
    {
      CcTcpProtocol::CHeader* pListHeader = static_cast<CcTcpProtocol::CHeader*>(pListPacket->getCurrentBuffer());
      if (pListHeader->uiSeqnum > pTcpHeader->uiSeqnum)
      {
        m_pPrivate->pPacketsQueue.insert(i, pPacket);
        pPacket->bInUse = true;
        break;
      }
      else if (pListHeader->uiSeqnum == pTcpHeader->uiSeqnum)
      {
        // We have it, throw it away
        break;
      }
      i++;
    }
  }
  return true;
}
