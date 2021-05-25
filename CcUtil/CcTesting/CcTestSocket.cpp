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
 * @brief     Implemtation of class CcTestSocket
 */
#include "CcTestSocket.h"
#include "CcTesting.h"

CcTestSocket::CcTestSocket() :
  ISocket(ESocketType::TCP)
{

}

CcTestSocket::~CcTestSocket()
{

}

ISocket* CcTestSocket::accept()
{
  CCNEWTYPE(pSocket, CcTestSocket);
  return pSocket;
}

CcSocketAddressInfo CcTestSocket::getHostByName(const CcString& CCUNUSED_PARAM(sHostname))
{
  return CcSocketAddressInfo();
}

CcStatus CcTestSocket::setOption(ESocketOption CCUNUSED_PARAM(eOption), void* CCUNUSED_PARAM(pData), size_t CCUNUSED_PARAM(uiDataLen))
{
  return EStatus::NotSupported;
}

CcStatus CcTestSocket::setOptionRaw(int CCUNUSED_PARAM(iLevel), int CCUNUSED_PARAM(iOptName), void* CCUNUSED_PARAM(pData), size_t CCUNUSED_PARAM(uiDataLen))
{
  return EStatus::NotSupported;
}

size_t CcTestSocket::read(void* pBuffer, size_t uSize)
{ 
  size_t uiData = m_oReadData.read(pBuffer, uSize); 
  m_oReadData.remove(0, uSize);
  return uiData;
}

size_t CcTestSocket::write(const void* pBuffer, size_t uSize)
{
  m_oReadData.append(static_cast<const char*>(pBuffer), uSize);
  return uSize;
}