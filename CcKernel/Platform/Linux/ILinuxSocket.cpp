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
 * @brief     Implementation of Class ILinuxSocket
 */
#include "ILinuxSocket.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"


ILinuxSocket::ILinuxSocket(ESocketType type) :
  ISocket(type),
  m_ClientSocket(-1),
  m_oConnectionInfo(type)
{
}

ILinuxSocket::ILinuxSocket(int socket, sockaddr sockAddr, int sockAddrlen) :
  m_ClientSocket(socket)
{
  CCUNUSED(sockAddrlen);
  m_oConnectionInfo.setAddressData( (CcTypes_sockaddr_in*)&sockAddr, sizeof(sockAddr));
  socklen_t iLen = static_cast<socklen_t>(m_oPeerInfo.ai_addrlen) ;
  getpeername(m_ClientSocket, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), &iLen);
}

ILinuxSocket::~ILinuxSocket()
{
}

CcSocketAddressInfo ILinuxSocket::getHostByName(const CcString& hostname)
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

void ILinuxSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  timeval tv;
  tv.tv_usec = uiTimeValue.getMSecond();  /* 30 Secs Timeout */
  tv.tv_sec  = uiTimeValue.getSecond();
  setsockopt(m_ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
  setsockopt(m_ClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

CcSocketAddressInfo ILinuxSocket::getPeerInfo()
{
  return m_oPeerInfo;
}

void ILinuxSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oPeerInfo = oPeerInfo;
}


CcStatus ILinuxSocket::setOption(ESocketOption eOption, void* pData, size_t uiDataLen)
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
    {
      int32 iEnable = 1;
      if (pData != nullptr && uiDataLen >= sizeof(int32))
      {
        iEnable = *static_cast<int32*>(pData);
      }
      oStatus = setOptionRaw(SOL_SOCKET, SO_REUSEADDR, &iEnable, sizeof(iEnable));
      if(oStatus)
      {
        oStatus = setOptionRaw(SOL_SOCKET, SO_REUSEPORT, &iEnable, sizeof(iEnable));
      }
      break;
    }
    case ESocketOption::ReuseAddress:
    {
      int32 iEnable = 1;
      if (pData != nullptr && uiDataLen >= sizeof(int32))
      {
        iEnable = *static_cast<int32*>(pData);
      }
      oStatus = setOptionRaw(SOL_SOCKET, SO_REUSEADDR, &iEnable, sizeof(iEnable));
      break;
    }
    case ESocketOption::ReusePort:
    {
      int32 iEnable = 1;
      if (pData != nullptr && uiDataLen >= sizeof(int32))
      {
        iEnable = *static_cast<int32*>(pData);
      }
      oStatus = setOptionRaw(SOL_SOCKET, SO_REUSEPORT, &iEnable, sizeof(iEnable));
      break;
    }
    default:
      oStatus = false;
      break;
  }
  return oStatus;
}

CcStatus ILinuxSocket::setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen)
{
  CcStatus oStatus;
  int iResult = setsockopt(m_ClientSocket, iLevel, iOptName, static_cast<char*>(pData), static_cast<int>(uiDataLen));
  if (iResult != 0)
  {
    oStatus.setSystemError(errno);
  }
  return oStatus;
}
