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

class CcRemoteDeviceBoardSupport::CPrivate
{
public:
  static CcRemoteDeviceBoardSupport::SPortPin aPins[];
  static CcRemoteDeviceBoardSupport::SPortPinArray oArray;
};

CcRemoteDeviceBoardSupport::SPortPin CcRemoteDeviceBoardSupport::CPrivate::aPins[] =
{
  {"GPIO0", 0, 0, IGpioPin::EDirection::Output},
  {"GPIO1", 0, 1, IGpioPin::EDirection::Output},
  {"GPIO2", 0, 2, IGpioPin::EDirection::Output},
  {"GPIO3", 0, 3, IGpioPin::EDirection::Output}
};

CcRemoteDeviceBoardSupport::SPortPinArray CcRemoteDeviceBoardSupport::CPrivate::oArray =
{
  CCSIZEOFARRAY(CcRemoteDeviceBoardSupport::CPrivate::aPins),
  CcRemoteDeviceBoardSupport::CPrivate::aPins
};

CcRemoteDeviceBoardSupport::CcRemoteDeviceBoardSupport()
{
}

CcRemoteDeviceBoardSupport::~CcRemoteDeviceBoardSupport()
{
}

bool CcRemoteDeviceBoardSupport::hasGpio()
{
  return CcKernel::getDevice(EDeviceType::GpioPort).isValid();
}

bool CcRemoteDeviceBoardSupport::hasLan()
{
  return CcKernel::getDevice(EDeviceType::Network).isValid();
}

bool CcRemoteDeviceBoardSupport::hasWlanAccessPoint()
{
  return CcKernel::getDevice(EDeviceType::WlanAccessPoint).isValid();
}

bool CcRemoteDeviceBoardSupport::hasWlanClient()
{
  return CcKernel::getDevice(EDeviceType::WlanClient).isValid();
}

const CcRemoteDeviceBoardSupport::SPortPinArray &CcRemoteDeviceBoardSupport::getGpioPins() const
{
  return CPrivate::oArray;
}
