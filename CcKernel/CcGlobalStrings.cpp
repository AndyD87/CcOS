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
  CcConstString_C(CcOS,"CcOS");
  CcConstString_C(Empty,"");
  CcConstString_C(Space," ");
  CcConstString_C(True,"true");
  CcConstString_C(False,"false");
  CcConstString_C(On,"on");
  CcConstString_C(Off,"off");
  CcConstString_C(Null,"null");
  CcConstString_C(Negative,"-");
  CcConstString_C(EolShort,"\n");
  CcConstString_C(EolLong,"\r\n");
  CcConstString_C(EolCr,"\r");
  const CcString& Cr = EolCr;
#ifdef WINDOWS
  const CcString& EolOs = EolLong;
#else
  const CcString& EolOs = EolShort;
#endif
  namespace EnvVars
  {
    CcConstString_C(AppNoIoBuffering,"CCOS_APPS_NO_IO_BUFFERING");
  }
  namespace Seperators
  {
    CcConstString_C(DoubleDot,"..");
    CcConstString_C(Dot,".");
    CcConstString_C(Pipe,"|");
    CcConstString_C(Comma,",");
    CcConstString_C(Colon,":");
    CcConstString_C(Semicolon,";");
    CcConstString_C(Slash,"/");
    CcConstString_C(DoubleSlashes,"//");
    CcConstString_C(BackSlash,"\\");
    const CcString& Space = CcGlobalStrings::Space;
    const CcString& Path = Slash;
    const CcString& IpV4 = Dot;
    const CcString& MacAddress = Colon;
    CcConstString_C(MacAddressMinus,"-");
    CcConstString_C(QuestionMark,"?");
    CcConstString_C(Quote,"\"");
    CcConstString_C(QuoteEscaped,"\\\"");
    CcConstString_C(Ampersand,"&");
    CcConstString_C(Equal,"=");
  }
  namespace Brackets
  {
    CcConstString_C(Left,",");
    CcConstString_C(Right,")");
    CcConstString_C(SquareLeft,"[");
    CcConstString_C(SquareRight,"]");
    CcConstString_C(CurlyLeft,"{");
    CcConstString_C(CurlyRight,"}");
  }
  namespace Types
  {
    namespace Hash
    {
      CcConstString_C(Crc32,"crc32");
      CcConstString_C(Md5,"md5");
      CcConstString_C(Sha256,"sha256");
    }
  }
  namespace Names
  {
    CcConstString_C(CcProcessThreadName,"CcProcessThread");
  }  
  namespace Numbers
  {
    CcConstString_C(i0,"0");
    CcConstString_C(i1,"1");
    CcConstString_C(i2,"2");
    CcConstString_C(i3,"3");
    CcConstString_C(i4,"4");
    CcConstString_C(i5,"5");
    CcConstString_C(i6,"6");
    CcConstString_C(i7,"7");
    CcConstString_C(i8,"8");
    CcConstString_C(i9,"9");
  }
  namespace Prefixes
  {
    CcConstString_C(Lib,"lib");
  }
  namespace Extensions
  {
    CcConstString_C(Bin,"bin");
    CcConstString_C(Bmp,"bmp");
    CcConstString_C(Gif,"gif");
    CcConstString_C(Jpg,"jpg");
    CcConstString_C(Png,"png");
    CcConstString_C(Pbm,"pbm");
    CcConstString_C(Pgm,"pgm");
    CcConstString_C(Ppm,"ppm");
    CcConstString_C(Dll,"dll");
    CcConstString_C(So, "so");
    namespace System
    {
      const CcString& DynamicLibraryWindows = Dll;
      const CcString& DynamicLibraryCommon  = So;
      #ifdef WINDOWS
        const CcString& DynamicLibrary = DynamicLibraryWindows;
      #else
        const CcString& DynamicLibrary = DynamicLibraryCommon;
      #endif
    }
  }
}
