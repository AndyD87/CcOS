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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcHttpResponse
 */
#include "CcHttpResponse.h"
#include "CcHttpConstStrings.h"
#include "CcKernel.h"

CcHttpResponse::CcHttpResponse( const CcString& Parse )
{
  if (Parse.length())
    parse(Parse);
}

CcHttpResponse::CcHttpResponse(bool init)
{
  if (init)
  {
    m_Header.AcceptRanges      = "bytes";
    m_Header.Allow             = "GET, HEAD, POST";
    m_Header.CacheControl      = "";
    m_Header.Connection        = "close";
    m_Header.ContentEncoding   = "";
    m_Header.ContentLength     = "";
    m_Header.ContentLocation   = "";
    m_Header.ContentMd5        = "";
    m_Header.ContentRange      = "";
    m_Header.ContentType       = "";
    m_Header.Date              = "";
    m_Header.HTTP              = "HTTP/1.1 200 All Ok";
    m_Header.LastModified      = "";
    m_Header.Location          = "";
    m_Header.Refresh           = "";
    m_Header.Server            = "CcOS HttpServer";
    m_Header.SetCookie         = "";
    m_Header.TransferEncoding  = "";
  }
}

CcHttpResponse::~CcHttpResponse( void )
{
}

CcString CcHttpResponse::getHeader(void)
{
  CcString Header;
  if (m_Header.HTTP.length())
    Header << m_Header.HTTP << CcHttpConstStrings::EOL;
  if (m_Header.AcceptRanges.length())
    Header << "Accept-Ranges: " << m_Header.AcceptRanges << CcHttpConstStrings::EOL;
  if (m_Header.Allow.length())
    Header << "Allow: " << m_Header.Allow << CcHttpConstStrings::EOL;
  if (m_Header.CacheControl.length())
    Header << "Cache-Control: " << m_Header.CacheControl << CcHttpConstStrings::EOL;
  if (m_Header.Connection.length())
    Header << "Connection: " << m_Header.Connection << CcHttpConstStrings::EOL;
  if (m_Header.ContentEncoding.length())
    Header << "Content-Encoding: " << m_Header.ContentEncoding << CcHttpConstStrings::EOL;
  if (m_Header.ContentLength.length())
    Header << "Content-Length: " << m_Header.ContentLength << CcHttpConstStrings::EOL;
  if (m_Header.ContentLocation.length())
    Header << "Content-Location: " << m_Header.ContentLocation << CcHttpConstStrings::EOL;
  if (m_Header.ContentMd5.length())
    Header << "Content-MD5: " << m_Header.ContentMd5 << CcHttpConstStrings::EOL;
  if (m_Header.ContentRange.length())
    Header << "Content-Range: " << m_Header.ContentRange << CcHttpConstStrings::EOL;
  if (m_Header.ContentType.length())
    Header << "Content-Type: " << m_Header.ContentType << CcHttpConstStrings::EOL;
  if (m_Header.Date.length())
    Header << "Date: " << m_Header.Date << CcHttpConstStrings::EOL;
  if (m_Header.LastModified.length())
    Header << "Last-Modified: " << m_Header.LastModified << CcHttpConstStrings::EOL;
  if (m_Header.Location.length())
    Header << "Location: " << m_Header.Location << CcHttpConstStrings::EOL;
  if (m_Header.Refresh.length())
    Header << "Refresh: " << m_Header.Refresh << CcHttpConstStrings::EOL;
  if (m_Header.Server.length())
    Header << "Server: " << m_Header.Server << CcHttpConstStrings::EOL;
  if (m_Header.SetCookie.length())
    Header << "Set-Cookie: " << m_Header.SetCookie << CcHttpConstStrings::EOL;
  if (m_Header.TransferEncoding.length())
    Header << "Transfer-Encoding: " << m_Header.TransferEncoding << CcHttpConstStrings::EOL;
  Header << CcHttpConstStrings::EOL;
  return Header;
}

void CcHttpResponse::parse(const CcString& Parse)
{
  CcStringList slLines(Parse.splitLines());
  for (size_t i = 0; i < slLines.size(); i++)
  {
    parseLine(slLines.at(i));
  }
}

void CcHttpResponse::parseLine(const CcString& Parse)
{
  size_t pos = Parse.find(":");
  if (pos < Parse.length())
  {
    CcString sArgument(Parse.substr(0, pos));
    CcString sValue = Parse.substr(pos + 1).trim();
    if (sArgument.compare("Accept-Ranges", ESensitivity::CaseInsensitiv))
      m_Header.AcceptRanges = sValue;
    else if (sArgument.compare("Allow", ESensitivity::CaseInsensitiv))
      m_Header.Allow = sValue;
    else if (sArgument.compare("Cache-Control", ESensitivity::CaseInsensitiv))
      m_Header.CacheControl = sValue;
    else if (sArgument.compare("Connection", ESensitivity::CaseInsensitiv))
      m_Header.Connection = sValue;
    else if (sArgument.compare("Content-Encoding", ESensitivity::CaseInsensitiv))
      m_Header.ContentEncoding = sValue;
    else if (sArgument.compare("Content-Length", ESensitivity::CaseInsensitiv))
      m_Header.ContentLength = sValue;
    else if (sArgument.compare("Content-Location", ESensitivity::CaseInsensitiv))
      m_Header.ContentLocation = sValue;
    else if (sArgument.compare("Content-MD5", ESensitivity::CaseInsensitiv))
      m_Header.ContentMd5 = sValue;
    else if (sArgument.compare("Content-Range", ESensitivity::CaseInsensitiv))
      m_Header.ContentRange = sValue;
    else if (sArgument.compare("Content-Type", ESensitivity::CaseInsensitiv))
      m_Header.ContentType = sValue;
    else if (sArgument.compare("Date", ESensitivity::CaseInsensitiv))
      m_Header.Date = sValue;
    else if (sArgument.compare("Last-Modified", ESensitivity::CaseInsensitiv))
      m_Header.LastModified = sValue;
    else if (sArgument.compare("Location", ESensitivity::CaseInsensitiv))
      m_Header.Location = sValue;
    else if (sArgument.compare("Refresh", ESensitivity::CaseInsensitiv))
      m_Header.Refresh = sValue;
    else if (sArgument.compare("Server", ESensitivity::CaseInsensitiv))
      m_Header.Server = sValue;
    else if (sArgument.compare("Cookie", ESensitivity::CaseInsensitiv))
      m_Header.SetCookie = sValue;
    else if (sArgument.compare("Transfer-Encoding", ESensitivity::CaseInsensitiv))
      m_Header.TransferEncoding = sValue;
    else
      CcKernel::delayMs(1);
  }
  else if (Parse.startsWith("HTTP"))
  {
    m_Header.HTTP = Parse.getStringBetween(" ", " ");
  }
}
