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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSocket
 */
#include "Network/CcSocket.h"
#include "CcKernel.h"

CcSocket::CcSocket() :
  ISocket(ESocketType::Unknown),
  m_pSystemSocket(nullptr)
{
}

CcSocket::CcSocket(ESocketType type):
  ISocket(type)
{
  m_pSystemSocket = CcKernel::getSocket(type);
}

CcSocket::CcSocket(ISocket* pSocketImport) :
  m_pSystemSocket(pSocketImport)
{
}

CcSocket::CcSocket(const CcSocket& oToCopy) :
  ISocket(oToCopy)
{
  operator=(oToCopy);
}

CcSocket::CcSocket(CcSocket&& oToMove) :
  ISocket(oToMove)
{
  operator=(CCMOVE(oToMove));
}

CcSocket::~CcSocket()
{
}

CcSocket& CcSocket::operator=(ISocket* pToSet)
{
  // close socket if one is set
  close();
  // lock is required, because it would delete the previous one.
  m_oLock.lock();
  m_pSystemSocket = pToSet;
  m_oLock.unlock();
  return *this;
}

CcSocket& CcSocket::operator=(CcSocket&& oToMove)
{
  if (this != &oToMove)
  {
    ISocket::operator = (CCMOVE(oToMove));
    m_pSystemSocket   =  CCMOVE(oToMove.m_pSystemSocket);
  }
  return *this;
}

CcSocket& CcSocket::operator=(const CcSocket& oToCopy)
{
  ISocket::operator=(oToCopy);
  // close socket if one is set
  if(m_pSystemSocket.isValid())
    m_pSystemSocket->close();
  m_oLock.lock();
  m_pSystemSocket = oToCopy.m_pSystemSocket;
  m_oLock.unlock();
  return *this;
}

size_t CcSocket::read(void* pBuffer, size_t uSize)
{
  size_t uiRead = SIZE_MAX;
  if (m_pSystemSocket != nullptr &&
      m_oLock.tryLock() == true)
  {
    uiRead = m_pSystemSocket->read(pBuffer, uSize);
    m_oLock.unlock();
  }
  return uiRead;
}

size_t CcSocket::write(const void* pBuffer, size_t uSize)
{
  size_t uiWritten = SIZE_MAX;
  if (m_pSystemSocket != nullptr &&
      m_oLock.tryLock() == true)
  {
    uiWritten = m_pSystemSocket->write(pBuffer, uSize);
    m_oLock.unlock();
  }
  return uiWritten;
}

CcStatus CcSocket::open(EOpenFlags oFlags)
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->open(oFlags);
  }
  m_oLock.unlock();
  return oStatus;
}

CcStatus CcSocket::close()
{
  CcStatus oStatus(false);
  // Save socket to avoid delete
  CcSharedPointer<ISocket> pSocket = m_pSystemSocket;
  if (!m_oLock.tryLock())
  {
    if (pSocket != nullptr)
    {
      oStatus = pSocket->cancel();
    }
    m_oLock.lock();
    if (pSocket != nullptr)
    {
      oStatus = pSocket->close();
    }
  }
  if (pSocket != nullptr)
  {
    oStatus = pSocket->close();
  }
  m_pSystemSocket = nullptr;
  m_oLock.unlock();
  return oStatus;
}

CcStatus CcSocket::cancel()
{
  CcStatus oStatus(false);
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->cancel();
  }
  return oStatus;
}

CcStatus CcSocket::bind(uint16 Port)
{
  CcStatus bSuccess = false;
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    CcSocketAddressInfo oAddrInfo;
    oAddrInfo.init(m_pSystemSocket->getType());
    oAddrInfo.setPort(Port);
    m_pSystemSocket->setAddressInfo(oAddrInfo);
    bSuccess = m_pSystemSocket->bind();
  }
  m_oLock.unlock();
  return bSuccess;
}

CcStatus CcSocket::setAddressInfo(const CcSocketAddressInfo& oAddrInfo)
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->setAddressInfo(oAddrInfo);
  }
  m_oLock.unlock();
  return oStatus;
}


CcSocketAddressInfo& CcSocket::getAddressInfo()
{
  CcSocketAddressInfo* pAddrInfo = &CcSocketAddressInfo::getInvalid();
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    pAddrInfo = &m_pSystemSocket->getAddressInfo();
  }
  m_oLock.unlock();
  return *pAddrInfo;
}


CcStatus CcSocket::bind()
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->bind();
  }
  else
  {
    CCDEBUG("CcSocket::bind invalid socket");
  }
  m_oLock.unlock();
  return oStatus;
}

CcStatus CcSocket::bind(const CcSocketAddressInfo& oAddressInfo)
{
  setAddressInfo(oAddressInfo);
  return bind();
}

CcStatus CcSocket::connect()
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->connect();
  }
  m_oLock.unlock();
  return oStatus;
}

CcStatus CcSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  setPeerInfo(oAddressInfo);
  return connect();
}

CcStatus CcSocket::listen()
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->listen();
  }
  m_oLock.unlock();
  return oStatus;
}

ISocket* CcSocket::accept()
{
  ISocket* pSocket = nullptr;
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    pSocket =  m_pSystemSocket->accept();
  }
  m_oLock.unlock();
  return pSocket;
}

CcStatus CcSocket::setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode)
{
  CcStatus oSuccess;
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oSuccess = m_pSystemSocket->setTimeout(uiTimeValue, eMode);
  }
  m_oLock.unlock();
  return oSuccess;
}

CcSocketAddressInfo CcSocket::getHostByName(const CcString& hostname)
{
  CcSocketAddressInfo oAddressInfo;
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oAddressInfo = m_pSystemSocket->getHostByName(hostname);
  }
  m_oLock.unlock();
  return oAddressInfo;
}

CcSocketAddressInfo& CcSocket::getPeerInfo()
{
  CcSocketAddressInfo* oAddressInfo = &CcSocketAddressInfo::getInvalid();
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oAddressInfo = &m_pSystemSocket->getPeerInfo();
  }
  m_oLock.unlock();
  return *oAddressInfo;
}

void CcSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    m_pSystemSocket->setPeerInfo(oPeerInfo);
  }
  m_oLock.unlock();
}

CcStatus CcSocket::setOption(ESocketOption eOption, void* pData, size_t uiDataLen)
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->setOption(eOption, pData, uiDataLen);
  }
  m_oLock.unlock();
  return oStatus;
}

CcStatus CcSocket::setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen)
{
  CcStatus oStatus(false);
  m_oLock.lock();
  if (m_pSystemSocket != nullptr)
  {
    oStatus = m_pSystemSocket->setOptionRaw(iLevel, iOptName, pData, uiDataLen);
  }
  m_oLock.unlock();
  return oStatus;
}

CcStatus CcSocket::connect(const CcString& hostName, const CcString& hostPort)
{
  CcSocketAddressInfo oAddressInfo;
  oAddressInfo = getHostByName(hostName);
  oAddressInfo.setPort(hostPort.toUint16());
  return connect(oAddressInfo);
}
