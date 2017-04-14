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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcHttpRequest
 */
#include "CcHttpRequest.h"
#include "CcHttpConstStrings.h"
#include "CcKernel.h"
#include "CcStringUtil.h"

CcHttpRequest::CcHttpRequest(const CcString& Parse)
{
  parse(Parse);
}

CcHttpRequest::CcHttpRequest(const EHttpRequestType eType)
{
  m_Header.RequestType = eType;
}


CcHttpRequest::CcHttpRequest()
{
  m_Header.RequestType    = EHttpRequestType::Get;
  m_Header.Accept         = "*/*";
  m_Header.AcceptCharset  = "utf-8";
  m_Header.AcceptEncoding = "text,deflate";
  m_Header.AcceptLanguage = "";
  m_Header.Authorization  = "";
  m_Header.CacheControl   = "";
  m_Header.Connection     = "";
  m_Header.Cookie         = "";
  m_Header.ContentLength  = "";
  m_Header.ContentType    = "text/html";
  m_Header.Host           = "";
  m_Header.Range          = "";
  m_Header.Referer        = "";
  m_Header.TransferEncoding = "";
  m_Header.UserAgent = "CcOS Http-Client";
}

CcHttpRequest::~CcHttpRequest( void )
{
}

CcString CcHttpRequest::getHeader(void)
{
  CcString Header;
  switch (m_Header.RequestType)
  {
    case EHttpRequestType::Get:
      Header = CcHttpConstStrings::Get;
      Header << " " << m_Header.Path << " HTTP/1.1";
      break;
    case EHttpRequestType::PostMultip:
    case EHttpRequestType::PostUrlEnc:
      Header = CcHttpConstStrings::Post;
      Header << " " << m_Header.Path << " HTTP/1.1";
      break;
    case EHttpRequestType::Head:
      Header = CcHttpConstStrings::Head;
      Header << " " + m_Header.Path << " HTTP/1.1";
      break;
  }
  Header << CcHttpConstStrings::EOL;
  if (m_Header.Host.length())
    Header << "Host: " << m_Header.Host << CcHttpConstStrings::EOL;
  if (m_Header.Authorization.length())
    Header << "Authorization: " << m_Header.Authorization << CcHttpConstStrings::EOL;
  else if (m_sUsername.length() || m_sPassword.length())
  {
    CcString sAuthorityString(CcStringUtil::encodeBase64(m_sUsername + ":" + m_sPassword));
    Header << "Authorization: Basic " << sAuthorityString << CcHttpConstStrings::EOL;
  }
  if(m_Header.Accept.length())
    Header << "Accept: " << m_Header.Accept << CcHttpConstStrings::EOL;
  if(m_Header.AcceptCharset.length())
    Header << "Accept-Charset: " << m_Header.AcceptCharset << CcHttpConstStrings::EOL;
  if (m_Header.AcceptEncoding.length())
    Header << "Accept-Encoding: " << m_Header.AcceptEncoding << CcHttpConstStrings::EOL;
  if (m_Header.AcceptLanguage.length())
    Header << "Accept-Language: " << m_Header.AcceptLanguage << CcHttpConstStrings::EOL;
  if (m_Header.CacheControl.length())
    Header << "Cach-Control: " << m_Header.CacheControl << CcHttpConstStrings::EOL;
  if (m_Header.Connection.length())
    Header << "Connection: " << m_Header.Connection << CcHttpConstStrings::EOL;
  if (m_Header.Cookie.length())
    Header << "Cookie: " << m_Header.Cookie << CcHttpConstStrings::EOL;
  if (m_Header.ContentLength.length())
    Header << "Content-Length: " << m_Header.ContentLength << CcHttpConstStrings::EOL;
  if (m_Header.ContentType.length())
    Header << "Content-Type: " << m_Header.ContentType << CcHttpConstStrings::EOL;
  if (m_Header.Range.length())
    Header << "Range: " + m_Header.Range + CcHttpConstStrings::EOL;
  if (m_Header.Referer.length())
    Header << "Referer: " << m_Header.Referer << CcHttpConstStrings::EOL;
  if (m_Header.TransferEncoding.length())
    Header << "TransferEncoding: " << m_Header.TransferEncoding << CcHttpConstStrings::EOL;
  if (m_Header.UserAgent.length())
    Header << "User-Agent: " << m_Header.UserAgent << CcHttpConstStrings::EOL;
  return Header;
}

