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
 * @brief     Implementation of Class CcHttpRequest
 */
#include "CcHttpRequest.h"
#include "CcHttpWorkData.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"
#include "CcHttpDefaults.h"

CcHttpRequest::CcHttpRequest(const CcString& Parse)
{
  parse(Parse);
}

CcHttpRequest::CcHttpRequest(bool bInitValues)
{
  clear(bInitValues);
}

CcHttpRequest::~CcHttpRequest()
{
}

CcString CcHttpRequest::getHeader()
{
  CcString sHeader;
  for (CcString& sLine : m_oHeaderLines)
  {
    sHeader << sLine << CcHttpGlobalStrings::EOL;
  }
  addTransferEncoding();
  if (  m_pCookies &&
        m_pCookies->size() > 0)
  {
    sHeader << m_pCookies->getCookieLine() << CcHttpGlobalStrings::EOL;
  }
  sHeader << CcHttpGlobalStrings::EOL;
  return sHeader;
}

void CcHttpRequest::clear(bool bInitValues)
{
  m_oHeaderLines.clear();
  m_oTransferEncoding = CcHttpTransferEncoding::Chunked;
  if (bInitValues)
  {
    setAccept("*/*");
    setAcceptCharset(CcHttpDefaults::Charset);
    setAcceptEncoding("text,deflate");
    setAcceptLanguage("de,en-US;q=0.7,en;q=0.3");
    setConnection("keep-alive");
    setUserAgent(CcHttpDefaults::Agent);
  }
}

void CcHttpRequest::parse(const CcString& Parse)
{
  m_oHeaderLines = Parse.splitLines();
  if (m_oHeaderLines.size() > 0)
  {
    parseFirstLine(m_oHeaderLines[0]);
    m_oHeaderLines.remove(0);
    for (CcString& sLine : m_oHeaderLines)
    {
      parseLine(sLine);
    }
  }
}

void CcHttpRequest::appendHeaderLine(const CcString& sKey, const CcString& sValue)
{
  CcString sLine(sKey);
  sLine << CcHttpGlobalStrings::Header::Seperator << sValue;
  size_t uiPos = findKey(sKey);
  if (uiPos < m_oHeaderLines.size())
  {
    m_oHeaderLines[uiPos] = sLine;
  }
  else
  {
    m_oHeaderLines.append(sLine);
  }
}

CcString CcHttpRequest::getContentType()
{
  CcString sReturn;
  for (CcString& sLine : m_oHeaderLines)
  {
    if (sLine.length() >= CcHttpGlobalStrings::Header::ContentType.length() + 1 &&
      sLine.startsWith(CcHttpGlobalStrings::Header::ContentType))
    {
      sReturn = sLine.substr(CcHttpGlobalStrings::Header::ContentType.length() + 1).trim();
    }
  }
  return sReturn;
}

uint64 CcHttpRequest::getContentLength()
{
  CcString sReturn;
  for (CcString& sLine : m_oHeaderLines)
  {
    if (sLine.length() >= CcHttpGlobalStrings::Header::ContentLength.length() + 1 &&
      sLine.startsWith(CcHttpGlobalStrings::Header::ContentLength))
    {
      sReturn = sLine.substr(CcHttpGlobalStrings::Header::ContentLength.length() + 1).trim();
    }
  }
  return sReturn.toUint64();
}

size_t CcHttpRequest::findKey(const CcString& sKey)
{
  size_t uiPos = 0;
  for (const CcString& sLine : m_oHeaderLines)
  {
    if (sLine.startsWith(sKey, ESensitivity::CaseInsensitiv))
    {
      break;
    }
    uiPos++;
  }
  if (uiPos > m_oHeaderLines.size())
    uiPos = SIZE_MAX;
  return uiPos;
}

void CcHttpRequest::parseFirstLine(const CcString& Parse)
{
  if (SIZE_MAX != Parse.find("HTTP"))
  {
    size_t posSpace = Parse.find(CcGlobalStrings::Space);
    if (posSpace != SIZE_MAX)
    {
      CcString sMethod = Parse.substr(0, posSpace);
      if (sMethod.compareInsensitve(CcHttpGlobalStrings::Get))
        m_eRequestType = EHttpRequestType::Get;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Head))
        m_eRequestType = EHttpRequestType::Head;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Post))
        m_eRequestType = EHttpRequestType::Post;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Put))
        m_eRequestType = EHttpRequestType::Put;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::List))
        m_eRequestType = EHttpRequestType::List;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Delete))
        m_eRequestType = EHttpRequestType::Delete;
      else if (sMethod.compareInsensitve(CcHttpGlobalStrings::Patch))
        m_eRequestType = EHttpRequestType::Patch;
      else
        m_eRequestType = EHttpRequestType::Unknown;
      m_sPath = Parse.getStringBetween(CcGlobalStrings::Space, CcGlobalStrings::Space);
      m_oQuery = CcHttpWorkData::splitAndParseQueryLine(m_sPath);
    }
  }
}

