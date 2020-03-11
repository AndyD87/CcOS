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
#include "CcGlobalStrings.h"

namespace CcGlobalStrings
{
  const CcString CcOS("CcOS");
  const CcString Empty("");
  const CcString Space(" ");
  const CcString True("true");
  const CcString False("false");
  const CcString On("on");
  const CcString Off("off");
  const CcString Null("null");
  const CcString Negative("-");
  const CcString EolShort("\n");
  const CcString EolLong("\r\n");
  const CcString EolCr("\r");
#ifdef WINDOWS
  const CcString& EolOs = EolLong;
#else
  const CcString& EolOs = EolShort;
#endif
  namespace EnvVars
  {
    const CcString AppNoIoBuffering("CCOS_APPS_NO_IO_BUFFERING");
  }
  namespace Seperators
  {
    const CcString Dot(".");
    const CcString Pipe("|");
    const CcString Comma(",");
    const CcString Colon(":");
    const CcString Semicolon(";");
    const CcString Slash("/");
    const CcString DoubleSlashes("//");
    const CcString BackSlash("\\");
    const CcString& Space = CcGlobalStrings::Space;
    const CcString& Path = Slash;
    const CcString& IpV4 = Dot;
    const CcString& MacAddress = Colon;
    const CcString MacAddressMinus("-");
    const CcString QuestionMark("?");
    const CcString Quote("\"");
    const CcString QuoteEscaped("\\\"");
    const CcString Ampersand("&");
    const CcString Equal("=");
  }
  namespace Brackets
  {
    const CcString Left("(");
    const CcString Right(")");
    const CcString SquareLeft("[");
    const CcString SquareRight("]");
    const CcString CurlyLeft("{");
    const CcString CurlyRight("}");
  }
  namespace Types
  {
    namespace Hash
    {
      const CcString Crc32("crc32");
      const CcString Md5("md5");
      const CcString Sha256("sha256");
    }
  }
  namespace Names
  {
    const CcString CcProcessThreadName("CcProcessThread");
  }  
  namespace Extensions
  {
    const CcString Bin("bin");
    const CcString Bmp("bmp");
    const CcString Gif("gif");
    const CcString Jpg("jpg");
    const CcString Png("png");
    const CcString Pbm("pbm");
    const CcString Pgm("pgm");
    const CcString Ppm("ppm");
  }
}

const CcString CcGlobalStrings::Numbers::i0("0");
const CcString CcGlobalStrings::Numbers::i1("1");
const CcString CcGlobalStrings::Numbers::i2("2");
const CcString CcGlobalStrings::Numbers::i3("3");
const CcString CcGlobalStrings::Numbers::i4("4");
const CcString CcGlobalStrings::Numbers::i5("5");
const CcString CcGlobalStrings::Numbers::i6("6");
const CcString CcGlobalStrings::Numbers::i7("7");
const CcString CcGlobalStrings::Numbers::i8("8");
const CcString CcGlobalStrings::Numbers::i9("9");
