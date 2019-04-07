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
 * @brief     Implementation of Class INetworkSocket
 */
#include "INetworkSocket.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include <fcntl.h>
#include <stdio.h>
#include "errno.h"
#include "CcStatic.h"


INetworkSocket::INetworkSocket(CcNetworkStack* pStack, ESocketType eType) :
  ISocket(eType),
  m_pStack(pStack),
  m_oConnectionInfo(eType),
  m_oPeerInfo(eType)
{
}

INetworkSocket::~INetworkSocket()
{
}

CcSocketAddressInfo INetworkSocket::getHostByName(const CcString& hostname)
{
  CcSocketAddressInfo oRetConnectionInfo;
  return oRetConnectionInfo;
}

void INetworkSocket::setTimeout(const CcDateTime& uiTimeValue)
{
}

CcSocketAddressInfo INetworkSocket::getPeerInfo()
{
  return m_oPeerInfo;
}

void INetworkSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oPeerInfo = oPeerInfo;
}


CcStatus INetworkSocket::setOption(ESocketOption eOption, void* pData, size_t uiDataLen)
{
  CcStatus oStatus;
  return oStatus;
}

CcStatus INetworkSocket::setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen)
{
  CcStatus oStatus;
  return oStatus;
}
