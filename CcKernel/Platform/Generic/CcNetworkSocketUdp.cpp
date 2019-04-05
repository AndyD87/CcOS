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
 * @brief     Implementation of Class CcNetworkSocketUdp
 */
#include "CcNetworkSocketUdp.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include "CcStatic.h"


CcNetworkSocketUdp::CcNetworkSocketUdp() :
  INetworkSocket(ESocketType::UDP)
{
}

CcNetworkSocketUdp::~CcNetworkSocketUdp()
{
}

CcStatus CcNetworkSocketUdp::setAddressInfo(const CcSocketAddressInfo &oAddrInfo)
{
  CcStatus oResult;
  return oResult;
}

CcStatus CcNetworkSocketUdp::bind()
{
  CcStatus oResult;
  return oResult;
}

CcStatus CcNetworkSocketUdp::connect()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus CcNetworkSocketUdp::listen()
{
  CcStatus oRet(false);
  return oRet;
}

ISocket* CcNetworkSocketUdp::accept()
{
  return nullptr;
}

size_t CcNetworkSocketUdp::read(void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  return uiRet;
}

size_t CcNetworkSocketUdp::write(const void *buf, size_t bufSize)
{
  size_t uiRet = SIZE_MAX;
  return uiRet;
}

CcStatus CcNetworkSocketUdp::open(EOpenFlags eFlags)
{
  CCUNUSED(eFlags);
  return false;
}

CcStatus CcNetworkSocketUdp::close()
{
  CcStatus oRet=false;
  return oRet;
}

CcStatus CcNetworkSocketUdp::cancel()
{
  CcStatus oRet(false);
  return oRet;
}

size_t CcNetworkSocketUdp::readTimeout(char *buf, size_t bufSize, time_t timeout)
{
  size_t iRet = 0;
  return iRet;
}

bool CcNetworkSocketUdp::insertPacket(CcNetworkPacket* pPacket)
{
  CCUNUSED_TODO(pPacket);
}
