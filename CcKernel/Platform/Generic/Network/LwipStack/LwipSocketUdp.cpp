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
 * @brief     Implementation of Class LwipSocketUdp
 */
#include "LwipSocketUdp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"


LwipSocketUdp::LwipSocketUdp() :
  ILwipSocket(ESocketType::UDP)
{
}

LwipSocketUdp::LwipSocketUdp(int socket, sockaddr sockAddr, int sockAddrlen) :
  ILwipSocket(socket, sockAddr, sockAddrlen)
{
}

LwipSocketUdp::~LwipSocketUdp()
{
  int iResult;
  if (m_hClientSocket >= 0)
  {
    iResult = ::lwip_shutdown(m_hClientSocket, SHUT_RDWR);
    m_hClientSocket = -1;
    if (iResult != 0)
    {
      close();
    }
  }
}

CcStatus LwipSocketUdp::setAddressInfo(const CcSocketAddressInfo &oAddrInfo)
{
  CcStatus oResult;
  m_oConnectionInfo = oAddrInfo;
  // Create a SOCKET for connecting to server
  m_hClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  return oResult;
}

CcStatus LwipSocketUdp::bind()
{
  CcStatus oResult;
  int iResult;
  if (m_hClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "LwipSocketUdp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    sockaddr* pSockaddrIn = static_cast<sockaddr*>(m_oConnectionInfo.sockaddr());
    pSockaddrIn->sa_family = AF_INET;
    pSockaddrIn->sa_len = 0;
    iResult = ::lwip_bind(m_hClientSocket, pSockaddrIn, (int) m_oConnectionInfo.ai_addrlen);
    if (iResult != 0)
    {
      oResult.setSystemError(errno);
      CCDEBUG("LwipSocketUdp::bind failed with error: " + CcString::fromNumber(errno));
      close();
    }
  }
  return oResult;
}

CcStatus LwipSocketUdp::connect()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus LwipSocketUdp::listen()
{
  CcStatus oRet(false);
  return oRet;
}

ISocket* LwipSocketUdp::accept()
{
  return nullptr;
}

size_t LwipSocketUdp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  if (m_hClientSocket >= 0)
  {
    socklen_t iFromSize = static_cast<socklen_t>(m_oPeerInfo.ai_addrlen);
    int iResult = ::recvfrom(m_hClientSocket, static_cast<char*>(buf), (int) bufSize, 0, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), &iFromSize);
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
  return uiRet;
}

size_t LwipSocketUdp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = 0;
  // Send an initial buffer
  int iFromSize = static_cast<int>(m_oPeerInfo.ai_addrlen);
  int iBufferSize = static_cast<int>(bufSize);
  int iResult = ::sendto(m_hClientSocket, static_cast<const char*>(buf), iBufferSize, 0, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), iFromSize);
  if (iResult < 0)
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

CcStatus LwipSocketUdp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  // Create a SOCKET for connecting to server
  m_hClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  if (m_hClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG("LwipSocketUdp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  return oResult;
}

CcStatus LwipSocketUdp::close()
{
  CcStatus oRet=false;
  if(m_hClientSocket >= 0)
  {
    oRet = ::close(m_hClientSocket);
    m_hClientSocket = -1;
  }
  return oRet;
}

CcStatus LwipSocketUdp::cancel()
{
  CcStatus oRet(false);
  if (::lwip_shutdown(m_hClientSocket, SHUT_RDWR) >= 0)
  {
    oRet = true;
    m_hClientSocket = 0;
  }
  return oRet;
}
