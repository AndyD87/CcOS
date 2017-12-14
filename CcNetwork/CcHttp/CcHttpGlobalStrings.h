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
 * @page      CcHttp
 * @subpage   CcHttpGlobalStrings
 *
 * @page      CcHttpGlobalStrings
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpGlobalStrings
 */
#ifndef CcHttpGlobalStrings_H_
#define CcHttpGlobalStrings_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"

/**
 * @brief Namespace to sort all global strings in CcHttp
 */
namespace CcHttpGlobalStrings
{
  extern const CcHttpSHARED CcString& EOL;
  extern const CcHttpSHARED CcString EOLSeperator;
  extern const CcHttpSHARED CcString Get;
  extern const CcHttpSHARED CcString Post;
  extern const CcHttpSHARED CcString Head;
  namespace Header
  {
    extern const CcHttpSHARED CcString Accept;
    extern const CcHttpSHARED CcString AcceptCharset;
    extern const CcHttpSHARED CcString AcceptEncoding;
    extern const CcHttpSHARED CcString AcceptLanguage;
    extern const CcHttpSHARED CcString AcceptRanges;
    extern const CcHttpSHARED CcString Allow;
    extern const CcHttpSHARED CcString Authorization;
    extern const CcHttpSHARED CcString CacheControl;
    extern const CcHttpSHARED CcString Connection;
    extern const CcHttpSHARED CcString ContentEncoding;
    extern const CcHttpSHARED CcString ContentLength;
    extern const CcHttpSHARED CcString ContentLocation;
    extern const CcHttpSHARED CcString ContentMd5;
    extern const CcHttpSHARED CcString ContentRange;
    extern const CcHttpSHARED CcString ContentType;
    extern const CcHttpSHARED CcString Date;
    extern const CcHttpSHARED CcString Host;
    extern const CcHttpSHARED CcString LastModified;
    extern const CcHttpSHARED CcString Location;
    extern const CcHttpSHARED CcString Refresh;
    extern const CcHttpSHARED CcString Server;
    extern const CcHttpSHARED CcString SetCookie;
    extern const CcHttpSHARED CcString TransferEncoding;
    extern const CcHttpSHARED CcString UserAgent;

  }
}

#endif /* CcHttpGlobalStrings_H_ */