void CcHttpRequest::parse(const CcString& Parse)
{
  CcStringList slLines(Parse.splitLines());
  for (size_t i = 0; i < slLines.size(); i++)
  {
    parseLine(slLines.at(i));
  }
}

void CcHttpRequest::setPath(const CcString& sPath)
{
  m_Header.Path = sPath;
}

void CcHttpRequest::parseLine(const CcString& Parse)
{
  size_t pos = Parse.find(":");
  if (pos < Parse.length())
  {
    CcString sArgument(Parse.substr(0, pos));
    CcString sValue = Parse.substr(pos + 1).trim();
    if (sArgument.compare("Accept", ESensitivity::CaseInsensitiv))
      m_Header.Accept = sValue;
    else if (sArgument.compare("Accept-Charset", ESensitivity::CaseInsensitiv))
      m_Header.AcceptCharset = sValue;
    else if (sArgument.compare("Accept-Encoding", ESensitivity::CaseInsensitiv))
      m_Header.AcceptEncoding = sValue;
    else if (sArgument.compare("Accept-Language", ESensitivity::CaseInsensitiv))
      m_Header.AcceptLanguage = sValue;
    else if (sArgument.compare("Authorization", ESensitivity::CaseInsensitiv))
      m_Header.Authorization = sValue;
    else if (sArgument.compare("Cache-Control", ESensitivity::CaseInsensitiv))
      m_Header.CacheControl = sValue;
    else if (sArgument.compare("Connection", ESensitivity::CaseInsensitiv))
      m_Header.Connection = sValue;
    else if (sArgument.compare("Cookie", ESensitivity::CaseInsensitiv))
      m_Header.Cookie = sValue;
    else if (sArgument.compare("Content-Length", ESensitivity::CaseInsensitiv))
      m_Header.ContentLength = sValue;
    else if (sArgument.compare("Content-Type", ESensitivity::CaseInsensitiv))
      m_Header.ContentType = sValue;
    else if (sArgument.compare("Host", ESensitivity::CaseInsensitiv))
      m_Header.Host = sValue;
    else if (sArgument.compare("Range", ESensitivity::CaseInsensitiv))
      m_Header.Range = sValue;
    else if (sArgument.compare("Referer", ESensitivity::CaseInsensitiv))
      m_Header.Referer = sValue;
    else if (sArgument.compare("Transfer-Encoding", ESensitivity::CaseInsensitiv))
      m_Header.TransferEncoding = sValue;
    else if (sArgument.compare("User-Agent", ESensitivity::CaseInsensitiv))
      m_Header.UserAgent = sValue;
  }
  else if (SIZE_MAX != Parse.find("HTTP"))
  {
    size_t posSpace = Parse.find(" ");
    if (posSpace != SIZE_MAX)
    {
      m_Header.HTTPMethod = Parse.substr(0, posSpace);
      m_Header.HTTPTarget = Parse.getStringBetween(" ", " ");
    }
  }
}


void CcHttpRequest::setHost(const CcString& Host)
{
  m_Header.Host = Host;
}
void CcHttpRequest::setUserAgent(const CcString& Agent)
{
  m_Header.UserAgent = Agent;
}

void CcHttpRequest::setContentType(const CcString& sContentType)
{
  m_Header.ContentType = sContentType;
}

void CcHttpRequest::setContentSize(size_t size)
{
  m_Header.ContentLength.clear();
  m_Header.ContentLength.appendNumber(size);
}

void CcHttpRequest::setMozillaAgent(void)
{
  m_Header.UserAgent = "Mozilla / 5.0 (Windows NT 10.0; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 47.0.2526.106 Safari / 537.36";
}
