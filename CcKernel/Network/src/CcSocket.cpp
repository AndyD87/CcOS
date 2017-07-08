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

CcSocket::CcSocket(ESocketType type):
  CcSocketAbstract(type)
{
}

CcSocket::~CcSocket( void )
{
}

const CcSocketAddressInfo& CcSocket::getPeerInfo(void)
{
  return m_oPeerInfo;
}

void CcSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_oPeerInfo = oPeerInfo;
}

bool CcSocket::connect(const CcString& hostName, const CcString& hostPort)
{
  bool bRet;
  CcSocketAddressInfo oAddressInfo;
  oAddressInfo = getHostByName(hostName);
  oAddressInfo.setPort(hostPort.toUint16());
  bRet = connect(oAddressInfo);
  return bRet;
}