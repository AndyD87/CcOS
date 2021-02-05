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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     namespace CcSystem
 */
#pragma once

#include "CcString.h"
#include "CcBase.h"

/**
 * @brief Common global strings, which are often used will be statically defined here.
 *        Do not access global variable in a static global variable, it might be initialized before
 *        this.
 */
class CcGlobalStrings
{
public:
  static CcKernelSHARED CcConstString_H(CcOS);        //!< CcOS as string
  static CcKernelSHARED CcConstString_H(Empty);       //!< Empty string
  static CcKernelSHARED CcConstString_H(Space);       //!< Single space
  static CcKernelSHARED CcConstString_H(True);        //!< String: true
  static CcKernelSHARED CcConstString_H(False);       //!< String: false
  static CcKernelSHARED CcConstString_H(On);          //!< String: on
  static CcKernelSHARED CcConstString_H(Off);         //!< String: off
  static CcKernelSHARED CcConstString_H(Null);        //!< String: null
  static CcKernelSHARED CcConstString_H(Negative);    //!< String: -
  static CcKernelSHARED CcConstString_H(EolShort);    //!< String: "\n"
  static CcKernelSHARED CcConstString_H(EolLong);     //!< String: "\r\n"
  static CcKernelSHARED CcConstString_H(EolCr);       //!< String: "\r"
  static const CcKernelSHARED CcString& Cr;           //!< String: "\r"
#ifdef WINDOWS
  static const CcKernelSHARED CcString& EolOs;        //!< String on windows: "\r\n"
#else
  static const CcKernelSHARED CcString& EolOs;        //!< String on linux: "\n"
#endif
  //! Common environtment variables
  struct EnvVars
  {
    static CcKernelSHARED CcConstString_H(Path);      //!< Path env variable for finding executable
    static CcKernelSHARED CcConstString_H(AppNoIoBuffering); //!< String for a better output handling: CCOS_APPS_NO_IO_BUFFERING
  };
  //! Common seperators, sometimes twice but referenced for a better readability
  struct Seperators
  {
    static CcKernelSHARED CcConstString_H(Dot);             //!< dot
    static CcKernelSHARED CcConstString_H(DoubleDot);       //!< String with two dots
    static CcKernelSHARED CcConstString_H(Pipe);            //!< pipe
    static CcKernelSHARED CcConstString_H(Comma);           //!< ,
    static CcKernelSHARED CcConstString_H(Colon);           //!< :
    static CcKernelSHARED CcConstString_H(Semicolon);       //!< ;
    static CcKernelSHARED CcConstString_H(Slash);           //!< /
    static CcKernelSHARED CcConstString_H(BackSlash);       //!< \ ;
    static CcKernelSHARED CcConstString_H(DoubleSlashes);   //!< //
    static CcKernelSHARED CcConstString_H(Line);            //!< -
    static CcKernelSHARED CcConstString_H(Underline);       //!< _
    static const CcKernelSHARED CcString& Space;            //!< " "
    static const CcKernelSHARED CcString& Path;             //!< /
    static const CcKernelSHARED CcString& IpV4;             //!< String: .
    static const CcKernelSHARED CcString& MacAddress;       //!< &#58;
    static const CcKernelSHARED CcString& MacAddressMinus;  //!< &#58;
    static CcKernelSHARED CcConstString_H(QuestionMark);    //!< ?
    static CcKernelSHARED CcConstString_H(Quote);           //!< "
    static CcKernelSHARED CcConstString_H(QuoteEscaped);    //!< \"
    static CcKernelSHARED CcConstString_H(Ampersand);       //!< &
    static CcKernelSHARED CcConstString_H(Equal);           //!< =
  };
  //! Brackets, often used to search for.
  struct Brackets
  {
    static CcKernelSHARED CcConstString_H(Left);        //!< (
    static CcKernelSHARED CcConstString_H(Right);       //!< )
    static CcKernelSHARED CcConstString_H(SquareLeft);  //!< [
    static CcKernelSHARED CcConstString_H(SquareRight); //!< ]
    static CcKernelSHARED CcConstString_H(CurlyLeft);   //!< {
    static CcKernelSHARED CcConstString_H(CurlyRight);  //!< }
  };
  //! Different types by name
  struct Types
  {
    //! Names of hash types
    struct Hash
    {
      static CcKernelSHARED CcConstString_H(Crc32);   //!< crc32
      static CcKernelSHARED CcConstString_H(Md5);     //!< md5
      static CcKernelSHARED CcConstString_H(Sha256);  //!< sha256
    };
  };
  //! Known different names
  struct Names
  {
    static CcKernelSHARED CcConstString_H(CcProcessThreadName); //!< CcProcessThread
  };
  //! Known and often used numbers.
  struct Numbers
  {
    static CcKernelSHARED CcConstString_H(i0);  //!<  0
    static CcKernelSHARED CcConstString_H(i1);  //!<  1
    static CcKernelSHARED CcConstString_H(i2);  //!<  2
    static CcKernelSHARED CcConstString_H(i3);  //!<  3
    static CcKernelSHARED CcConstString_H(i4);  //!<  4
    static CcKernelSHARED CcConstString_H(i5);  //!<  5
    static CcKernelSHARED CcConstString_H(i6);  //!<  6
    static CcKernelSHARED CcConstString_H(i7);  //!<  7
    static CcKernelSHARED CcConstString_H(i8);  //!<  8
    static CcKernelSHARED CcConstString_H(i9);  //!<  9
  };
  //! Common prefixes
  struct Prefixes
  {
    static CcKernelSHARED CcConstString_H(Lib); //!< lib
  };
  //! Common file extenstions
  struct Extensions
  {
    static CcKernelSHARED CcConstString_H(Bin); //!< bin
    static CcKernelSHARED CcConstString_H(Bmp); //!< bmp
    static CcKernelSHARED CcConstString_H(Gif); //!< gif
    static CcKernelSHARED CcConstString_H(Jpg); //!< jpg
    static CcKernelSHARED CcConstString_H(Png); //!< png
    static CcKernelSHARED CcConstString_H(Pbm); //!< pbm
    static CcKernelSHARED CcConstString_H(Pgm); //!< pgm
    static CcKernelSHARED CcConstString_H(Ppm); //!< ppm
    static CcKernelSHARED CcConstString_H(Dll); //!< dll
    static CcKernelSHARED CcConstString_H(So);  //!< so
    //! System specific extenstions.
    struct System
    {
      static const CcKernelSHARED CcString& DynamicLibraryWindows;  //!< Windows library name: dll
      static const CcKernelSHARED CcString& DynamicLibraryCommon;   //!< Common library name: so
      static const CcKernelSHARED CcString& DynamicLibrary;         //!< Name depending on current os see above
    };
  };
};
