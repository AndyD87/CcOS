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

class CcRemoteDeviceBoardSupport::CPrivate
{
public:
  static CcRemoteDeviceBoardSupport::SPortPin aPins[];
  static CcRemoteDeviceBoardSupport::SPortPinArray oArray;
};

CcRemoteDeviceBoardSupport::SPortPin CcRemoteDeviceBoardSupport::CPrivate::aPins[] =
{
  {0, 0},
  {0, 1},
  {0, 2},
  {0, 3}
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
  return true;
}

bool CcRemoteDeviceBoardSupport::hasLan()
{
  return false;
}

bool CcRemoteDeviceBoardSupport::hasWlanAccessPoint()
{
  return true;
}

bool CcRemoteDeviceBoardSupport::hasWlanClient()
{
  return true;
}

const CcRemoteDeviceBoardSupport::SPortPinArray &CcRemoteDeviceBoardSupport::getGpioPins() const
{
  return CPrivate::oArray;
}
