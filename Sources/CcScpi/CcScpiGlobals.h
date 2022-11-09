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
 */

#include "CcBase.h"
#include "CcScpi.h"
#include "CcString.h"


namespace CcScpiGlobals
{
  namespace ErrorMsgs
  {
    extern const CcScpiSHARED CcString NoError;
    extern const CcScpiSHARED CcString FileNotFound;
    extern const CcScpiSHARED CcString AccessViolation;
    extern const CcScpiSHARED CcString DiskFull;
    extern const CcScpiSHARED CcString IllegalOperation;
    extern const CcScpiSHARED CcString UnknownTransfer;
    extern const CcScpiSHARED CcString FileAlreadyExists;
    extern const CcScpiSHARED CcString NoSuchUser;
    extern const CcScpiSHARED CcString Termination;
    extern const CcScpiSHARED CcString Unknown;
  }
};
