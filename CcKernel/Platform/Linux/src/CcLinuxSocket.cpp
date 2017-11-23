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
 * @file      CcLinuxSocket
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcLinuxSocket
 */
#include "CcLinuxSocket.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include <cstring>


CcLinuxSocket::CcLinuxSocket(ESocketType type) :
  CcSocketAbstract(type),
  m_ClientSocket(0)
{
}

CcLinuxSocket::CcLinuxSocket(int socket, sockaddr sockAddr, int sockAddrlen) :
  m_ClientSocket(socket)
{
  CCUNUSED(sockAddrlen);
  m_oConnectionInfo.setAddressData( (CcTypes_sockaddr_in*)&sockAddr, sizeof(sockAddr));
  socklen_t iLen = static_cast<socklen_t>(m_oPeerInfo.ai_addrlen) ;
  getpeername(m_ClientSocket, reinterpret_cast<sockaddr*>(m_oPeerInfo.sockaddr()), &iLen);
}

CcLinuxSocket::~CcLinuxSocket( void )
{
  int iResult;
  if (m_ClientSocket >= 0)
  {
    iResult = shutdown(m_ClientSocket, SHUT_RDWR);
    m_ClientSocket = -1;
    if (iResult != 0) 
    {
      close();
    }
  }
}

CcStatus CcLinuxSocket::bind(const CcSocketAddressInfo &oAddrInfo)
{
  CcStatus oResult;
  m_oConnectionInfo = oAddrInfo;
  int iResult;
  // Create a SOCKET for connecting to server
  m_ClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  if (m_ClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "CcLinuxSocket::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    iResult = ::bind(m_ClientSocket, reinterpret_cast<sockaddr*>(m_oConnectionInfo.ai_addr), (int) m_oConnectionInfo.ai_addrlen);
    if (iResult != 0)
    {
      oResult.setSystemError(errno);
      CCDEBUG("CcLinuxSocket::bind failed with error: " + CcString::fromNumber(errno));
      close();
    }
  }
  return oResult;
}

CcStatus CcLinuxSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  CcStatus oStatus(true);
  struct addrinfo *result = nullptr;
  struct addrinfo *ptr = nullptr;
  struct addrinfo hints;
  int iResult;

  memset( &hints, 0, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(oAddressInfo.getIpString().getCharString(), oAddressInfo.getPortString().getCharString(), &hints, &result);
  if ( iResult != 0 )
  {
    oStatus.setError(iResult);
    CCERROR("getaddrinfo failed with error: " + CcString::fromNumber(oStatus.getErrorUnit()));
  }
  else
  {
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
      m_ClientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      if (m_ClientSocket < 0)
      {
        oStatus.setError(iResult);
        CCERROR("socket failed with error: " + CcString::fromNumber(oStatus.getErrorUnit()));
        break;
      }
      else
      {
        // Connect to server.
        iResult = ::connect(m_ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult < 0)
        {
          close();
        }
        else
        {
          break;
        }
      }
    }
    freeaddrinfo(result);
  }
  return oStatus;
}

CcStatus CcLinuxSocket::listen(void)
{
  CcStatus oRet(false);
  if(!::listen(m_ClientSocket, 0))
  {
    oRet = true;
  }
  return oRet;
}

CcSocketAbstract* CcLinuxSocket::accept(void)
{
  // Accept a client socket
  CcSocketAbstract *sRet = nullptr;
  int Temp;
  sockaddr sockAddr;
  socklen_t sockAddrlen=sizeof(sockAddr);
  Temp = ::accept(m_ClientSocket, &sockAddr, &sockAddrlen);
  if (Temp < 0) 
  {
    CCERROR("accept failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    sRet = new CcLinuxSocket(Temp, sockAddr, sockAddrlen);
  }
  return sRet;
}

size_t CcLinuxSocket::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  int iResult = 0;
  if (m_ClientSocket >= 0)
  {
    iResult = ::recv(m_ClientSocket, buf, bufSize, 0);
    if (iResult < 0)
    {
      CCERROR("read failed with error: " + CcString::fromNumber(errno) );
      close();
    }
    else
    {
      uiRet = static_cast<size_t>(iResult);
    }
  }
  else
  {
    uiRet = static_cast<size_t>(iResult);
  }
  return uiRet;
}

size_t CcLinuxSocket::write(const void *buf, size_t bufSize)
{
  size_t uiRet = 0;
  // Send an initial buffer
  int iResult;
  iResult = ::send(m_ClientSocket, buf, bufSize, 0);
  if (iResult == -1) 
  {
    CCERROR("write failed with error: " + CcString::fromNumber(errno));
    close();
    uiRet = SIZE_MAX;
  }
  else
  {
    uiRet = static_cast<size_t>(iResult);
  }
  return uiRet;
}

CcSocketAddressInfo CcLinuxSocket::getHostByName(const CcString& hostname)
{
  CcSocketAddressInfo oRetConnectionInfo;
  struct addrinfo *result = NULL;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  int iRet = getaddrinfo(hostname.getCharString(), NULL, &hints, &result);
    sockaddr_in* pSock = (sockaddr_in*)result->ai_addr;
  if (iRet == 0)
  {
    oRetConnectionInfo.setAddressData((CcTypes_sockaddr_in*)pSock, sizeof(sockaddr));
  }
  return oRetConnectionInfo;
}

void CcLinuxSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  timeval tv;
  tv.tv_usec = uiTimeValue.getMSecond();  /* 30 Secs Timeout */
  tv.tv_sec  = uiTimeValue.getSecond();
  setsockopt(m_ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
  setsockopt(m_ClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

CcSocketAddressInfo CcLinuxSocket::getPeerInfo(void)
{
  return m_oPeerInfo;
}

void CcLinuxSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oPeerInfo = oPeerInfo;
}


CcStatus CcLinuxSocket::close(void)
{
  CcStatus oRet=false;
  if(m_ClientSocket != 0){
    oRet = ::close(m_ClientSocket);
  }
  return oRet;
}

CcStatus CcLinuxSocket::cancel(void)
{
  CcStatus oRet(false);
  if (-1 != shutdown(m_ClientSocket, SHUT_RDWR)){
    oRet = true;
    m_ClientSocket = 0;
  }
  return oRet;
}

size_t CcLinuxSocket::readTimeout(char *buf, size_t bufSize, time_t timeout) 
{
  size_t iRet = 0;
  fd_set readfds;
  struct timeval tv;
  int rv;
  // clear the set ahead of time
  FD_ZERO(&readfds);

  // add our descriptors to the set
  FD_SET(m_ClientSocket, &readfds);

  // since we got s2 second, it's the "greater", so we use that for
  // the n param in select()

  // wait until either socket has data ready to be recv()d (timeout 10.5 secs)
  tv.tv_sec = 0;
  tv.tv_usec = timeout;
  rv = select(m_ClientSocket+1, &readfds, NULL, NULL, &tv);

  if (rv == -1) 
  {
    iRet = SIZE_MAX;
    CCERROR("error occurred in select()");
  }
  else if (rv == 0) 
  {
    CCVERBOSE("Timeout occurred!  No data after 10.5 seconds.");
  }
  else 
  {
    // one or both of the descriptors have data
    if (FD_ISSET(m_ClientSocket, &readfds)) 
    {
      iRet = recv(m_ClientSocket, buf, bufSize, 0);
    }
  }
  return iRet;
}
