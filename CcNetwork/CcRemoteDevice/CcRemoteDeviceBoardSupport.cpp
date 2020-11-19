/*
 * This file is part of CcRemoteDeviceBoardSupport.
 *
 * CcRemoteDeviceBoardSupport is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceBoardSupport is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceBoardSupport.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceBoardSupport
 */

#include "CcRemoteDeviceBoardSupport.h"
#include "CcKernel.h"
#include "CcDeviceList.h"

class CcRemoteDeviceBoardSupport::CPrivate
{
public:
  CcDeviceList                              oDeviceList;
  CcRemoteDeviceBoardSupport::CPortPinArray oArray;
};

CcRemoteDeviceBoardSupport::CcRemoteDeviceBoardSupport()
{
  CCNEW(m_pPrivate, CPrivate);
  initBoard();
}

CcRemoteDeviceBoardSupport::~CcRemoteDeviceBoardSupport()
{
  CCDELETE(m_pPrivate);
}

void CcRemoteDeviceBoardSupport::addDevice(const CcDevice& oHandle)
{
  m_pPrivate->oDeviceList.add(oHandle);
}

bool CcRemoteDeviceBoardSupport::hasGpio()
{
  return m_pPrivate->oArray.size() > 0;
}

bool CcRemoteDeviceBoardSupport::hasLan()
{
  return m_pPrivate->oDeviceList.getDevice(EDeviceType::Network).isValid();
}

bool CcRemoteDeviceBoardSupport::hasWlanAccessPoint()
{
  return m_pPrivate->oDeviceList.getDevice(EDeviceType::WlanAccessPoint).isValid();
}

bool CcRemoteDeviceBoardSupport::hasWlanClient()
{
  return m_pPrivate->oDeviceList.getDevice(EDeviceType::WlanClient).isValid();
}

const CcRemoteDeviceBoardSupport::CPortPinArray &CcRemoteDeviceBoardSupport::getGpioPins() const
{
  return m_pPrivate->oArray;
}

CcRemoteDeviceBoardSupport::CPortPinArray &CcRemoteDeviceBoardSupport::getGpioPins()
{
  return m_pPrivate->oArray;
}
