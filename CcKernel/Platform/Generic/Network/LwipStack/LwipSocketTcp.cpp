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
#include "lwip/api.h"

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
  if (m_pNetconn != nullptr)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "LwipSocketTcp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    iResult = ::netconn_bind(m_pNetconn, m_oConnectionInfo.getIp().getUint32(false), m_oConnectionInfo.getPort());
    if (iResult != 0)
    {
      oResult.setSystemError(errno);
      CCDEBUG("LwipSocketTcp::bind failed with error: " + CcString::fromNumber(errno));
      close();
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
  iResult = getaddrinfo(m_oConnectionInfo.getIpString().getCharString(), m_oConnectionInfo.getPortString().getCharString(), &hints, &result);
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
        iResult = ::lwip_connect(m_hClientSocket, ptr->ai_addr, static_cast<socklen_t>(ptr->ai_addrlen));
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

CcStatus LwipSocketTcp::listen()
{
  CcStatus oRet(false);
  if(::netconn_listen(m_pNetconn) == 0)
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
  netconn* pNewNetconn;
  Temp = ::netconn_accept(m_pNetconn, &pNewNetconn);
  if (Temp < 0)
  {
    CCERROR("accept failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    CCNEW(sRet, LwipSocketTcp, Temp, pNewNetconn);
  }
  m_bAccepting = false;
  return sRet;
}

size_t LwipSocketTcp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  if (m_pNetconn != nullptr)
  {
    netbuf* pBuf;
    err_t iResult = ::netconn_recv(m_pNetconn, &pBuf);
    if (iResult == 0)
    {
      void *data;
      u16_t len;
      netbuf_data(nb, &data, &len);
      uiRet = CCMIN(len, bufSize);
      CcStatic::memcpy(buf, data, uiRet);
      close();
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
  err_t iResult = ::netconn_write(m_pNetconn, buf, bufSize, NETCONN_COPY);
  if (iResult != 0)
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

CcStatus LwipSocketTcp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  close();
  // Create a SOCKET for connecting to server
  m_pNetconn = netconn_new(NETCONN_TCP);
  if (m_pNetconn == nullptr)
  {
    oResult.setSystemError(errno);
    CCDEBUG("LwipSocketTcp::open socket failed with error: " + CcString::fromNumber(oResult.getSystemError()));
  }
  return oResult;
}

CcStatus LwipSocketTcp::close()
{
  CcStatus oRet=false;
  if(m_pNetconn != nullptr)
  {
    if(m_bAccepting)
    {
      oRet = ::netconn_shutdown(m_pNetconn, NETCONN_SHUT_RD, NETCONN_SHUT_WR);
      m_bAccepting = false;
    }
    else
    {
      oRet = ::netconn_close(m_pNetconn);
      m_pNetconn = nullptr;
    }
  }
  return oRet;
}

CcStatus LwipSocketTcp::cancel()
{
  CcStatus oRet(false);
  oRet = ::netconn_shutdown(m_pNetconn, NETCONN_SHUT_RD, NETCONN_SHUT_WR);
  return oRet;
}
