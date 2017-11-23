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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSocket
 */
#include "Network/CcSocket.h"
#include "CcKernel.h"

CcSocket::CcSocket(ESocketType type):
  CcSocketAbstract(type)
{
  m_pSystemSocket = CcKernel::getSocket(type);
}

CcSocket::CcSocket(CcSocketAbstract* pSocketImport) :
  m_pSystemSocket(pSocketImport)
{
}

CcSocket::CcSocket(const CcSocket& oToCopy) :
  CcSocketAbstract(oToCopy)
{
  operator=(oToCopy);
}

CcSocket::CcSocket(CcSocket&& oToMove) :
  CcSocketAbstract(oToMove)
{
  operator=(std::move(oToMove));
}

CcSocket::~CcSocket(void)
{
}

CcSocket& CcSocket::operator=(const CcSocket& oToCopy)
{
  CcSocketAbstract::operator=(oToCopy);
  m_pSystemSocket = oToCopy.m_pSystemSocket;
  return *this;
}

CcSocket& CcSocket::operator=(CcSocket&& oToMove)
{
  CcSocketAbstract::operator=(std::move(oToMove));
  if (this != &oToMove)
  {
    m_pSystemSocket = oToMove.m_pSystemSocket;
    oToMove.m_pSystemSocket = nullptr;
  }
  return *this;
}

bool CcSocket::operator==(const CcSocket& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcSocket::operator!=(const CcSocket& oToCompare) const
{
  return !operator==(oToCompare);
}

size_t CcSocket::read(void* buffer, size_t size)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->read(buffer, size);
  }
  return false;
}

size_t CcSocket::write(const void* buffer, size_t size)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->write(buffer, size);
  }
  return false;
}

CcStatus CcSocket::open(EOpenFlags oFlags)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->open(oFlags);
  }
  return false;
}

CcStatus CcSocket::close()
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->close();
  }
  return false;
}

CcStatus CcSocket::cancel()
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->cancel();
  }
  return false;
}

CcStatus CcSocket::bind(uint16 Port)
{
  if (m_pSystemSocket != nullptr)
  {
    CcSocketAddressInfo oAddrInfo;
    oAddrInfo.setPort(Port);
    oAddrInfo.init(m_pSystemSocket->getType());
    return m_pSystemSocket->bind(oAddrInfo);
  }
  return false;
}

CcStatus CcSocket::bind(const CcSocketAddressInfo& oAddrInfo)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->bind(oAddrInfo);
  }
  return false;
}

CcStatus CcSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->connect(oAddressInfo);
  }
  return false;
}

CcStatus CcSocket::listen(void)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->listen();
  }
  return false;
}

CcSocketAbstract* CcSocket::accept(void)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->accept();
  }
  return nullptr;
}

void CcSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  if (m_pSystemSocket != nullptr)
  {
    m_pSystemSocket->setTimeout(uiTimeValue);
  }
}

CcSocketAddressInfo CcSocket::getHostByName(const CcString& hostname)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->getHostByName(hostname);
  }
  return CcSocketAddressInfo();
}

CcSocketAddressInfo CcSocket::getPeerInfo(void)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->getPeerInfo();
  }
  return CcSocketAddressInfo();
}

void CcSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  if (m_pSystemSocket != nullptr)
  {
    return m_pSystemSocket->setPeerInfo(oPeerInfo);
  }
}

CcStatus CcSocket::connect(const CcString& hostName, const CcString& hostPort)
{
  bool bRet;
  CcSocketAddressInfo oAddressInfo;
  oAddressInfo = getHostByName(hostName);
  oAddressInfo.setPort(hostPort.toUint16());
  bRet = connect(oAddressInfo);
  return bRet;
}
