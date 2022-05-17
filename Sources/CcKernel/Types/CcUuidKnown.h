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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUuidKnown
 **/
#pragma once

#include "CcBase.h"
#include "CcUuid.h"

namespace CcUuidKnown
{
  namespace CcOS
  {
    //! CcOS Vendor id {a87f5d15-6a3c-47a9-8254-826347189ced}
    extern const CcKernelSHARED CcUuid CcVendor;
    //! CcOS default remote device id
    extern const CcKernelSHARED CcUuid CcRemoteDevice;
  }
}
