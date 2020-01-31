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
#include "CcBase.h"

#ifndef H_CcGlobalStrings_H_
#define H_CcGlobalStrings_H_

namespace CcGlobalStrings
{
  extern const CcKernelSHARED CcString CcOS;
  extern const CcKernelSHARED CcString Empty;
  extern const CcKernelSHARED CcString Space;
  extern const CcKernelSHARED CcString True;
  extern const CcKernelSHARED CcString False;
  extern const CcKernelSHARED CcString On;
  extern const CcKernelSHARED CcString Off;
  extern const CcKernelSHARED CcString Null;
  extern const CcKernelSHARED CcString Negative;
  extern const CcKernelSHARED CcString EolShort;
  extern const CcKernelSHARED CcString EolLong;
  extern const CcKernelSHARED CcString EolCr;
#ifdef WINDOWS
  extern const CcKernelSHARED CcString& EolOs;
#else
  extern const CcKernelSHARED CcString& EolOs;
#endif
  namespace EnvVars
  {
    extern const CcKernelSHARED CcString AppNoIoBuffering;
  }
  namespace Seperators
  {
    extern const CcKernelSHARED CcString Dot;
    extern const CcKernelSHARED CcString Pipe;
    extern const CcKernelSHARED CcString Comma;
    extern const CcKernelSHARED CcString Colon;
    extern const CcKernelSHARED CcString Semicolon;
    extern const CcKernelSHARED CcString Slash;
    extern const CcKernelSHARED CcString BackSlash;
    extern const CcKernelSHARED CcString DoubleSlashes;
    extern const CcKernelSHARED CcString& Space;
    extern const CcKernelSHARED CcString& Path;
    extern const CcKernelSHARED CcString& IpV4;
    extern const CcKernelSHARED CcString& MacAddress;
    extern const CcKernelSHARED CcString MacAddressMinus;
    extern const CcKernelSHARED CcString QuestionMark;
    extern const CcKernelSHARED CcString Quote;
    extern const CcKernelSHARED CcString QuoteEscaped;
    extern const CcKernelSHARED CcString Ampersand;
    extern const CcKernelSHARED CcString Equal;
  }
  namespace Brackets
  {
    extern const CcKernelSHARED CcString Left;
    extern const CcKernelSHARED CcString Right;
    extern const CcKernelSHARED CcString SquareLeft;
    extern const CcKernelSHARED CcString SquareRight;
    extern const CcKernelSHARED CcString CurlyLeft;
    extern const CcKernelSHARED CcString CurlyRight;
  }
  namespace Types
  {
    namespace Hash
    {
      extern const CcKernelSHARED CcString Crc32;
      extern const CcKernelSHARED CcString Md5;
      extern const CcKernelSHARED CcString Sha256;
    }
  }
  namespace Names
  {
    extern const CcKernelSHARED CcString CcProcessThreadName;
  }
  namespace Numbers
  {
    extern const CcKernelSHARED CcString i0;
    extern const CcKernelSHARED CcString i1;
    extern const CcKernelSHARED CcString i2;
    extern const CcKernelSHARED CcString i3;
    extern const CcKernelSHARED CcString i4;
    extern const CcKernelSHARED CcString i5;
    extern const CcKernelSHARED CcString i6;
    extern const CcKernelSHARED CcString i7;
    extern const CcKernelSHARED CcString i8;
    extern const CcKernelSHARED CcString i9;
  }
}

#endif // H_CcGlobalStrings_H_
