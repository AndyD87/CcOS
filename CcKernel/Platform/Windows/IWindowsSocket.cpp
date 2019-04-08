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
 * @brief     Class IWindowsSocket
 */
#include "IWindowsSocket.h"
#include <io.h>
#include <fcntl.h>
#include "CcKernel.h"
#include "CcDateTime.h"

//Statics
bool IWindowsSocket::g_sWsaStarted = false;
CRITICAL_SECTION IWindowsSocket::m_CS;

IWindowsSocket::IWindowsSocket(ESocketType type) :
  ISocket(type),
  m_oConnectionInfo(type),
  m_hClientSocket(INVALID_SOCKET)
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

IWindowsSocket::IWindowsSocket(SOCKET socket, sockaddr sockAddr, int sockAddrlen) :
  ISocket(ESocketType::TCP),
  m_hClientSocket(socket)
{
  CCUNUSED(sockAddrlen);
  m_oConnectionInfo.setAddressData( (CcTypes_sockaddr_in*)&sockAddr, sizeof(sockAddr));
  int len = static_cast<int>(m_oPeerInfo.ai_addrlen);
  getpeername(m_hClientSocket, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), &len);
}

void IWindowsSocket::startWSA()
{
  WSADATA wsaData;
  if (!g_sWsaStarted)
  {
    g_sWsaStarted = true;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
      CCDEBUG("IWindowsSocket::startWSA unable to start");
  }
}

IWindowsSocket::~IWindowsSocket()
{
  close();
}

CcStatus IWindowsSocket::setOption(ESocketOption eOption, void* pData, size_t uiDataLen)
{
  CcStatus oStatus;
  switch (eOption)
  {
    case ESocketOption::Broadcast:
    {
      INT32 iEnable = 1;
      if (pData != nullptr && uiDataLen >= sizeof(INT32))
      {
        iEnable = *static_cast<INT32*>(pData);
      }
      oStatus = setOptionRaw(SOL_SOCKET, SO_BROADCAST, &iEnable, sizeof(iEnable));
      break;
    }
    case ESocketOption::Reuse:
    {
      INT32 iEnable = 1;
      if (pData != nullptr && uiDataLen >= sizeof(INT32))
      {
        iEnable = *static_cast<INT32*>(pData);
      }
      oStatus = setOptionRaw(SOL_SOCKET, SO_REUSEADDR, &iEnable, sizeof(iEnable));
      break;
    }
    default:
      oStatus = false;
      break;
  }
  return oStatus;
}

CcStatus IWindowsSocket::setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen)
{
  CcStatus oStatus;
  int iResult = setsockopt(m_hClientSocket, iLevel, iOptName, static_cast<char*>(pData), static_cast<int>(uiDataLen));
  if (iResult != 0)
  {
    oStatus.setSystemError(WSAGetLastError());
  }
  return oStatus;
}

CcSocketAddressInfo IWindowsSocket::getHostByName(const CcString& hostname)
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

void IWindowsSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  DWORD uiMilliseconds = static_cast<DWORD>(uiTimeValue.getTimestampMs());
  if(setsockopt(m_hClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&uiMilliseconds, sizeof(uiMilliseconds)) != 0)
    CCDEBUG("Socket set receive Timeout failed");
  if(setsockopt(m_hClientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&uiMilliseconds, sizeof(uiMilliseconds)) != 0)
    CCDEBUG("Socket set send Timeout failed");
}

CcSocketAddressInfo IWindowsSocket::getPeerInfo()
{
  return m_oPeerInfo;
}

void IWindowsSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oPeerInfo = oPeerInfo;
}

CcStatus IWindowsSocket::close()
{
  bool bRet(false);
  if (SOCKET_ERROR != closesocket(m_hClientSocket))
  {
    bRet = true;
    m_hClientSocket = INVALID_SOCKET;
  }
  return bRet;
}

CcStatus IWindowsSocket::cancel()
{
  return true;
}

size_t IWindowsSocket::readTimeout(void *buf, size_t bufSize, const CcDateTime& oTimeout)
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
    CCERROR("IWindowsSocket::readTimeout error occured in select");
  }
  else if (rv == 0)
  {
    CCWARNING("IWindowsSocket::readTimeout Timeout occured while reading");
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
