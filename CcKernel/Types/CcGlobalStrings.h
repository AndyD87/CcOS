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

class CcGlobalStrings
{
public:
  static CcKernelSHARED CcConstString_H(CcOS);
  static CcKernelSHARED CcConstString_H(Empty);
  static CcKernelSHARED CcConstString_H(Space);
  static CcKernelSHARED CcConstString_H(True);
  static CcKernelSHARED CcConstString_H(False);
  static CcKernelSHARED CcConstString_H(On);
  static CcKernelSHARED CcConstString_H(Off);
  static CcKernelSHARED CcConstString_H(Null);
  static CcKernelSHARED CcConstString_H(Negative);
  static CcKernelSHARED CcConstString_H(EolShort);
  static CcKernelSHARED CcConstString_H(EolLong);
  static CcKernelSHARED CcConstString_H(EolCr);
  static const CcKernelSHARED CcString& Cr;
#ifdef WINDOWS
  static const CcKernelSHARED CcString& EolOs;
#else
  static const CcKernelSHARED CcString& EolOs;
#endif
  struct EnvVars
  {
    static CcKernelSHARED CcConstString_H(AppNoIoBuffering);
  };
  struct Seperators
  {
    static CcKernelSHARED CcConstString_H(DoubleDot);
    static CcKernelSHARED CcConstString_H(Dot);
    static CcKernelSHARED CcConstString_H(Pipe);
    static CcKernelSHARED CcConstString_H(Comma);
    static CcKernelSHARED CcConstString_H(Colon);
    static CcKernelSHARED CcConstString_H(Semicolon);
    static CcKernelSHARED CcConstString_H(Slash);
    static CcKernelSHARED CcConstString_H(BackSlash);
    static CcKernelSHARED CcConstString_H(DoubleSlashes);
    static const CcKernelSHARED CcString& Space;
    static const CcKernelSHARED CcString& Path;
    static const CcKernelSHARED CcString& IpV4;
    static const CcKernelSHARED CcString& MacAddress;
    static CcKernelSHARED CcConstString_H(MacAddressMinus);
    static CcKernelSHARED CcConstString_H(QuestionMark);
    static CcKernelSHARED CcConstString_H(Quote);
    static CcKernelSHARED CcConstString_H(QuoteEscaped);
    static CcKernelSHARED CcConstString_H(Ampersand);
    static CcKernelSHARED CcConstString_H(Equal);
  };
  struct Brackets
  {
    static CcKernelSHARED CcConstString_H(Left);
    static CcKernelSHARED CcConstString_H(Right);
    static CcKernelSHARED CcConstString_H(SquareLeft);
    static CcKernelSHARED CcConstString_H(SquareRight);
    static CcKernelSHARED CcConstString_H(CurlyLeft);
    static CcKernelSHARED CcConstString_H(CurlyRight);
  };
  struct Types
  {
    struct Hash
    {
      static CcKernelSHARED CcConstString_H(Crc32);
      static CcKernelSHARED CcConstString_H(Md5);
      static CcKernelSHARED CcConstString_H(Sha256);
    };
  };
  struct Names
  {
    static CcKernelSHARED CcConstString_H(CcProcessThreadName);
  };
  struct Numbers
  {
    static CcKernelSHARED CcConstString_H(i0);
    static CcKernelSHARED CcConstString_H(i1);
    static CcKernelSHARED CcConstString_H(i2);
    static CcKernelSHARED CcConstString_H(i3);
    static CcKernelSHARED CcConstString_H(i4);
    static CcKernelSHARED CcConstString_H(i5);
    static CcKernelSHARED CcConstString_H(i6);
    static CcKernelSHARED CcConstString_H(i7);
    static CcKernelSHARED CcConstString_H(i8);
    static CcKernelSHARED CcConstString_H(i9);
  };
  struct Prefixes
  {
    static CcKernelSHARED CcConstString_H(Lib);
  };
  struct Extensions
  {
    static CcKernelSHARED CcConstString_H(Bin);
    static CcKernelSHARED CcConstString_H(Bmp);
    static CcKernelSHARED CcConstString_H(Gif);
    static CcKernelSHARED CcConstString_H(Jpg);
    static CcKernelSHARED CcConstString_H(Png);
    static CcKernelSHARED CcConstString_H(Pbm);
    static CcKernelSHARED CcConstString_H(Pgm);
    static CcKernelSHARED CcConstString_H(Ppm);
    static CcKernelSHARED CcConstString_H(Dll);
    static CcKernelSHARED CcConstString_H(So);
    struct System
    {
      static const CcKernelSHARED CcString& DynamicLibraryWindows;
      static const CcKernelSHARED CcString& DynamicLibraryCommon;
      static const CcKernelSHARED CcString& DynamicLibrary;
    };
  };

  static void init();
};

#endif // H_CcGlobalStrings_H_
