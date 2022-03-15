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
 * @brief     Implementation of Class CcHttpResponse
 */
#include "CcHttpResponse.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcHttpDefaults.h"

CcHttpResponse::CcHttpResponse(const CcString& Parse) :
  m_sContentType()
{
  if (Parse.length())
    parse(Parse);
}

CcHttpResponse::CcHttpResponse(bool init)
{
  if (init)
  {
    setAllow("GET, HEAD, POST");
    setConnection("close");
    setHttp("HTTP/1.1 200 All Ok");
    setServer(CcHttpDefaults::ServerName);
  }
}

CcHttpResponse::~CcHttpResponse()
{
}

CcString CcHttpResponse::getHeader()
{
  CcString sHeader;
  for (CcString& sLine : m_oHeaderLines)
  {
    sHeader << sLine << CcHttpGlobalStrings::EOL;
  }
  addTransferEncoding(sHeader);
  sHeader << CcHttpGlobalStrings::Header::ContentType << CcHttpGlobalStrings::Header::Seperator;
  if(m_sContentType.length() > 0)
  {
    sHeader << m_sContentType << CcHttpGlobalStrings::EOL;
  }
  else
  {
    sHeader << CcHttpGlobalStrings::MimeTypes::TextHtml << CcHttpGlobalStrings::EOL;
  }
  if (m_pCookies &&
      m_pCookies->size() > 0)
  {
    sHeader << m_pCookies->getSetCookieLine() << CcHttpGlobalStrings::EOL;
  }
  sHeader << CcHttpGlobalStrings::EOL;
  return sHeader;
}

void CcHttpResponse::parse(const CcString& Parse)
{
  m_oHeaderLines = Parse.splitLines(false);
  for (CcString& sLine : m_oHeaderLines)
  {
    parseLine(sLine);
  }
}

void CcHttpResponse::appendHeaderLine(const CcString& sKey, const CcString& sValue)
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

uint64 CcHttpResponse::getContentLength()
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

void CcHttpResponse::setAcceptRanges(const CcString& sRange)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::AcceptRanges, sRange);
}

void CcHttpResponse::setAllow(const CcString& sAllowed)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::Allow, sAllowed);
}

void CcHttpResponse::setConnection(const CcString& sConnection)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::Connection, sConnection);
}

void CcHttpResponse::setHttp(const CcString& sHttpsState)
{
  m_oHeaderLines.insert(0, sHttpsState);
}

void CcHttpResponse::setServer(const CcString& sServer)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::Server, sServer);
}

void CcHttpResponse::setWwwAuthenticate(const CcString& sType)
{
  appendHeaderLine(CcHttpGlobalStrings::Header::WwwAuthenticate, sType);
}

void CcHttpResponse::setError(CcHttpGlobals::EError eError)
{
  switch (eError)
  {
    case CcHttpGlobals::EError::Ok:
      setHttp("HTTP/1.1 200 All Ok");
      break;
    case CcHttpGlobals::EError::Error:
      setHttp("HTTP/1.1 400 Bad Request");
      break;
    case CcHttpGlobals::EError::ErrorNotFound:
      setHttp("HTTP/1.1 404 Not Found");
      break;
    case CcHttpGlobals::EError::ErrorUnauthorized:
      setHttp("HTTP/1.1 401 Unauthorized");
      break;
    case CcHttpGlobals::EError::ErrorMethodNotAllowed:
      setHttp("HTTP/1.1 405 Method Not Allowed");
      break;
    case CcHttpGlobals::EError::ServerError:
      CCFALLTHROUGH;
    default:
      setHttp("HTTP/1.1 500 Internal Server Error");
  }
}

size_t CcHttpResponse::findKey(const CcString& sKey)
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

void CcHttpResponse::parseLine(const CcString& Parse)
{
  size_t pos = Parse.find(CcGlobalStrings::Seperators::Colon);
  if (pos < Parse.length())
  {
    CcString sArgument(Parse.substr(0, pos));
    CcString sValue = Parse.substr(pos + 1).trim();
    if (sArgument.compare(CcHttpGlobalStrings::Header::TransferEncoding, ESensitivity::CaseInsensitiv))
      m_oTransferEncoding.parseLine(sValue);
    else if (sArgument.compare(CcHttpGlobalStrings::Header::ContentLength, ESensitivity::CaseInsensitiv) && m_pCookies)
    {
      m_oTransferEncoding.setFlag(CcHttpTransferEncoding::Normal);
    }
    else if (sArgument.compare(CcHttpGlobalStrings::Header::SetCookie, ESensitivity::CaseInsensitiv) && m_pCookies)
      m_pCookies->parseLine(sValue);
  }
  else if (Parse.startsWith("HTTP"))
  {
    m_uiHttpCode = Parse.getStringBetween(CcGlobalStrings::Space, CcGlobalStrings::Space).toUint16();
  }
}


void CcHttpResponse::addTransferEncoding(CcString& sHeader)
{
  if (m_oTransferEncoding.hasFlags())
  {
    sHeader.append(m_oTransferEncoding.getLine());
    sHeader.append(CcHttpGlobalStrings::EOL);
  }
}
