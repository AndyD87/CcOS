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
 * @brief     Implementation of Class LwipSocketTcp
 */

#include "LwipSocketTcp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"

LwipSocketTcp::LwipSocketTcp() :
  ILwipSocket(ESocketType::TCP)
{
}

LwipSocketTcp::LwipSocketTcp(int socket, sockaddr sockAddr, uint32 sockAddrlen) :
  ILwipSocket(socket, sockAddr, sockAddrlen)
{
}

LwipSocketTcp::~LwipSocketTcp()
{
  close();
}

CcStatus LwipSocketTcp::setAddressInfo(const CcSocketAddressInfo &oAddrInfo)
{
  close();
  m_oConnectionInfo = oAddrInfo;
  return open();
}

CcStatus LwipSocketTcp::bind()
{
  CcStatus oResult;
  int iResult;
  CCDEBUG( "LwipSocketTcp::bind");
  // Create a SOCKET for connecting to server
  if (m_hClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "LwipSocketTcp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    static_cast<sockaddr_in*>(m_oConnectionInfo.sockaddr())->sin_family = 2;
    static_cast<sockaddr_in*>(m_oConnectionInfo.sockaddr())->sin_len = sizeof(sockaddr_in);
    iResult = ::lwip_bind(m_hClientSocket, static_cast<sockaddr*>(m_oConnectionInfo.sockaddr()), static_cast<socklen_t>(m_oConnectionInfo.ai_addrlen));
    if (iResult != 0)
    {
      oResult.setSystemError(errno);
    }
  }
  return oResult;
}

CcStatus LwipSocketTcp::connect()
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
    CCERROR("getaddrinfo failed with error: " + CcString::fromNumber(oStatus.getErrorUint()));
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
        CCERROR("socket failed with error: " + CcString::fromNumber(oStatus.getErrorUint()));
        break;
      }
      else
      {
        // Connect to server.
        iResult = lwip_connect(m_hClientSocket, ptr->ai_addr, static_cast<socklen_t>(ptr->ai_addrlen));
        if (iResult < 0)
        {
          oStatus.setSystemError(iResult);
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

CcStatus LwipSocketTcp::listen()
{
  CcStatus oRet(false);
  if(lwip_listen(m_hClientSocket, 0) == 0)
  {
    oRet = true;
  }
  return oRet;
}

ISocket* LwipSocketTcp::accept()
{
  m_bAccepting = true;
  // Accept a client socket
  ISocket *sRet = nullptr;
  int Temp;
  sockaddr sockAddr;
  socklen_t sockAddrlen=sizeof(sockAddr);
  Temp = lwip_accept(m_hClientSocket, &sockAddr, &sockAddrlen);
  if (Temp < 0)
  {
    CCERROR("accept failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    CCNEW(sRet, LwipSocketTcp, Temp, sockAddr, sockAddrlen);
  }
  m_bAccepting = false;
  return sRet;
}

size_t LwipSocketTcp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  if (m_hClientSocket >= 0)
  {
    ssize_t iResult = ::recv(m_hClientSocket, buf, bufSize, 0);
    if (iResult < 0)
    {
      CCERROR("read failed with error: " + CcString::fromNumber(errno) );
    }
    else
    {
      uiRet = static_cast<size_t>(iResult);
    }
  }
  return uiRet;
}

size_t LwipSocketTcp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = 0;
  // Send an initial buffer
  ssize_t iResult = lwip_send(m_hClientSocket, buf, bufSize, 0);
  if (iResult < 0)
  {
    CCERROR("write failed with error: " + CcString::fromNumber(errno));
    uiRet = SIZE_MAX;
  }
  else
  {
    uiRet = static_cast<size_t>(iResult);
  }
  return uiRet;
}

CcStatus LwipSocketTcp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  CCDEBUG("LwipSocketTcp open");
  // Create a SOCKET for connecting to server
  m_hClientSocket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_hClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG("LwipSocketTcp::open socket failed with error: " + CcString::fromNumber(oResult.getSystemError()));
  }
  return oResult;
}

CcStatus LwipSocketTcp::close()
{
  CcStatus oRet=false;
  if(m_hClientSocket >= 0)
  {
    if(m_bAccepting)
    {
      oRet = lwip_shutdown(m_hClientSocket, SHUT_RDWR);
      m_bAccepting = false;
    }
    else
    {
      oRet = lwip_shutdown(m_hClientSocket, SHUT_RDWR);
      // Wait for all done until we can close connection
      CcKernel::sleep(300);
      oRet = lwip_close(m_hClientSocket);
      m_hClientSocket = -1;
    }
  }
  return oRet;
}

CcStatus LwipSocketTcp::cancel()
{
  CcStatus oRet(false);
  if (-1 != lwip_shutdown(m_hClientSocket, SHUT_RDWR))
  {
    oRet = true;
    m_hClientSocket = -1;
  }
  return oRet;
}
