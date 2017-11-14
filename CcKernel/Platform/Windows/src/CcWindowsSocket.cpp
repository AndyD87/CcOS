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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindowsSocket
 */
#include "CcWindowsSocket.h"
#include <io.h>
#include <fcntl.h>
#include "CcKernel.h"
#include "CcDateTime.h"

//Statics
bool CcWindowsSocket::g_sWsaStarted = false;
CRITICAL_SECTION CcWindowsSocket::m_CS;

CcWindowsSocket::CcWindowsSocket(ESocketType type) :
  CcSocket(type),
  m_ClientSocket(INVALID_SOCKET)
{
  if (g_sWsaStarted)
  {
    EnterCriticalSection(&m_CS);
    LeaveCriticalSection(&m_CS);
  }
  else
  {
    InitializeCriticalSection(&m_CS);
    EnterCriticalSection(&m_CS);
    startWSA();
    LeaveCriticalSection(&m_CS);
  }
}

CcWindowsSocket::CcWindowsSocket(SOCKET socket, sockaddr sockAddr, int sockAddrlen) :
  CcSocket(ESocketType::TCP),
  m_ClientSocket(socket),
  m_sockAddr(sockAddr),
  m_sockAddrlen(sockAddrlen)
{
  int len = sizeof(m_oPeerInfo);
  getpeername(m_ClientSocket, (struct sockaddr*)&m_oPeerInfo, &len);
}

void CcWindowsSocket::startWSA(void)
{
  WSADATA wsaData;
  if (!g_sWsaStarted)
  {
    g_sWsaStarted = true;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
      CCDEBUG("CcWindowsSocket::startWSA unable to start");
  }
}

CcWindowsSocket::~CcWindowsSocket( void )
{
  close();
}

