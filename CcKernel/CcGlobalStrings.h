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
 * @page      CcKernel
 * @subpage   CcGlobalStrings
 *
 * @page      CcGlobalStrings
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     namespace CcSystem
 */
#include "CcString.h"
#include "CcKernelBase.h"

#ifndef CcGlobalStrings_H_
#define CcGlobalStrings_H_

namespace CcGlobalStrings
{
  extern const CcKernelSHARED CcString Empty;
  extern const CcKernelSHARED CcString True;
  extern const CcKernelSHARED CcString False;
  extern const CcKernelSHARED CcString On;
  extern const CcKernelSHARED CcString Off;
  extern const CcKernelSHARED CcString Null;
  extern const CcKernelSHARED CcString EolShort;
  extern const CcKernelSHARED CcString EolLong;
  extern const CcKernelSHARED CcString EolCr;
#if WIN32
  extern const CcKernelSHARED CcString& EolOs;
#else
  extern const CcKernelSHARED CcString& EolOs;
#endif
  namespace Seperators
  {
    extern const CcKernelSHARED CcString Pipe;
    extern const CcKernelSHARED CcString Comma;
    extern const CcKernelSHARED CcString Colon;
    extern const CcKernelSHARED CcString Semicolon;
    extern const CcKernelSHARED CcString Slash;
    extern const CcKernelSHARED CcString& Path;
    extern const CcKernelSHARED CcString& IpV4;
    extern const CcKernelSHARED CcString& MacAddress;
    extern const CcKernelSHARED CcString MacAddress2;

  }
}

#endif