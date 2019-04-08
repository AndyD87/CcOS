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
 * @brief     Class CcWindowsSocketTcp
 */
#include "CcWindowsSocketTcp.h"
#include <io.h>
#include <fcntl.h>
#include "CcKernel.h"
#include "CcDateTime.h"

CcWindowsSocketTcp::CcWindowsSocketTcp() :
  IWindowsSocket(ESocketType::TCP)
{
}

CcWindowsSocketTcp::CcWindowsSocketTcp(SOCKET socket, sockaddr sockAddr, int sockAddrlen) :
  IWindowsSocket(socket, sockAddr, sockAddrlen)
{
}

CcWindowsSocketTcp::~CcWindowsSocketTcp()
{
}

CcStatus CcWindowsSocketTcp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  // Create a SOCKET for connecting to server
  m_hClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  if (m_hClientSocket == INVALID_SOCKET)
  {
    oResult.setSystemError(WSAGetLastError());
    CCDEBUG("CcWindowsSocketTcp::bind socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
  }
  return oResult;
}

CcStatus CcWindowsSocketTcp::setAddressInfo(const CcSocketAddressInfo& oAddrInfo)
{
  CcStatus oResult;
  m_oConnectionInfo = oAddrInfo;
  return oResult;
}

CcStatus CcWindowsSocketTcp::bind()
{
  CcStatus oResult;
  if (m_hClientSocket == INVALID_SOCKET &&
      open() == false)
  {
    oResult.setSystemError(WSAGetLastError());
    CCDEBUG( "CcWindowsSocketTcp::bind socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
  }
  else
  {
    int iResult = ::bind(m_hClientSocket, static_cast<sockaddr*>(m_oConnectionInfo.sockaddr()), (int) m_oConnectionInfo.ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
      oResult.setSystemError(WSAGetLastError());
      CCDEBUG("CcWindowsSocketTcp::bind failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
    }
  }
  return oResult;
}

CcStatus CcWindowsSocketTcp::connect()
{
  bool bRet = true;
  struct addrinfo *result = nullptr,
    *ptr = nullptr,
    hints;
  int iResult;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(m_oConnectionInfo.getIpString().getCharString(), m_oConnectionInfo.getPortString().getCharString(), &hints, &result);
  if (iResult != 0)
  {
    CCDEBUG("CcWindowsSocketTcp::connect getaddrinfo failed with error: " + CcString::fromNumber(iResult));
    bRet = false;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != nullptr && bRet == true; ptr = ptr->ai_next)
  {
    // Create a SOCKET for connecting to server
    m_hClientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (m_hClientSocket == INVALID_SOCKET)
    {
      CCDEBUG("CcWindowsSocketTcp::connect socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
      bRet = false;
    }
    else
    {
      // Connect to server.
      iResult = ::connect(m_hClientSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
      if (iResult == SOCKET_ERROR)
      {
        close();
        m_hClientSocket = INVALID_SOCKET;
      }
      else
      {
        break;
      }
    }
  }
  freeaddrinfo(result);
  if (m_hClientSocket == INVALID_SOCKET)
  {
    CCDEBUG("CcWindowsSocketTcp::connect unable to connect to server");
    bRet = false;
  }
  return bRet;
}

CcStatus CcWindowsSocketTcp::listen()
{
  bool bRet = false;
  if (!::listen(m_hClientSocket, 0))
    bRet = true;
  else
    CCDEBUG("CcWindowsSocketTcp::listen failed with error: " + CcString::fromNumber(WSAGetLastError()));
  return bRet;
}

ISocket* CcWindowsSocketTcp::accept()
{
  // Accept a client socket
  ISocket *sRet = nullptr;
  SOCKET Temp;
  sockaddr sockAddr;
  int sockAddrlen=sizeof(sockAddr);
  HANDLE hEvents[2];
  ULONG  setting[2] = {0, 0};
  m_hAbortEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

  hEvents[0] = CreateEvent(NULL, FALSE, FALSE, NULL); /* Will be signalled for an incoming connection. */
  hEvents[1] = m_hAbortEvent; /* Will be signalled by another thread to abort the accept operation. */

  WSAEventSelect(m_hClientSocket, hEvents[0], FD_ACCEPT); /* Puts socket in non-blocking mode and
                                              * tells windows to signal hEvents[0] when the
                                              * accept() has completed or an error occurs. */

  if (WaitForMultipleObjects(2, hEvents, FALSE, INFINITE) == WAIT_OBJECT_0)
  {
    /* An incoming connection has arrived or a network error has occurred. */
    Temp = ::accept(m_hClientSocket, &sockAddr, &sockAddrlen);
    if (Temp == INVALID_SOCKET)
    {
      CCDEBUG("CcWindowsSocketTcp::accept failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
    }
    else
    {
      sRet = new CcWindowsSocketTcp(Temp, sockAddr, sockAddrlen);
      CCMONITORNEW(sRet);
    }
  }
  else
  {
    Temp = INVALID_SOCKET;
  }

  // If this fail, our program is interrupted
  if (SOCKET_ERROR != WSAEventSelect(m_hClientSocket, m_hAbortEvent, 0))
  {
    // Cleanup event signalling
    CloseHandle(m_hAbortEvent);
  }
  m_hAbortEvent = NULL;

  /* Put sockets back into blocking mode */
  ioctlsocket(m_hClientSocket, FIONBIO, &setting[0]);
  ioctlsocket(Temp, FIONBIO, &setting[1]);

  return sRet;
}

size_t CcWindowsSocketTcp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  if (m_hClientSocket != INVALID_SOCKET)
  {
    int iResult = ::recv(m_hClientSocket, static_cast<char*>(buf), (int) bufSize, 0);
    if (iResult < 0)
    {
      CCDEBUG("CcWindowsSocketTcp::read failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
    }
    else
    {
      uiRet = static_cast<size_t>(iResult);
    }
  }
  return uiRet;
}

size_t CcWindowsSocketTcp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  int iResult = ::send(m_hClientSocket, static_cast<const char*>(buf), (int) bufSize, 0);
  if (iResult < 0)
  {
    CCDEBUG("CcWindowsSocketTcp::write failed with error: " + CcString::fromNumber(WSAGetLastError()));
    close();
    return SIZE_MAX;
  }
  else
  {
    uiRet = static_cast<size_t>(iResult);
  }
  return iResult;
}

CcStatus CcWindowsSocketTcp::close()
{
  bool bRet(false);
  if (m_hAbortEvent != NULL)
  {
    SetEvent(m_hAbortEvent);
  }
  if (SOCKET_ERROR != closesocket(m_hClientSocket))
  {
    bRet = true;
    m_hClientSocket = INVALID_SOCKET;
  }
  return bRet;
}

CcStatus CcWindowsSocketTcp::cancel()
{
  return true;
}

size_t CcWindowsSocketTcp::readTimeout(void *buf, size_t bufSize, const CcDateTime& oTimeout)
{
  size_t iRet = 0;
  fd_set readfds;
  struct timeval tv;
  int rv;
  FD_ZERO(&readfds);
  FD_SET(m_hClientSocket, &readfds);

  tv.tv_sec = 0;
  tv.tv_usec = (long)oTimeout.getTimestampUs();
  rv = select((int)m_hClientSocket+1, &readfds, nullptr, nullptr, &tv);

  if (rv == -1) 
  {
    CCERROR("CcWindowsSocketTcp::readTimeout error occured in select");
  }
  else if (rv == 0)
  {
    CCWARNING("CcWindowsSocketTcp::readTimeout Timeout occured while reading");
  }
  else 
  {
    // one or both of the descriptors have data
    if (FD_ISSET(m_hClientSocket, &readfds)) {
      iRet = recv(m_hClientSocket, static_cast<char*>(buf), (int)bufSize, 0);
    }
  }
  return iRet;
}