CcStatus CcWindowsSocket::bind(uint16 Port)
{
  int iResult;
  struct addrinfo *result = nullptr;
  struct addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  switch (m_SockType)
  {
    case ESocketType::TCP:
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_PASSIVE;
      break;
    case ESocketType::UDP:
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      hints.ai_protocol = IPPROTO_UDP;
      break;
  }

  // Resolve the server address and port
  CcString sPort;
  sPort.appendNumber(Port);
  iResult = getaddrinfo(nullptr, sPort.getCharString(), &hints, &result);
  if (iResult != 0) 
  {
    CCDEBUG( "CcWindowsSocket::bind getaddrinfo failed with error: " + CcString::fromNumber(iResult));
    return false;
  }

  // Create a SOCKET for connecting to server
  m_ClientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (m_ClientSocket == INVALID_SOCKET)
  {
    CCDEBUG( "CcWindowsSocket::bind socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
    freeaddrinfo(result);
    return false;
  }

  switch (m_SockType)
  {
    case ESocketType::TCP:
      // Setup the TCP listening socket
      iResult = ::bind(m_ClientSocket, result->ai_addr, (int) result->ai_addrlen);
      if (iResult == SOCKET_ERROR)
      {
        CCDEBUG("CcWindowsSocket::bind failed with error: " + CcString::fromNumber(WSAGetLastError()));
        freeaddrinfo(result);
        close();
        return false;
      }
      break;
    case ESocketType::UDP:
    {
      struct    sockaddr_in servaddr;  /*  socket address structure  */
      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      servaddr.sin_port = htons(Port);

      if (::bind(m_ClientSocket, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
      {
        CCERROR("ERROR IN BIND \n");
      }
    }
  }
  return true;
}

CcStatus CcWindowsSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  m_oConnectionInfo = oAddressInfo;
  CcString sHostname;
  sHostname.appendIPv4(m_oConnectionInfo.getIPv4());
  CcString sPort;
  sPort.appendNumber(m_oConnectionInfo.getPort());
  return connect(sHostname, sPort);
}

CcStatus CcWindowsSocket::connect(const CcString& hostName, const CcString& hostPort)
{
  bool bRet = true;
  struct addrinfo *result = nullptr,
                  *ptr = nullptr,
                  hints;
  int iResult;

  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(hostName.getCharString(), hostPort.getCharString(), &hints, &result);
  if ( iResult != 0 ) 
  {
    CCDEBUG("CcWindowsSocket::connect getaddrinfo failed with error: " + CcString::fromNumber(iResult));
    bRet = false;
  }

  // Attempt to connect to an address until one succeeds
  for(ptr=result; ptr != nullptr && bRet == true ;ptr=ptr->ai_next) 
  {
    // Create a SOCKET for connecting to server
    m_ClientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (m_ClientSocket == INVALID_SOCKET) 
    {
      CCDEBUG("CcWindowsSocket::connect socket failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
      bRet = false;
    }
    else
    {
      // Connect to server.
      iResult = ::connect(m_ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
      if (iResult == SOCKET_ERROR) 
      {
        close();
        m_ClientSocket = INVALID_SOCKET;
      }
      else
      {
        break;
      }
    }
  }
  freeaddrinfo(result);
  if (m_ClientSocket == INVALID_SOCKET) 
  {
    CCDEBUG("CcWindowsSocket::connect unable to connect to server");
    bRet = false;
  }
  return bRet;
}

CcStatus CcWindowsSocket::listen(void)
{
  bool bRet = false;
  if (!::listen(m_ClientSocket, 0))
    bRet = true;
  else
    CCDEBUG("CcWindowsSocket::listen failed with error: " + CcString::fromNumber(WSAGetLastError()));
  return bRet;
}

CcSocket* CcWindowsSocket::accept(void)
{
  // Accept a client socket
  CcSocket *sRet = nullptr;
  SOCKET Temp;
  sockaddr sockAddr;
  int sockAddrlen=sizeof(sockAddr);
  Temp = ::accept(m_ClientSocket, &sockAddr, &sockAddrlen);
  if (Temp == INVALID_SOCKET) 
  {
    CCDEBUG("CcWindowsSocket::accept failed with error: " + CcString::fromNumber(WSAGetLastError()));
    close();
  }
  else
  {
    sRet = new CcWindowsSocket(Temp, sockAddr, sockAddrlen); 
    CCMONITORNEW(sRet);
  }
  return sRet;
}

size_t CcWindowsSocket::read(void *buf, size_t bufSize)
{
  size_t recSize = SIZE_MAX;
  if (m_ClientSocket != INVALID_SOCKET)
  {
    if (m_SockType == ESocketType::TCP)
    {
      recSize = ::recv(m_ClientSocket, static_cast<char*>(buf), (int) bufSize, 0);
    }
    else
    {
      int iFromSize = static_cast<int>(sizeof(m_oPeerInfo));
      recSize = ::recvfrom(m_ClientSocket, static_cast<char*>(buf), (int) bufSize, 0, (sockaddr *) &m_oPeerInfo, &iFromSize);
    }
    if (recSize == SOCKET_ERROR)
    {
      CCDEBUG("CcWindowsSocket::read failed with error: " + CcString::fromNumber(WSAGetLastError()));
      close();
    }
  }
  return recSize;
}

size_t CcWindowsSocket::write(const void *buf, size_t bufSize)
{
  // Send an initial buffer
  size_t iResult;
  if (m_SockType == ESocketType::TCP)
  {
    iResult = ::send(m_ClientSocket, static_cast<const char*>(buf), (int) bufSize, 0);
  }
  else
  {
    int iFromSize = static_cast<int>(sizeof(m_oPeerInfo));
    iResult = ::sendto(m_ClientSocket, static_cast<const char*>(buf), (int) bufSize, 0, (sockaddr *) &m_oPeerInfo, iFromSize);
  }
  if (iResult == SOCKET_ERROR) 
  {
    CCDEBUG("CcWindowsSocket::write failed with error: " + CcString::fromNumber(WSAGetLastError()));
    close();
    return SIZE_MAX;
  }
  return iResult;
}

CcSocketAddressInfo CcWindowsSocket::getHostByName(const CcString& hostname)
{
  CcSocketAddressInfo oRetConnectionInfo;
  struct addrinfo *result = nullptr;
  struct addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  int iRet = getaddrinfo(hostname.getCharString(), nullptr, &hints, &result);
  sockaddr_in* pSock = (sockaddr_in*)result->ai_addr;
  if (iRet == 0)
  {
    oRetConnectionInfo.setAddressData((CcTypes_sockaddr_in*)pSock, sizeof(sockaddr));
  }
  return oRetConnectionInfo;
}

void CcWindowsSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  DWORD uiMilliseconds = static_cast<DWORD>(uiTimeValue.getTimestampMs());
  if(setsockopt(m_ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&uiMilliseconds, sizeof(uiMilliseconds)) != 0)
    CCDEBUG("Socket set receive Timeout failed");
  if(setsockopt(m_ClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&uiMilliseconds, sizeof(uiMilliseconds)) != 0)
    CCDEBUG("Socket set send Timeout failed");
}

CcStatus CcWindowsSocket::close(void)
{
  bool bRet(false);
  if (SOCKET_ERROR != closesocket(m_ClientSocket))
  {
    bRet = true;
    m_ClientSocket = INVALID_SOCKET;
  }
  return bRet;
}

CcStatus CcWindowsSocket::cancel(void)
{
  return true;
}

size_t CcWindowsSocket::readTimeout(char *buf, size_t bufSize, time_t timeout) 
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
    CCERROR("CcWindowsSocket::readTimeout error occured in select");
  }
  else if (rv == 0)
  {
    CCWARNING("CcWindowsSocket::readTimeout Timeout occured while reading");
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
