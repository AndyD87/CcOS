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
 * @file      LinuxSocket
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class LinuxSocket
 */
#include "LinuxSocket.h"
#include "CcKernel.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include <cstring>


LinuxSocket::LinuxSocket(ESocketType type) :
  m_Type(type),
  m_ClientSocket(0)
{
}

LinuxSocket::LinuxSocket(int socket, sockaddr sockAddr, int sockAddrlen) :
  m_ClientSocket(socket),
  m_sockAddr(sockAddr),
  m_sockAddrlen(sockAddrlen)
{
  m_oConnectionInfo.setAddressData( (CcTypes_sockaddr_in*)&sockAddr, sizeof(sockAddr));
}

LinuxSocket::~LinuxSocket( void )
{
  int iResult;
  if (m_ClientSocket != 0){
    iResult = shutdown(m_ClientSocket, SHUT_RDWR);
    if (iResult != 0) {
      close();
    }
  }
}

bool LinuxSocket::bind( uint16 Port)
{
  int iResult;
  struct addrinfo *result = 0;
  struct addrinfo aiHints;
  memset(&aiHints, 0, sizeof(aiHints));
  aiHints.ai_family = AF_INET;
  aiHints.ai_socktype = SOCK_STREAM;
  aiHints.ai_protocol = IPPROTO_TCP;
  aiHints.ai_flags = AI_PASSIVE;

  // Resolve the server address and port
  CcString sPort;
  sPort.appendNumber(Port);
  iResult = getaddrinfo(NULL, sPort.getCharString(), &aiHints, &result);
  if (iResult != 0) {
    CCERROR("getaddrinfo failed with error: " +CcString::fromNumber( iResult));
    return false;
  }

  // Create a SOCKET for connecting to server
  m_ClientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (m_ClientSocket < 0) {
    CCERROR("socket failed with error: " + CcString::fromNumber(errno));
    freeaddrinfo(result);
    return false;
  }

  // Setup the TCP listening socket
  iResult = ::bind(m_ClientSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult != 0) {
    CCERROR("bind failed with error: " + CcString::fromNumber(errno));
    freeaddrinfo(result);
    return false;
  }
  return true;
}

bool LinuxSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  return connect(oAddressInfo.getIPv4String(), oAddressInfo.getPortString());
}

bool LinuxSocket::connect(const CcString &hostName, const CcString &hostPort)
{
  struct addrinfo *result = NULL,
                  *ptr = NULL,
                  hints;
  int iResult;

  memset( &hints, 0, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(hostName.getCharString(), hostPort.getCharString(), &hints, &result);
  if ( iResult != 0 ) {
      CCERROR("getaddrinfo failed with error: " + CcString::fromNumber(iResult));
      return 1;
  }

  // Attempt to connect to an address until one succeeds
  for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

      // Create a SOCKET for connecting to server
    m_ClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
          ptr->ai_protocol);
    if (m_ClientSocket < 0) {
          CCERROR("socket failed with error: " + CcString::fromNumber(errno));
          return 1;
      }

      // Connect to server.
    iResult = ::connect(m_ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult < 0) {
      close();
      m_ClientSocket = -1;
      continue;
    }
    break;
  }
  freeaddrinfo(result);
  return 1;
}

bool LinuxSocket::listen(void)
{
  if(!::listen(m_ClientSocket, 0))
    return true;
  return false;
}

CcSocket* LinuxSocket::accept(void)
{
  // Accept a client socket
  CcSocket *sRet = 0;
  int Temp;
  sockaddr sockAddr;
  socklen_t sockAddrlen=sizeof(sockAddr);
  Temp = ::accept(m_ClientSocket, &sockAddr, &sockAddrlen);
  if (Temp < 0) {
    CCERROR("accept failed with error: " + CcString::fromNumber(errno));
    close();
  }
  else{
    sRet = new LinuxSocket(Temp, sockAddr, sockAddrlen);
  }
  return sRet;
}

size_t LinuxSocket::read(char *buf, size_t bufSize)
{
  size_t recSize = SIZE_MAX;
  if (m_ClientSocket >= 0)
  {
    recSize = ::recv(m_ClientSocket, buf, bufSize, 0);
    if (recSize == -1){
      CCERROR("accept failed with error: " + CcString::fromNumber(errno) );
      close();
    }
  }
  return recSize;
}

size_t LinuxSocket::write(const char *buf, size_t bufSize)
{
  // Send an initial buffer
  int iResult;
  iResult = ::send(m_ClientSocket, buf, bufSize, 0);
  if (iResult == -1) {
    CCERROR("accept failed with error: " + CcString::fromNumber(errno));
    close();
    return SIZE_MAX;
  }
  return iResult;
}

CcSocketAddressInfo LinuxSocket::getHostByName(const CcString& hostname)
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

void LinuxSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  timeval tv;
  tv.tv_usec = uiTimeValue.getMSecond();  /* 30 Secs Timeout */
  tv.tv_sec  = uiTimeValue.getSecond();
  setsockopt(m_ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
  setsockopt(m_ClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

bool LinuxSocket::close(void)
{
  bool bRet=false;
  if(m_ClientSocket != 0){
    bRet = ::close(m_ClientSocket);
  }
  return bRet;
}

bool LinuxSocket::cancel(void){
  bool bRet(false);
  if (-1 != shutdown(m_ClientSocket, SHUT_RDWR)){
    bRet = true;
    m_ClientSocket = 0;
  }
  return bRet;
}

size_t LinuxSocket::readTimeout(char *buf, size_t bufSize, time_t timeout) {
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

  if (rv == -1) {
    CCERROR("error occurred in select()");
  }
  else if (rv == 0) {
    CCVERBOSE("Timeout occurred!  No data after 10.5 seconds.");
  }
  else {
    // one or both of the descriptors have data
    if (FD_ISSET(m_ClientSocket, &readfds)) {
      iRet = recv(m_ClientSocket, buf, bufSize, 0);
    }
  }
  return iRet;
}
