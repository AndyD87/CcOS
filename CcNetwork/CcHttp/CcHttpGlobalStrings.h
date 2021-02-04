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
 * @brief     Class CcHttpGlobalStrings
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"

/**
 * @brief Namespace to sort all global strings in CcHttp
 */
namespace CcHttpGlobalStrings
{
  extern const CcHttpSHARED CcString& EOL;          //!< String for \\r\\n
  extern const CcHttpSHARED CcString EOLSeperator;  //!< String for \\r\\n\\r\\n
  extern const CcHttpSHARED CcString Get;           //!< String for GET
  extern const CcHttpSHARED CcString Post;          //!< String for POST
  extern const CcHttpSHARED CcString Head;          //!< String for HEAD
  extern const CcHttpSHARED CcString Put;           //!< String for PUT
  extern const CcHttpSHARED CcString List;          //!< String for LIST
  extern const CcHttpSHARED CcString Delete;        //!< String for DELETE
  extern const CcHttpSHARED CcString Patch;         //!< String for PATCH
  namespace Header
  {
    extern const CcHttpSHARED CcString Accept;            //!< Head field Accept
    extern const CcHttpSHARED CcString AcceptCharset;     //!< Head field Accept-Charset
    extern const CcHttpSHARED CcString AcceptEncoding;    //!< Head field Accept-Encoding
    extern const CcHttpSHARED CcString AcceptLanguage;    //!< Head field Accept-Language
    extern const CcHttpSHARED CcString AcceptRanges;      //!< Head field Accept-Ranges
    extern const CcHttpSHARED CcString Allow;             //!< Head field Allow
    extern const CcHttpSHARED CcString Authorization;     //!< Head field Authorization
    extern const CcHttpSHARED CcString CacheControl;      //!< Head field Cache-Control
    extern const CcHttpSHARED CcString Connection;        //!< Head field Connection
    extern const CcHttpSHARED CcString Cookie;            //!< Head field Cookie
    extern const CcHttpSHARED CcString ContentEncoding;   //!< Head field Content-Encoding
    extern const CcHttpSHARED CcString ContentLength;     //!< Head field Content-Length
    extern const CcHttpSHARED CcString ContentLocation;   //!< Head field Content-Location
    extern const CcHttpSHARED CcString ContentMd5;        //!< Head field Content-Md5
    extern const CcHttpSHARED CcString ContentRange;      //!< Head field Content-Range
    extern const CcHttpSHARED CcString ContentType;       //!< Head field Content-Type
    extern const CcHttpSHARED CcString Date;              //!< Head field Date
    extern const CcHttpSHARED CcString Host;              //!< Head field Host
    extern const CcHttpSHARED CcString LastModified;      //!< Head field Last-Modified
    extern const CcHttpSHARED CcString Location;          //!< Head field Location
    extern const CcHttpSHARED CcString Refresh;           //!< Head field Refresh
    extern const CcHttpSHARED CcString Seperator;         //!< Head field Seperator
    extern const CcHttpSHARED CcString Server;            //!< Head field Server
    extern const CcHttpSHARED CcString SetCookie;         //!< Head field Set-Cookie
    extern const CcHttpSHARED CcString TransferEncoding;  //!< Head field Transfer-Encoding
    extern const CcHttpSHARED CcString UserAgent;         //!< Head field User-Agent
    extern const CcHttpSHARED CcString WwwAuthenticate;   //!< Head field Www-Authenticate
  }
  namespace MimeTypes
  {
    extern const CcHttpSHARED CcString ApplicationOctet;  //!< Applicatin/Octet
    extern const CcHttpSHARED CcString TextHtml;          //!< text/html
    extern const CcHttpSHARED CcString TextCss;           //!< text/css
    extern const CcHttpSHARED CcString TextJavascript;    //!< text/javascript
  }
}
