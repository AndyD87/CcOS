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

CcConstStringClass_C(CcOS,"CcOS", CcGlobalStrings);
CcConstStringClass_C(Empty,"", CcGlobalStrings);
CcConstStringClass_C(Space," ", CcGlobalStrings);
CcConstStringClass_C(True,"true", CcGlobalStrings);
CcConstStringClass_C(False,"false", CcGlobalStrings);
CcConstStringClass_C(On,"on", CcGlobalStrings);
CcConstStringClass_C(Off,"off", CcGlobalStrings);
CcConstStringClass_C(Null,"null", CcGlobalStrings);
CcConstStringClass_C(Negative,"-", CcGlobalStrings);
CcConstStringClass_C(EolShort,"\n", CcGlobalStrings);
CcConstStringClass_C(EolLong,"\r\n", CcGlobalStrings);
CcConstStringClass_C(EolCr,"\r", CcGlobalStrings);
const CcString& CcGlobalStrings::Cr = EolCr;
#ifdef WINDOWS
  const CcString& CcGlobalStrings::EolOs = CcGlobalStrings::EolLong;
#else
  const CcString& CcGlobalStrings::EolOs = CcGlobalStrings::EolShort;
#endif

CcConstStringClass_C(AppNoIoBuffering,"CCOS_APPS_NO_IO_BUFFERING", CcGlobalStrings::EnvVars);

CcConstStringClass_C(DoubleDot,"..", CcGlobalStrings::Seperators);
CcConstStringClass_C(Dot,".", CcGlobalStrings::Seperators);
CcConstStringClass_C(Pipe,"|", CcGlobalStrings::Seperators);
CcConstStringClass_C(Comma,",", CcGlobalStrings::Seperators);
CcConstStringClass_C(Colon,":", CcGlobalStrings::Seperators);
CcConstStringClass_C(Semicolon,";", CcGlobalStrings::Seperators);
CcConstStringClass_C(Slash,"/", CcGlobalStrings::Seperators);
CcConstStringClass_C(DoubleSlashes,"//", CcGlobalStrings::Seperators);
CcConstStringClass_C(BackSlash,"\\", CcGlobalStrings::Seperators);
const CcString& CcGlobalStrings::Seperators::Space = CcGlobalStrings::Space;
const CcString& CcGlobalStrings::Seperators::Path = Slash;
const CcString& CcGlobalStrings::Seperators::IpV4 = Dot;
const CcString& CcGlobalStrings::Seperators::MacAddress = Colon;
CcConstStringClass_C(MacAddressMinus,"-", CcGlobalStrings::Seperators);
CcConstStringClass_C(QuestionMark,"?", CcGlobalStrings::Seperators);
CcConstStringClass_C(Quote,"\"", CcGlobalStrings::Seperators);
CcConstStringClass_C(QuoteEscaped,"\\\"", CcGlobalStrings::Seperators);
CcConstStringClass_C(Ampersand,"&", CcGlobalStrings::Seperators);
CcConstStringClass_C(Equal,"=", CcGlobalStrings::Seperators);

CcConstStringClass_C(Left,",", CcGlobalStrings::Brackets);
CcConstStringClass_C(Right,")", CcGlobalStrings::Brackets);
CcConstStringClass_C(SquareLeft,"[", CcGlobalStrings::Brackets);
CcConstStringClass_C(SquareRight,"]", CcGlobalStrings::Brackets);
CcConstStringClass_C(CurlyLeft,"{", CcGlobalStrings::Brackets);
CcConstStringClass_C(CurlyRight,"}", CcGlobalStrings::Brackets);

CcConstStringClass_C(Crc32,"crc32", CcGlobalStrings::Types::Hash);
CcConstStringClass_C(Md5,"md5", CcGlobalStrings::Types::Hash);
CcConstStringClass_C(Sha256,"sha256", CcGlobalStrings::Types::Hash);

CcConstStringClass_C(CcProcessThreadName,"CcProcessThread", CcGlobalStrings::Names);

CcConstStringClass_C(i0,"0", CcGlobalStrings::Numbers);
CcConstStringClass_C(i1,"1", CcGlobalStrings::Numbers);
CcConstStringClass_C(i2,"2", CcGlobalStrings::Numbers);
CcConstStringClass_C(i3,"3", CcGlobalStrings::Numbers);
CcConstStringClass_C(i4,"4", CcGlobalStrings::Numbers);
CcConstStringClass_C(i5,"5", CcGlobalStrings::Numbers);
CcConstStringClass_C(i6,"6", CcGlobalStrings::Numbers);
CcConstStringClass_C(i7,"7", CcGlobalStrings::Numbers);
CcConstStringClass_C(i8,"8", CcGlobalStrings::Numbers);
CcConstStringClass_C(i9,"9", CcGlobalStrings::Numbers);

CcConstStringClass_C(Lib,"lib", CcGlobalStrings::Prefixes);

CcConstStringClass_C(Bin,"bin", CcGlobalStrings::Extensions);
CcConstStringClass_C(Bmp,"bmp", CcGlobalStrings::Extensions);
CcConstStringClass_C(Gif,"gif", CcGlobalStrings::Extensions);
CcConstStringClass_C(Jpg,"jpg", CcGlobalStrings::Extensions);
CcConstStringClass_C(Png,"png", CcGlobalStrings::Extensions);
CcConstStringClass_C(Pbm,"pbm", CcGlobalStrings::Extensions);
CcConstStringClass_C(Pgm,"pgm", CcGlobalStrings::Extensions);
CcConstStringClass_C(Ppm,"ppm", CcGlobalStrings::Extensions);
CcConstStringClass_C(Dll,"dll", CcGlobalStrings::Extensions);
CcConstStringClass_C(So, "so", CcGlobalStrings::Extensions);

const CcString& CcGlobalStrings::Extensions::System::DynamicLibraryWindows = Dll;
const CcString& CcGlobalStrings::Extensions::System::DynamicLibraryCommon  = So;
#ifdef WINDOWS
  const CcString& CcGlobalStrings::Extensions::System::DynamicLibrary = CcGlobalStrings::Extensions::System::DynamicLibraryWindows;
#else
  const CcString& CcGlobalStrings::Extensions::System::DynamicLibrary = CcGlobalStrings::Extensions::System::DynamicLibraryCommon;
#endif

  void CcGlobalStrings::init()
  {
  }
