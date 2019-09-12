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
 * @brief     Implementation of Class ILwipSocket
 */
#include "ILwipSocket.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"
#include "lwip/api.h"

ILwipSocket::ILwipSocket(ESocketType type) :
  ISocket(type),
  m_oConnectionInfo(type)
{
  switch(type)
  {
    case ESocketType::TCP:
      m_pNetconn = netconn_new(NETCONN_TCP);
      break;
    case ESocketType::UDP:
      m_pNetconn = netconn_new(NETCONN_UDP);
      break;
    default:
      m_pNetconn = nullptr;
  }
}

ILwipSocket::ILwipSocket(netconn* pNetconn) :
  m_pNetconn(pNetconn)
{
}

ILwipSocket::~ILwipSocket()
{
}

CcSocketAddressInfo ILwipSocket::getHostByName(const CcString& hostname)
{
  CcSocketAddressInfo oRetConnectionInfo;
  struct addrinfo *result = NULL;
  struct addrinfo hints;
  CcStatic::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  int iRet = getaddrinfo(hostname.getCharString(), NULL, &hints, &result);
  if (iRet == 0 && result != nullptr)
  {
    sockaddr_in* pSock = (sockaddr_in*)result->ai_addr;
    oRetConnectionInfo.setAddressData((CcTypes_sockaddr_in*)pSock, sizeof(sockaddr));
  }
  return oRetConnectionInfo;
}

CcStatus ILwipSocket::setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode)
{
  CcStatus oSuccess;
  if(eMode == ERwMode::Read || eMode == ERwMode::ReadWrite)
  {
    netconn_set_recvtimeout(m_pNetconn, uiTimeValue.getMSecond());
  }
  if(eMode == ERwMode::Write || eMode == ERwMode::ReadWrite)
  {
    netconn_set_sendtimeout(m_pNetconn, uiTimeValue.getMSecond());
  }
  return oSuccess;
}

CcSocketAddressInfo ILwipSocket::getPeerInfo()
{
  return m_oPeerInfo;
}

void ILwipSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oPeerInfo = oPeerInfo;
}


CcStatus ILwipSocket::setOption(ESocketOption eOption, void* pData, size_t uiDataLen)
{
  CcStatus oStatus;
  switch (eOption)
  {
    case ESocketOption::Broadcast:
    {
      int iEnable = 1;
      if (pData != nullptr && uiDataLen >= sizeof(int32))
      {
        iEnable = *static_cast<int32*>(pData);
      }
      oStatus = setOptionRaw(SOL_SOCKET, SO_BROADCAST, &iEnable, sizeof(iEnable));
      break;
    }
    case ESocketOption::Reuse:
      CCFALLTHROUGH;
    case ESocketOption::ReuseAddress:
      CCFALLTHROUGH;
    default:
      oStatus = EStatus::NotSupported;
      break;
  }
  return oStatus;
}

CcStatus ILwipSocket::setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen)
{
  CcStatus oStatus(EStatus::NotSupported);
  CCUNUSED(iLevel);
  CCUNUSED(iOptName);
  CCUNUSED(pData);
  CCUNUSED(uiDataLen);
  return oStatus;
}