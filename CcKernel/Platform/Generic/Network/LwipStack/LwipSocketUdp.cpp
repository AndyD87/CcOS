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
#include "lwip/api.h"

LwipSocketUdp::LwipSocketUdp() :
  ILwipSocket(ESocketType::UDP)
{
}

LwipSocketUdp::LwipSocketUdp(netconn* pNetconn) :
  ILwipSocket(pNetconn)
{
}

LwipSocketUdp::~LwipSocketUdp()
{
  close();
}

CcStatus LwipSocketUdp::setAddressInfo(const CcSocketAddressInfo &oAddrInfo)
{
  close();
  m_oConnectionInfo = oAddrInfo;
  return open();
}

CcStatus LwipSocketUdp::bind()
{
  CcStatus oResult;
  int iResult;
  if (m_pNetconn == nullptr)
  {
    oResult.setSystemError(errno);
    CCDEBUG( "LwipSocketUdp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  else
  {
    ip_addr_t oIpAddress;
    oIpAddress.addr = m_oConnectionInfo.getIp().getUint32(false);
    iResult = ::netconn_bind(m_pNetconn, &oIpAddress, m_oConnectionInfo.getPort());
    if (iResult != ERR_OK)
    {
      oResult.setSystemError(iResult);
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
  if (m_pNetconn != nullptr)
  {
    netbuf* pBuf;
    err_t iResult = ::netconn_recv(m_pNetconn, &pBuf);
    if (iResult == 0)
    {
      void *data;
      u16_t len;
      netbuf_data(pBuf, &data, &len);
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

size_t LwipSocketUdp::write(const void *buf, size_t bufSize)
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

CcStatus LwipSocketUdp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  CcStatus oResult;
  close();
  // Create a SOCKET for connecting to server
  m_pNetconn = netconn_new(NETCONN_UDP);
  if (m_pNetconn == nullptr)
  {
    oResult.setSystemError(errno);
    CCDEBUG("LwipSocketUdp::bind socket failed with error: " + CcString::fromNumber(errno));
  }
  return oResult;
}

CcStatus LwipSocketUdp::close()
{
  CcStatus oRet=false;
  if(m_pNetconn != nullptr)
  {
    oRet = ::netconn_close(m_pNetconn);
    m_pNetconn = nullptr;
  }
  return oRet;
}

CcStatus LwipSocketUdp::cancel()
{
  CcStatus oRet(false);
  oRet = ::netconn_shutdown(m_pNetconn, 1, 1);
  return oRet;
}
