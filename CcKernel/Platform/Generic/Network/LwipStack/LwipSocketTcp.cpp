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

LwipSocketTcp::LwipSocketTcp(netconn* pNetconn) :
  ILwipSocket(pNetconn)
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
  err_t iResult;
  if (m_pNetconn == nullptr)
  {
    oResult.setSystemError(errno);
  }
  else
  {
    ip_addr_t oIpAddress;
    oIpAddress.addr = m_oConnectionInfo.getIp().getUint32(false);
    iResult = ::netconn_bind(m_pNetconn, &oIpAddress, m_oConnectionInfo.getPort());
    if (iResult != ERR_OK)
    {
      oResult.setSystemError(iResult);
      CCDEBUG("LwipSocketTcp::bind failed with error: " + CcString::fromNumber(iResult));
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
      if (open())
      {
        ip_addr_t* pAddress;
        ip_addr_t oIpAddress;
        oIpAddress.addr = m_oConnectionInfo.getIp().getUint32(false);
        if(oIpAddress.addr == 0)
        {
          pAddress = nullptr;
        }
        else
        {
          pAddress = &oIpAddress;
        }

        err_t iErr = netconn_connect(m_pNetconn, pAddress, m_oConnectionInfo.getPort());
        if (iErr != ERR_OK)
        {
          oStatus.setSystemError(iErr);
          CCERROR("socket failed with error: " + CcString::fromNumber(iErr));
          break;
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
  if(::netconn_listen(m_pNetconn) == ERR_OK)
  {
    oRet = true;
  }
  return oRet;
}

#include "CcMutex.h"
CcMutex LwipSocketTcp_MutexRead;
CcMutex LwipSocketTcp_MutexWrite;

ISocket* LwipSocketTcp::accept()
{
  m_bAccepting = true;
  // Accept a client socket
  ISocket *sRet = nullptr;
  netconn* pNewNetconn;
  err_t iError = ::netconn_accept(m_pNetconn, &pNewNetconn);
  if (pNewNetconn == nullptr ||
      iError != ERR_OK)
  {
    CCERROR("accept failed with error: " + CcString::fromNumber(iError));
  }
  else
  {
    CCNEW(sRet, LwipSocketTcp, pNewNetconn);
  }
  m_bAccepting = false;
  return sRet;
}

size_t LwipSocketTcp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  LwipSocketTcp_MutexRead.lock();
  CCDEBUG("Read start");
  if(m_pTempBuffer && m_uiTempBufferSize > 0)
  {
    if(m_uiTempBufferSize < bufSize)
    {
      uiRet = m_uiTempBufferSize;
      CcStatic::memcpy(buf, m_pTempBuffer, m_uiTempBufferSize);
      m_pTempBuffer = nullptr;
      m_uiTempBufferSize = 0;
    }
    else
    {
      uiRet = bufSize;
      CcStatic::memcpy(buf, m_pTempBuffer, bufSize);
      m_pTempBuffer += bufSize;
      m_uiTempBufferSize -= bufSize;
    }
  }
  else if (m_pNetconn != nullptr)
  {
    netbuf* pBuf;
    err_t iResult = ::netconn_recv(m_pNetconn, &pBuf);
    if (iResult == ERR_OK)
    {
      void *data;
      u16_t len;
      netbuf_data(pBuf, &data, &len);
      if(len < bufSize)
      {
        uiRet = len;
        CcStatic::memcpy(buf, data, uiRet);
      }
      else
      {
        uiRet = bufSize;
        CcStatic::memcpy(buf, data, uiRet);
        m_pTempBuffer = static_cast<char*>(data) + bufSize;
        m_uiTempBufferSize = len - bufSize;
      }
    }
    else
    {
      close();
    }
  }
  CCDEBUG("Read done");
  LwipSocketTcp_MutexRead.unlock();
  return uiRet;
}

size_t LwipSocketTcp::write(const void *buf, size_t bufSize)
{
  size_t uiRet;
  // Send an initial buffer
  LwipSocketTcp_MutexWrite.lock();
  CCDEBUG("Write start");
  err_t iResult = netconn_write_partly(m_pNetconn, buf, bufSize, NETCONN_COPY, nullptr);
  if (iResult != ERR_OK)
  {
    close();
    uiRet = SIZE_MAX;
    CCDEBUG("write failed with error: " + CcString::fromNumber(iResult));
  }
  else
  {
    //while(uiRet != bufSize)
    //{
    //  CCDEBUG("Write still in progress");
    //  CcKernel::sleep(10);
    //}
  }
  CCDEBUG("Write done");
  LwipSocketTcp_MutexWrite.unlock();
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
      oRet = ::netconn_shutdown(m_pNetconn, 1, 1);
      m_bAccepting = false;
    }
    else
    {
      CCERROR("close handle: " + CcString::fromNumber(reinterpret_cast<uintptr>(m_pNetconn)));
      oRet = ::netconn_close(m_pNetconn);
      m_pNetconn = nullptr;
    }
  }
  return oRet;
}

CcStatus LwipSocketTcp::cancel()
{
  CcStatus oRet(false);
  oRet = ::netconn_shutdown(m_pNetconn, 1, 1);
  return oRet;
}
