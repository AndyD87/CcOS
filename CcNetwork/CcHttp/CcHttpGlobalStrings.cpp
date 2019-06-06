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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcHttpServer
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */

#include "CcHttpGlobalStrings.h"
#include "CcGlobalStrings.h"


const CcString& CcHttpGlobalStrings::EOL = CcGlobalStrings::EolLong;
const CcString CcHttpGlobalStrings::EOLSeperator("\r\n\r\n");
const CcString CcHttpGlobalStrings::Get("GET");
const CcString CcHttpGlobalStrings::Post("POST");
const CcString CcHttpGlobalStrings::Head("HEAD");
const CcString CcHttpGlobalStrings::Put("PUT");
const CcString CcHttpGlobalStrings::List("LIST");
const CcString CcHttpGlobalStrings::Delete("DELETE");
const CcString CcHttpGlobalStrings::Patch("PATCH");

const CcString CcHttpGlobalStrings::Header::Accept("Accept");
const CcString CcHttpGlobalStrings::Header::AcceptCharset("Accept-Charset");
const CcString CcHttpGlobalStrings::Header::AcceptEncoding("Accept-Encoding");
const CcString CcHttpGlobalStrings::Header::AcceptLanguage("Accept-Language");
const CcString CcHttpGlobalStrings::Header::AcceptRanges("Accept-Ranges");
const CcString CcHttpGlobalStrings::Header::Allow("Allow");
const CcString CcHttpGlobalStrings::Header::Authorization("Authorization");
const CcString CcHttpGlobalStrings::Header::CacheControl("Cache-Control");
const CcString CcHttpGlobalStrings::Header::Connection("Connection");
const CcString CcHttpGlobalStrings::Header::ContentEncoding("Content-Encoding");
const CcString CcHttpGlobalStrings::Header::ContentLength("Content-Length");
const CcString CcHttpGlobalStrings::Header::ContentLocation("Content-Location");
const CcString CcHttpGlobalStrings::Header::ContentMd5("Content-MD5");
const CcString CcHttpGlobalStrings::Header::ContentRange("Content-Range");
const CcString CcHttpGlobalStrings::Header::ContentType("Content-Type");
const CcString CcHttpGlobalStrings::Header::Date("Date");
const CcString CcHttpGlobalStrings::Header::Host("Host");
const CcString CcHttpGlobalStrings::Header::LastModified("Last-Modified");
const CcString CcHttpGlobalStrings::Header::Location("Location");
const CcString CcHttpGlobalStrings::Header::Refresh("Refresh");
const CcString CcHttpGlobalStrings::Header::Server("Server");
const CcString CcHttpGlobalStrings::Header::SetCookie("Set-Cookie");
const CcString CcHttpGlobalStrings::Header::TransferEncoding("");
const CcString CcHttpGlobalStrings::Header::UserAgent("User-Agent");
