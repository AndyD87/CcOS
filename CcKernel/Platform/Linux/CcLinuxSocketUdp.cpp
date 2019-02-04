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
 * @brief     Implementation of Class CcLinuxSocketUdp
 */
#include "CcLinuxSocketUdp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"


CcLinuxSocketUdp::CcLinuxSocketUdp() :
  CcLinuxSocketBase(ESocketType::UDP)
{
}

CcLinuxSocketUdp::CcLinuxSocketUdp(int socket, sockaddr sockAddr, int sockAddrlen) :
  CcLinuxSocketBase(socket, sockAddr, sockAddrlen)
{
}

CcLinuxSocketUdp::~CcLinuxSocketUdp(void )
{
  int iResult;
  if (m_ClientSocket >= 0)
  {
    iResult = shutdown(m_ClientSocket, SHUT_RDWR);
    m_ClientSocket = -1;
    if (iResult != 0)
    {
      close();
    }
  }
}

CcStatus CcLinuxSocketUdp::setAddressInfo(const CcSocketAddressInfo &oAddrInfo)
{
  CcStatus oResult;
  m_oConnectionInfo = oAddrInfo;
  // Create a SOCKET for connecting to server
  m_ClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  return oResult;
}

CcStatus CcLinuxSocketUdp::bind()
{
  CcStatus oResult;
  int iResult;
  if (m_ClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "CcLinuxSocketUdp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    iResult = ::bind(m_ClientSocket, static_cast<sockaddr*>(m_oConnectionInfo.sockaddr()), (int) m_oConnectionInfo.ai_addrlen);
    if (iResult != 0)
    {
      oResult.setSystemError(errno);
      CCDEBUG("CcLinuxSocketUdp::bind failed with error: " + CcString::fromNumber(errno));
      close();
    }
  }
  return oResult;
}

CcStatus CcLinuxSocketUdp::connect()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus CcLinuxSocketUdp::listen(void)
{
  CcStatus oRet(false);
  return oRet;
}

CcSocketAbstract* CcLinuxSocketUdp::accept(void)
{
  return nullptr;
}

size_t CcLinuxSocketUdp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  // Send an initial buffer
  if (m_ClientSocket >= 0)
  {
    socklen_t iFromSize = static_cast<socklen_t>(m_oPeerInfo.ai_addrlen);
    int iResult = ::recvfrom(m_ClientSocket, static_cast<char*>(buf), (int) bufSize, 0, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), &iFromSize);
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

size_t CcLinuxSocketUdp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = 0;
  // Send an initial buffer
  int iFromSize = static_cast<int>(m_oPeerInfo.ai_addrlen);
  int iBufferSize = static_cast<int>(bufSize);
  int iResult = ::sendto(m_ClientSocket, static_cast<const char*>(buf), iBufferSize, 0, static_cast<sockaddr*>(m_oPeerInfo.sockaddr()), iFromSize);
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

CcStatus CcLinuxSocketUdp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  // Create a SOCKET for connecting to server
  m_ClientSocket = socket(m_oConnectionInfo.ai_family, m_oConnectionInfo.ai_socktype, m_oConnectionInfo.ai_protocol);
  if (m_ClientSocket < 0)
  {
    oResult.setSystemError(errno);
    CCDEBUG("CcLinuxSocketUdp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  return oResult;
}

CcStatus CcLinuxSocketUdp::close(void)
{
  CcStatus oRet=false;
  if(m_ClientSocket >= 0)
  {
    oRet = ::close(m_ClientSocket);
    m_ClientSocket = -1;
  }
  return oRet;
}

CcStatus CcLinuxSocketUdp::cancel(void)
{
  CcStatus oRet(false);
  if (shutdown(m_ClientSocket, SHUT_RDWR) >= 0)
  {
    oRet = true;
    m_ClientSocket = 0;
  }
  return oRet;
}

size_t CcLinuxSocketUdp::readTimeout(char *buf, size_t bufSize, time_t timeout)
{
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

  if (rv == -1)
  {
    iRet = SIZE_MAX;
    CCERROR("error occurred in select()");
  }
  else if (rv == 0)
  {
    CCVERBOSE("Timeout occurred!  No data after 10.5 seconds.");
  }
  else
  {
    // one or both of the descriptors have data
    if (FD_ISSET(m_ClientSocket, &readfds))
    {
      iRet = recv(m_ClientSocket, buf, bufSize, 0);
    }
  }
  return iRet;
}
