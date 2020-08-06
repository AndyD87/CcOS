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
  extern CcKernelSHARED CcConstString_H(CcOS);
  extern CcKernelSHARED CcConstString_H(Empty);
  extern CcKernelSHARED CcConstString_H(Space);
  extern CcKernelSHARED CcConstString_H(True);
  extern CcKernelSHARED CcConstString_H(False);
  extern CcKernelSHARED CcConstString_H(On);
  extern CcKernelSHARED CcConstString_H(Off);
  extern CcKernelSHARED CcConstString_H(Null);
  extern CcKernelSHARED CcConstString_H(Negative);
  extern CcKernelSHARED CcConstString_H(EolShort);
  extern CcKernelSHARED CcConstString_H(EolLong);
  extern CcKernelSHARED CcConstString_H(EolCr);
  extern const CcKernelSHARED CcString& Cr;
#ifdef WINDOWS
  extern const CcKernelSHARED CcString& EolOs;
#else
  extern const CcKernelSHARED CcString& EolOs;
#endif
  namespace EnvVars
  {
    extern CcKernelSHARED CcConstString_H(AppNoIoBuffering);
  }
  namespace Seperators
  {
    extern CcKernelSHARED CcConstString_H(DoubleDot);
    extern CcKernelSHARED CcConstString_H(Dot);
    extern CcKernelSHARED CcConstString_H(Pipe);
    extern CcKernelSHARED CcConstString_H(Comma);
    extern CcKernelSHARED CcConstString_H(Colon);
    extern CcKernelSHARED CcConstString_H(Semicolon);
    extern CcKernelSHARED CcConstString_H(Slash);
    extern CcKernelSHARED CcConstString_H(BackSlash);
    extern CcKernelSHARED CcConstString_H(DoubleSlashes);
    extern const CcKernelSHARED CcString& Space;
    extern const CcKernelSHARED CcString& Path;
    extern const CcKernelSHARED CcString& IpV4;
    extern const CcKernelSHARED CcString& MacAddress;
    extern CcKernelSHARED CcConstString_H(MacAddressMinus);
    extern CcKernelSHARED CcConstString_H(QuestionMark);
    extern CcKernelSHARED CcConstString_H(Quote);
    extern CcKernelSHARED CcConstString_H(QuoteEscaped);
    extern CcKernelSHARED CcConstString_H(Ampersand);
    extern CcKernelSHARED CcConstString_H(Equal);
  }
  namespace Brackets
  {
    extern CcKernelSHARED CcConstString_H(Left);
    extern CcKernelSHARED CcConstString_H(Right);
    extern CcKernelSHARED CcConstString_H(SquareLeft);
    extern CcKernelSHARED CcConstString_H(SquareRight);
    extern CcKernelSHARED CcConstString_H(CurlyLeft);
    extern CcKernelSHARED CcConstString_H(CurlyRight);
  }
  namespace Types
  {
    namespace Hash
    {
      extern CcKernelSHARED CcConstString_H(Crc32);
      extern CcKernelSHARED CcConstString_H(Md5);
      extern CcKernelSHARED CcConstString_H(Sha256);
    }
  }
  namespace Names
  {
    extern CcKernelSHARED CcConstString_H(CcProcessThreadName);
  }
  namespace Numbers
  {
    extern CcKernelSHARED CcConstString_H(i0);
    extern CcKernelSHARED CcConstString_H(i1);
    extern CcKernelSHARED CcConstString_H(i2);
    extern CcKernelSHARED CcConstString_H(i3);
    extern CcKernelSHARED CcConstString_H(i4);
    extern CcKernelSHARED CcConstString_H(i5);
    extern CcKernelSHARED CcConstString_H(i6);
    extern CcKernelSHARED CcConstString_H(i7);
    extern CcKernelSHARED CcConstString_H(i8);
    extern CcKernelSHARED CcConstString_H(i9);
  }
  namespace Prefixes
  {
    extern CcKernelSHARED CcConstString_H(Lib);
  }
  namespace Extensions
  {
    extern CcKernelSHARED CcConstString_H(Bin);
    extern CcKernelSHARED CcConstString_H(Bmp);
    extern CcKernelSHARED CcConstString_H(Gif);
    extern CcKernelSHARED CcConstString_H(Jpg);
    extern CcKernelSHARED CcConstString_H(Png);
    extern CcKernelSHARED CcConstString_H(Pbm);
    extern CcKernelSHARED CcConstString_H(Pgm);
    extern CcKernelSHARED CcConstString_H(Ppm);
    extern CcKernelSHARED CcConstString_H(Dll);
    extern CcKernelSHARED CcConstString_H(So);
    namespace System
    {
      extern const CcKernelSHARED CcString& DynamicLibraryWindows;
      extern const CcKernelSHARED CcString& DynamicLibraryCommon;
      extern const CcKernelSHARED CcString& DynamicLibrary;
    }
  }
}

#endif // H_CcGlobalStrings_H_
