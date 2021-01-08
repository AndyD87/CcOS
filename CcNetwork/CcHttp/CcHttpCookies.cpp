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
 * @brief     Implementation of Class CcHttpCookies
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */

#include "CcKernel.h"
#include "CcHttpCookies.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"
#include "CcHttpGlobalStrings.h"

void CcHttpCookies::parseLine(const CcString& sHeader)
{
  size_t uiPosition = 0;
  CCookie oNextCookie;
  CcString sValue = getNextValue(sHeader, uiPosition);
  while(sValue.length() > 0)
  {
    oNextCookie.parseValue(sValue);
    sValue = getNextValue(sHeader, uiPosition);
  }
  // Remove existing key
  remove(oNextCookie.sKey);
  m_oCookies.append(oNextCookie);
}

CcString CcHttpCookies::getCookieLine()
{
  CcString sLine = CcHttpGlobalStrings::Header::Cookie + CcHttpGlobalStrings::Header::Seperator;
  bool bFirst = true;
  for (CCookie& oCookie : m_oCookies)
  {
    if (bFirst == true)
      bFirst = false;
    else
      sLine << CcGlobalStrings::Seperators::Semicolon << CcGlobalStrings::Seperators::Space;
    sLine << oCookie.sKey << CcGlobalStrings::Seperators::Equal << oCookie.sValue;
  }
  return sLine;
}

CcString CcHttpCookies::getSetCookieLine()
{
  CcString sLine;
  for (CCookie& oCookie : m_oCookies)
  {
    sLine << CcHttpGlobalStrings::Header::SetCookie << CcHttpGlobalStrings::Header::Seperator  << oCookie.sKey << CcGlobalStrings::Seperators::Equal << oCookie.sValue << CcHttpGlobalStrings::EOL;
  }
  return sLine;
}

bool CcHttpCookies::exists(const CcString& sKey)
{
  bool bExists = false;
  for (CCookie& oCookie : m_oCookies)
  {
    if (oCookie.sKey == sKey)
    {
      bExists = true;
      break;
    }
  }
  return bExists;
}

bool CcHttpCookies::remove(const CcString& sKey)
{
  bool bExists = false;
  size_t uiPos = 0;
  for (CCookie& oCookie : m_oCookies)
  {
    if (oCookie.sKey == sKey)
    {
      m_oCookies.remove(uiPos);
      break;
    }
    uiPos++;
  }
  return bExists;
}

CcString CcHttpCookies::getNextValue(const CcString& sHeader, size_t& uiPosition)
{
  CcString sValue;
  size_t uiStartPosition = uiPosition;
  // Find position of next semicolon, ignore quoted strings
  while (sHeader[uiPosition] != CcGlobalStrings::Seperators::Semicolon[0] &&
         sHeader.size() > uiPosition)
  {
    if (sHeader[uiPosition] == CcGlobalStrings::Seperators::Quote[0])
    {
      while (sHeader[uiPosition] != CcGlobalStrings::Seperators::Quote[0])
        uiPosition++;
    }
    uiPosition++;
  }
  sValue = sHeader.substr(uiStartPosition, uiPosition - uiStartPosition);
  if (uiPosition < sHeader.length())
    uiPosition++;
  return sValue;
}

size_t CcHttpCookies::CCookie::findEqualSign(const CcString& sValue)
{
  size_t uiPosition = 0;
  // Find position of next semicolon, ignore quoted strings
  while (sValue[uiPosition] != CcGlobalStrings::Seperators::Equal[0] &&
         sValue.size() > uiPosition)
  {
    if (sValue[uiPosition] == CcGlobalStrings::Seperators::Quote[0])
    {
      while (sValue[uiPosition] != CcGlobalStrings::Seperators::Quote[0])
        uiPosition++;
    }
    uiPosition++;
  }
  return uiPosition;
}

void CcHttpCookies::CCookie::parseValue(const CcString& sCookie)
{
  size_t uiEqualSign = findEqualSign(sCookie);
  if (uiEqualSign > 0 &&
      uiEqualSign < sCookie.length())
  {
    sKey = sCookie.substr(0, uiEqualSign).trim();
    sValue = sCookie.substr(uiEqualSign + 1).trim();
  }
}