void CcHttpRequest::parseLine(const CcString& Parse)
{
  size_t pos = Parse.find(CcGlobalStrings::Seperators::Colon);
  if (pos < Parse.length())
  {
    CcString sArgument(Parse.substr(0, pos));
    CcString sValue = Parse.substr(pos + 1).trim();
    if (sArgument.compare("Transfer-Encoding", ESensitivity::CaseInsensitiv))
      m_oTransferEncoding.parseLine(sValue);
    else if (sArgument.compare("Cookie", ESensitivity::CaseInsensitiv) && m_pCookies)
      m_pCookies->parseLine(sValue);
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
  appendHeaderLine(CcHttpGlobalStrings::Header::Accept, sAccept);
}

void CcHttpRequest::setAcceptCharset(const CcString& sAcceptCharset)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::AcceptCharset, sAcceptCharset);
}

void CcHttpRequest::setAcceptEncoding(const CcString& sAcceptEncoding)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::AcceptEncoding, sAcceptEncoding);
}

void CcHttpRequest::setAcceptLanguage(const CcString& sAcceptLanguage)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::AcceptLanguage, sAcceptLanguage);
}

void CcHttpRequest::setConnection(const CcString& sConnection)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::Connection, sConnection);
}

void CcHttpRequest::setHost(const CcString& sHost)
{
  if (m_oHeaderLines.size() > 0)
  {
    CcString sLine(CcHttpGlobalStrings::Header::Host);
    sLine << CcHttpGlobalStrings::Header::Seperator << sHost;
    if (m_oHeaderLines.contains(CcGlobalStrings::Seperators::Colon))
    {
      m_oHeaderLines.insert(0, sLine);
    }
    else
    {
      m_oHeaderLines.insert(1, sLine);
    }
  }
  else
  {
    appendHeaderLine(CcHttpGlobalStrings::Header::Host, sHost);
  }
}

void CcHttpRequest::setUserAgent(const CcString& sAgent)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::UserAgent, sAgent);
}

void CcHttpRequest::setContentEncoding(const CcString& sContentEncoding)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::ContentEncoding, sContentEncoding);
}

void CcHttpRequest::setContentType(const CcString& sContentType)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::ContentType, sContentType);
}

void CcHttpRequest::setContentLength(uint64 uiSize)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::ContentLength, CcString::fromNumber(uiSize));
}

void CcHttpRequest::setAuthorization(const CcString& sUsername, const CcString& sPassword)
{
  CcString sAuthorityString(CcStringUtil::encodeBase64(sUsername + CcGlobalStrings::Seperators::Colon + sPassword));
  CcString sLine("Basic ");
  sLine << sAuthorityString;
  setAuthorization(sLine);
}

void CcHttpRequest::setAuthorization(const CcString& sAuthorization)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::Authorization, sAuthorization);
}

void CcHttpRequest::setRequestType(EHttpRequestType eType, const CcString& sPath)
{
  CcString sLine;
  m_eRequestType = eType;
  switch (eType)
  {
    case EHttpRequestType::Get:
      sLine = CcHttpGlobalStrings::Get;
      sLine << CcGlobalStrings::Space << sPath << " HTTP/1.1";
      break;
    case EHttpRequestType::Post:
      sLine = CcHttpGlobalStrings::Post;
      sLine << CcGlobalStrings::Space << sPath << " HTTP/1.1";
      break;
    case EHttpRequestType::Head:
      sLine = CcHttpGlobalStrings::Head;
      sLine << CcGlobalStrings::Space + sPath << " HTTP/1.1";
      break;
    case EHttpRequestType::Put:
      sLine = CcHttpGlobalStrings::Put;
      sLine << CcGlobalStrings::Space + sPath << " HTTP/1.1";
      break;
    default:
      m_eRequestType = EHttpRequestType::Unknown;
  }
  m_oHeaderLines.insert(0, sLine);
}

void CcHttpRequest::appendContent(const void* pData, size_t uiLen)
{
  m_oContent.append(static_cast<const char*>(pData), uiLen);
}

void CcHttpRequest::setMozillaAgent()
{
  setUserAgent("Mozilla / 5.0 (Windows NT 10.0; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 47.0.2526.106 Safari / 537.36");
}
