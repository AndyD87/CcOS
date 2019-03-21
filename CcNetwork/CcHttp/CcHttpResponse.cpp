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
 * @brief     Implementation of Class CcHttpResponse
 */
#include "CcHttpResponse.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"

CcHttpResponse::CcHttpResponse( const CcString& Parse )
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
    setServer("CcOS HttpServer");
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
  addTransferEncoding();
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

CcString CcHttpResponse::getContentType()
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
  CcString sLine(CcHttpGlobalStrings::Header::AcceptRanges);
  sLine << ": " << sRange;
  m_oHeaderLines.append(sLine);
}

void CcHttpResponse::setAllow(const CcString& sAllowed)
{
  CcString sLine(CcHttpGlobalStrings::Header::Allow);
  sLine << ": " << sAllowed;
  m_oHeaderLines.append(sLine);
}

void CcHttpResponse::setConnection(const CcString& sConnection)
{
  CcString sLine(CcHttpGlobalStrings::Header::Connection);
  sLine << ": " << sConnection;
  m_oHeaderLines.append(sLine);
}

void CcHttpResponse::setContentType(const CcString& sContentType)
{
  CcString sLine(CcHttpGlobalStrings::Header::ContentType);
  sLine << ": " << sContentType;
  m_oHeaderLines.append(sLine);
}

void CcHttpResponse::setHttp(const CcString& sHttpsState)
{
  m_oHeaderLines.insert(0, sHttpsState);
}

void CcHttpResponse::setServer(const CcString& sServer)
{
  CcString sLine(CcHttpGlobalStrings::Header::Server);
  sLine << ": " << sServer;
  m_oHeaderLines.append(sLine);
}

void CcHttpResponse::parseLine(const CcString& Parse)
{
  size_t pos = Parse.find(CcGlobalStrings::Seperators::Colon);
  if (pos < Parse.length())
  {
    CcString sArgument(Parse.substr(0, pos));
    CcString sValue = Parse.substr(pos + 1).trim();
    if (sArgument.compare("Transfer-Encoding", ESensitivity::CaseInsensitiv))
      m_oTransferEncoding.parseLine(sValue);
  }
  else if (Parse.startsWith("HTTP"))
  {
    m_uiHttpCode = Parse.getStringBetween(" ", " ").toUint16();
  }
}


void CcHttpResponse::addTransferEncoding()
{
  if (m_oTransferEncoding.hasFlags())
  {
    m_oContent.append(m_oTransferEncoding.getLine());
    m_oContent.append(CcHttpGlobalStrings::EOL);
  }
}
