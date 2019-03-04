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
 * @brief     Implementation of Class CcHttpRequest
 */
#include "CcHttpRequest.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"

CcHttpRequest::CcHttpRequest(const CcString& Parse)
{
  parse(Parse);
}

CcHttpRequest::CcHttpRequest():
  m_oTransferEncoding(CcHttpTransferEncoding::Chunked)
{
  setAccept("*/*");
  setAcceptCharset("utf-8");
  setAcceptEncoding("text,deflate");
  setAcceptLanguage("de,en-US;q=0.7,en;q=0.3");
  setConnection("keep-alive");
  setUserAgent("CcOS Http-Client");
}

CcHttpRequest::~CcHttpRequest(void )
{
}

CcString CcHttpRequest::getHeader(void)
{
  CcString sHeader;
  for (CcString& sLine : m_oHeaderLines)
  {
    sHeader << sLine << CcHttpGlobalStrings::EOL;
  }
  addTransferEncoding();
  sHeader << CcHttpGlobalStrings::EOL;
  return sHeader;
}

void CcHttpRequest::parse(const CcString& Parse)
{
  CcStringList slLines(Parse.splitLines());
  for (size_t i = 0; i < slLines.size(); i++)
  {
    parseLine(slLines.at(i));
  }
}

void CcHttpRequest::parseLine(const CcString& Parse)
{
  size_t pos = Parse.find(CcGlobalStrings::Seperators::Colon);
  if (pos < Parse.length())
  {
    CcString sArgument(Parse.substr(0, pos));
    CcString sValue = Parse.substr(pos + 1).trim();
  }
  else if (SIZE_MAX != Parse.find("HTTP"))
  {
    size_t posSpace = Parse.find(" ");
    if (posSpace != SIZE_MAX)
    {
      CcString sMethod = Parse.substr(0, posSpace);
      if (sMethod.compareInsensitve(CcHttpGlobalStrings::Get))
        m_eRequestType = EHttpRequestType::Get;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Head))
        m_eRequestType = EHttpRequestType::Head;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Post))
        m_eRequestType = EHttpRequestType::PostMultip;
      m_sPath = Parse.getStringBetween(" ", " ");
    }
  }
}

void CcHttpRequest::addTransferEncoding()
{
  if (m_oTransferEncoding.hasFlags())
  {
    m_oContent.appendString(m_oTransferEncoding.getLine());
    m_oContent.appendString(CcHttpGlobalStrings::EOL);
  }
}

void CcHttpRequest::setAccept(const CcString& sAccept)
{
  CcString sLine(CcHttpGlobalStrings::Header::Accept);
  sLine << ": " << sAccept;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setAcceptCharset(const CcString& sAcceptCharset)
{
  CcString sLine(CcHttpGlobalStrings::Header::AcceptCharset);
  sLine << ": " << sAcceptCharset;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setAcceptEncoding(const CcString& sAcceptEncoding)
{
  CcString sLine(CcHttpGlobalStrings::Header::AcceptEncoding);
  sLine << ": " << sAcceptEncoding;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setAcceptLanguage(const CcString& sAcceptLanguage)
{
  CcString sLine(CcHttpGlobalStrings::Header::AcceptLanguage);
  sLine << ": " << sAcceptLanguage;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setConnection(const CcString& sConnection)
{
  CcString sLine(CcHttpGlobalStrings::Header::Connection);
  sLine << ": " << sConnection;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setHost(const CcString& sHost)
{
  CcString sLine(CcHttpGlobalStrings::Header::Host);
  sLine << ": " << sHost;
  if (m_oHeaderLines.size() > 0)
    m_oHeaderLines.insert(1, sLine);
}

void CcHttpRequest::setUserAgent(const CcString& sAgent)
{
  CcString sLine(CcHttpGlobalStrings::Header::UserAgent);
  sLine << ": " << sAgent;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setContentEncoding(const CcString& sContentEncoding)
{
  CcString sLine(CcHttpGlobalStrings::Header::ContentEncoding);
  sLine << ": " << sContentEncoding;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setContentType(const CcString& sContentType)
{
  CcString sLine(CcHttpGlobalStrings::Header::ContentType);
  sLine << ": " << sContentType;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setContentLength(size_t size)
{
  CcString sLine(CcHttpGlobalStrings::Header::ContentLength);
  sLine << ": " << CcString::fromNumber(size);
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setAuthorization(const CcString& sUsername, const CcString& sPassword)
{
  CcString sAuthorityString(CcStringUtil::encodeBase64(sUsername + CcGlobalStrings::Seperators::Colon + sPassword));
  CcString sLine(CcHttpGlobalStrings::Header::Authorization);
  sLine << ": Basic " << sAuthorityString << CcHttpGlobalStrings::EOL;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setAuthorization(const CcString& sAuthorization)
{
  CcString sLine(CcHttpGlobalStrings::Header::Authorization);
  sLine << ": " << sAuthorization;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setRequestType(EHttpRequestType eType, const CcString& sPath)
{
  CcString sLine;
  m_eRequestType = eType;
  switch (eType)
  {
    case EHttpRequestType::Get:
      sLine = CcHttpGlobalStrings::Get;
      sLine << " " << sPath << " HTTP/1.1";
      break;
    case EHttpRequestType::PostMultip:
    case EHttpRequestType::PostUrlEnc:
      sLine = CcHttpGlobalStrings::Post;
      sLine << " " << sPath << " HTTP/1.1";
      break;
    case EHttpRequestType::Head:
      sLine = CcHttpGlobalStrings::Head;
      sLine << " " + sPath << " HTTP/1.1";
      break;
    default:
      m_eRequestType = EHttpRequestType::Unknown;
  }
  m_oHeaderLines.insert(0, sLine);
}

void CcHttpRequest::addLine(const CcString& sName, const CcString& sValue)
{
  CcString sLine = sName + ": " + sValue;
  m_oHeaderLines.append(sLine);
}

void CcHttpRequest::setMozillaAgent(void)
{
  setUserAgent("Mozilla / 5.0 (Windows NT 10.0; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 47.0.2526.106 Safari / 537.36");
}
