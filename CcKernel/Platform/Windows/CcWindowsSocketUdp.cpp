/*
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
 * @brief     Class CcWindowsSocketUdp
 */
#include "CcWindowsSocketUdp.h"
#include <io.h>
#include <fcntl.h>
#include "CcKernel.h"
#include "CcDateTime.h"

CcWindowsSocketUdp::CcWindowsSocketUdp() :
  CcWindowsSocketBase(ESocketType::UDP)
{
}

CcWindowsSocketUdp::CcWindowsSocketUdp(SOCKET socket, sockaddr sockAddr, int sockAddrlen) :
  CcWindowsSocketBase(socket, sockAddr, sockAddrlen)
{
}

CcWindowsSocketUdp::~CcWindowsSocketUdp( void )
{
}

CcStatus CcWindowsSocketUdp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  // Create a SOCKET for connecting to server
  m_ClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  if (m_ClientSocket == INVALID_SOCKET)
  {
    oResult.setSystemError(WSAGetLastError());
    CCDEBUG("CcWindowsSocketUdp::bind socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
  }
  return oResult;
}

CcStatus CcWindowsSocketUdp::setAddressInfo(const CcSocketAddressInfo& oAddrInfo)
{
  CcStatus oResult;
  m_oConnectionInfo = oAddrInfo;
  return oResult;
}

CcStatus CcWindowsSocketUdp::bind()
{
  CcStatus oResult;
  if (m_ClientSocket == INVALID_SOCKET &&
      open() == false)
  {
    oResult.setSystemError(WSAGetLastError());
    CCDEBUG( "CcWindowsSocketUdp::bind socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
  }
  else
  {
    int iResult = ::bind(m_ClientSocket, static_cast<sockaddr*>(m_oConnectionInfo.sockaddr()), (int) m_oConnectionInfo.ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
      oResult.setSystemError(WSAGetLastError());
      CCDEBUG("CcWindowsSocketUdp::bind failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
    }
  }
  return oResult;
}

CcStatus CcWindowsSocketUdp::connect()
{
  bool bRet = false;
  return bRet;
}

CcStatus CcWindowsSocketUdp::listen(void)
{
  bool bRet = false;
  if (!::listen(m_ClientSocket, 0))
    bRet = true;
  else
    CCDEBUG("CcWindowsSocketUdp::listen failed with error: " + CcString::fromNumber(WSAGetLastError()));
  return bRet;
}

ISocket* CcWindowsSocketUdp::accept(void)
{
  // Accept a client socket
  ISocket *sRet = nullptr;
  SOCKET Temp;
  sockaddr sockAddr;
  int sockAddrlen=sizeof(sockAddr);
  Temp = ::accept(m_ClientSocket, &sockAddr, &sockAddrlen);
  if (Temp == INVALID_SOCKET) 
  {
    CCDEBUG("CcWindowsSocketUdp::accept failed with error: " + CcString::fromNumber(WSAGetLastError()));
    close();
  }
  else
  {
    sRet = new CcWindowsSocketUdp(Temp, sockAddr, sockAddrlen); 
    CCMONITORNEW(sRet);
  }
  return sRet;
}

size_t CcWindowsSocketUdp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  if (m_ClientSocket != INVALID_SOCKET)
  {
    int iResult = 0;
    int iFromSize = static_cast<int>(m_oPeerInfo.ai_addrlen);
    iResult = ::recvfrom(m_ClientSocket, static_cast<char*>(buf), (int) bufSize, 0, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), &iFromSize);
    if (iResult < 0)
    {
      CCDEBUG("CcWindowsSocketUdp::read failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
    }
    else
    {
      uiRet = static_cast<size_t>(iResult);
    }
  }
  return uiRet;
}

size_t CcWindowsSocketUdp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  int iResult;
  int iFromSize = static_cast<int>(m_oPeerInfo.ai_addrlen);
  int iBufferSize = static_cast<int>(bufSize);
  iResult = ::sendto(m_ClientSocket, static_cast<const char*>(buf), iBufferSize, 0, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), iFromSize);
  if (iResult < 0)
  {
    CCDEBUG("CcWindowsSocketUdp::write failed with error: " + CcString::fromNumber(WSAGetLastError()));
    close();
    return SIZE_MAX;
  }
  else
  {
    uiRet = static_cast<size_t>(iResult);
  }
  return iResult;
}

CcStatus CcWindowsSocketUdp::close(void)
{
  bool bRet(false);
  if (SOCKET_ERROR != closesocket(m_ClientSocket))
  {
    bRet = true;
    m_ClientSocket = INVALID_SOCKET;
  }
  return bRet;
}

CcStatus CcWindowsSocketUdp::cancel(void)
{
  return true;
}

size_t CcWindowsSocketUdp::readTimeout(char *buf, size_t bufSize, time_t timeout) 
{
  size_t iRet = 0;
  fd_set readfds;
  struct timeval tv;
  int rv;
  FD_ZERO(&readfds);
  FD_SET(m_ClientSocket, &readfds);

  tv.tv_sec = 0;
  tv.tv_usec = (long)timeout;
  rv = select((int)m_ClientSocket+1, &readfds, nullptr, nullptr, &tv);

  if (rv == -1) 
  {
    CCERROR("CcWindowsSocketUdp::readTimeout error occured in select");
  }
  else if (rv == 0)
  {
    CCWARNING("CcWindowsSocketUdp::readTimeout Timeout occured while reading");
  }
  else 
  {
    // one or both of the descriptors have data
    if (FD_ISSET(m_ClientSocket, &readfds)) {
      iRet = recv(m_ClientSocket, buf, (int)bufSize, 0);
    }
  }
  return iRet;
}
