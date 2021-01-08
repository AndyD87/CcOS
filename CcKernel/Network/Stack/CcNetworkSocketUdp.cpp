/*
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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcNetworkSocketUdp
 */
#include "CcNetworkSocketUdp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include "CcStatic.h"
#include "CcVector.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/Stack/CcNetworkPacket.h"
#include "Network/Stack/CcNetworkStack.h"
#include "Network/NCommonTypes.h"
#include "Network/Stack/CcUdpProtocol.h"

class CcNetworkSocketUdp::CPrivate
{
public:
  CcUdpProtocol* pUdpProtocol = nullptr;
  CcVector<CcNetworkPacket*> pPacketsQueue;
  bool bInProgress = false;
};

CcNetworkSocketUdp::CcNetworkSocketUdp(CcNetworkStack* pStack) :
  INetworkSocket(pStack, ESocketType::UDP)
{
  CCNEW(m_pPrivate, CPrivate);
}

CcNetworkSocketUdp::~CcNetworkSocketUdp()
{
  close();
  CCDELETE(m_pPrivate);
}

CcStatus CcNetworkSocketUdp::bind()
{
  CcStatus oResult(false);
  if(open())
  {
    oResult = m_pPrivate->pUdpProtocol->registerSocket(this);
  }
  return oResult;
}

CcStatus CcNetworkSocketUdp::connect()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus CcNetworkSocketUdp::listen()
{
  CcStatus oRet(false);
  return oRet;
}

ISocket* CcNetworkSocketUdp::accept()
{
  return nullptr;
}

size_t CcNetworkSocketUdp::read(void *pBuffer, size_t uiBufferSize)
{
  return readTimeout(pBuffer, uiBufferSize, m_oReadTimeout);
}

size_t CcNetworkSocketUdp::write(const void* pBuffer, size_t uiBufferSize)
{
  size_t uiRet = SIZE_MAX;
  if(open())
  {
    CCNEWTYPE(pPacket, CcNetworkPacket);
    pPacket->oSourceIp = getConnectionInfo().getIp();
    pPacket->uiSourcePort = getConnectionInfo().getPort();
    pPacket->oTargetIp = getPeerInfo().getIp();
    pPacket->uiTargetPort = getPeerInfo().getPort();
    pPacket->write(pBuffer, uiBufferSize);
    if(m_pPrivate->pUdpProtocol->transmit(pPacket))
    {
      uiRet = uiBufferSize;
    }
    CCDELETE(pPacket);
  }
  return uiRet;
}

CcStatus CcNetworkSocketUdp::open(EOpenFlags eFlags)
{
  bool bSuccess = false;
  CCUNUSED(eFlags);
  if(m_pPrivate->pUdpProtocol == nullptr)
  {
    if(m_pStack != nullptr)
    {
      INetworkProtocol* pIpProtocol = m_pStack->findProtocol(NCommonTypes::NNetwork::NEthernet::IP);
      if(pIpProtocol != nullptr)
      {
        m_pPrivate->pUdpProtocol = static_cast<CcUdpProtocol*>(pIpProtocol->findProtocol(NCommonTypes::NNetwork::NEthernet::NIp::UDP));
        if(m_pPrivate->pUdpProtocol != nullptr)
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

CcStatus CcNetworkSocketUdp::close()
{
  CcStatus oRet=true;
  if(m_pPrivate->pUdpProtocol != nullptr)
  {
    m_pPrivate->pUdpProtocol->removeSocket(this);
    m_pPrivate->pUdpProtocol = nullptr;
  }
  return oRet;
}

CcStatus CcNetworkSocketUdp::cancel()
{
  CcStatus oRet(false);
  m_pPrivate->bInProgress = false;
  return oRet;
}

size_t CcNetworkSocketUdp::readTimeout(void *pBuffer, size_t uiBufferSize, const CcDateTime& oTimeout)
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

bool CcNetworkSocketUdp::insertPacket(CcNetworkPacketRef pPacket)
{
  m_pPrivate->pPacketsQueue.append(pPacket);
  pPacket = nullptr;
  return true;
}
