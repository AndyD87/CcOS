/**
 * @copyright  Andreas Dirmeier (C) 2022
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
 * @brief     Implementation of Class CcLinuxSocketTcp
 */

#include "CcLinuxSocketTcp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"

CcLinuxSocketTcp::CcLinuxSocketTcp() :
  ILinuxSocket(ESocketType::TCP)
{
}

CcLinuxSocketTcp::CcLinuxSocketTcp(int socket, sockaddr sockAddr, uint32 sockAddrlen) :
  ILinuxSocket(socket, sockAddr, sockAddrlen)
{
}

CcLinuxSocketTcp::~CcLinuxSocketTcp()
{
  close();
}

CcStatus CcLinuxSocketTcp::setAddressInfo(const CcSocketAddressInfo &oAddrInfo)
{
  CcStatus oResult;
  m_oConnectionInfo = oAddrInfo;
  m_hClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  return oResult;
}

CcStatus CcLinuxSocketTcp::bind()
{
  CcStatus oResult;
  int iResult;
  // Create a SOCKET for connecting to server
  if (m_hClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "CcLinuxSocketTcp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    iResult = ::bind(m_hClientSocket, static_cast<sockaddr*>(m_oConnectionInfo.sockaddr()), static_cast<socklen_t>(m_oConnectionInfo.ai_addrlen));
    if (iResult != 0)
    {
      oResult.setSystemError(errno);
      CCDEBUG("CcLinuxSocketTcp::bind failed with error: " + CcString::fromNumber(errno));
    }
  }
  return oResult;
}

CcStatus CcLinuxSocketTcp::connect()
{
  CcStatus oStatus(true);
  struct addrinfo *result = nullptr;
  struct addrinfo *ptr = nullptr;
  struct addrinfo hints;
  int iResult;

  CcStatic::memset( &hints, 0, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(m_oPeerInfo.getIpString().getCharString(), m_oPeerInfo.getPortString().getCharString(), &hints, &result);
  if ( iResult != 0 )
  {
    oStatus.setSystemError(iResult);
    CCVERBOSE("getaddrinfo failed with error: " + CcString::fromNumber(oStatus.getErrorUint()));
  }
  else
  {
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != nullptr ;ptr=ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
      m_hClientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      if (m_hClientSocket < 0)
      {
        oStatus.setSystemError(iResult);
        CCVERBOSE("socket failed with error: " + CcString::fromNumber(oStatus.getErrorUint()));
        break;
      }
      else
      {
        // Connect to server.
        iResult = ::connect(m_hClientSocket, ptr->ai_addr, static_cast<socklen_t>(ptr->ai_addrlen));
        if (iResult < 0)
        {
          oStatus.setSystemError(errno);
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

CcStatus CcLinuxSocketTcp::listen()
{
  CcStatus oRet(false);
  if(::listen(m_hClientSocket, 0) == 0)
  {
    oRet = true;
  }
  return oRet;
}

ISocket* CcLinuxSocketTcp::accept()
{
  // Accept a client socket
  ISocket *sRet = nullptr;
  int Temp;
  sockaddr sockAddr;
  socklen_t sockAddrlen=sizeof(sockAddr);
  Temp = ::accept(m_hClientSocket, &sockAddr, &sockAddrlen);
  if (Temp < 0)
  {
    CCVERBOSE("accept failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    CCNEW(sRet, CcLinuxSocketTcp, Temp, sockAddr, sockAddrlen);
  }
  return sRet;
}

size_t CcLinuxSocketTcp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  if (m_hClientSocket >= 0)
  {
    ssize_t iResult = ::recv(m_hClientSocket, buf, bufSize, 0);
    if (iResult < 0)
    {
      CCVERBOSE("read failed with error: " + CcString::fromNumber(errno) );
    }
    else
    {
      uiRet = static_cast<size_t>(iResult);
    }
  }
  return uiRet;
}

CcStatus CcLinuxSocketTcp::dataAvailable()
{
  CcStatus oStatus = false;
  /* Note: timeout must be (re)set every time before call to select() */
  timeval   tv;
  fd_set    fdread;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  FD_ZERO(&fdread);
  FD_SET( m_hClientSocket, &fdread );

  int selectStatus = select(m_hClientSocket+1, &fdread, NULL, NULL, &tv);

  switch( selectStatus )
  {
      case -1:
          oStatus = false;
          break;
      case 0:
          oStatus = EStatus::TimeoutReached;
          break;

      default: /* available to read */
          oStatus = true;
          break;
  } // end switch( selectStatus )
  return oStatus;
}

size_t CcLinuxSocketTcp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = 0;
  // Send an initial buffer
  ssize_t iResult = ::send(m_hClientSocket, buf, bufSize, 0);
  if (iResult < 0)
  {
    CCVERBOSE("write failed with error: " + CcString::fromNumber(errno));
    uiRet = SIZE_MAX;
  }
  else
  {
    uiRet = static_cast<size_t>(iResult);
  }
  return uiRet;
}

CcStatus CcLinuxSocketTcp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  // Create a SOCKET for connecting to server
  m_hClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  if (m_hClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG("CcLinuxSocketTcp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  return oResult;
}

CcStatus CcLinuxSocketTcp::close()
{
  CcStatus oRet=false;
  if(m_hClientSocket >= 0)
  {
    oRet = ::close(m_hClientSocket);
    m_hClientSocket = -1;
  }
  return oRet;
}

CcStatus CcLinuxSocketTcp::cancel()
{
  CcStatus oRet(false);
  if (-1 != shutdown(m_hClientSocket, SHUT_RDWR))
  {
    oRet = true;
    m_hClientSocket = -1;
  }
  return oRet;
}
