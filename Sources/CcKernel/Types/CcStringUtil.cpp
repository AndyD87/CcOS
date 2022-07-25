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
 * @brief     Implementation of Class CcStringUtil
 */
#include "CcStringUtil.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcByteArray.h"
#include "CcGlobalStrings.h"
#include "CcStatic.h"
#include "CcDateTime.h"
#include "CcMapCommon.h"
#ifndef GENERIC
  #include <math.h>
#endif

static const uint8 s_uiBase64Divider = 3;       //!< Base devider for base 64 converter
static const uint8 s_uiBase64OutputDivider = 4; //!< Base devider for base 64 converter output

size_t CcStringUtil::strlen(const char* pcString, size_t uiMaxLen)
{
  size_t uiRet = SIZE_MAX;
  for (size_t i=0; i < uiMaxLen; i++)
  {
    if (pcString[i] == 0)
    {
      uiRet = i;
      break;
    }
  }
  return uiRet;
}

int CcStringUtil::strcmp(const char* pcString1, const char* pcString2, size_t uiLen)
{
  int iRet = 0;
  size_t i = 0;
  while( iRet == 0 && i < uiLen )
  {
    if(pcString1[i] == 0 && pcString2[i] == 0)
    {
      break;
    }
    else if(pcString1[i] != pcString2[i])
    {
      iRet = pcString1[i] - pcString2[i];
      break;
    }
    i++;
  }
  return iRet;
}

char* CcStringUtil::strchr(char* pcString, char cToFind)
{
  char* pcCurrent = pcString;
  while(*pcCurrent != 0)
  {
    if(*pcCurrent == cToFind)
      return pcCurrent;
    pcCurrent++;
  }
  return nullptr;
}

size_t CcStringUtil::findChar(const wchar_t* pcString, wchar_t cToFind, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    if(pcString[i] == '\0' && cToFind != '\0')
      return SIZE_MAX;
    if (pcString[i] == cToFind)
      return i;
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findChar(const char* pcString, char cToFind, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    if(pcString[i] == '\0' && cToFind != '\0')
      return SIZE_MAX;
    if (pcString[i] == cToFind)
      return i;
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findCharEscaped(const char* pcString, char cToFind, char cEscape, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    if(pcString[i] == '\0' && cToFind != '\0')
      return SIZE_MAX;
    else if (pcString[i] == cEscape)
      i++;
    else if (pcString[i] == cToFind)
      return i;
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findCharEscapedSkipQuotes(const char* pcString, char cToFind, char cEscape, char cQoute, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    if (pcString[i] == '\0' && cToFind != '\0')
      return SIZE_MAX;
    else if (pcString[i] == cEscape)
      i++;
    else if (pcString[i] == cQoute)
      for (i++; i < uiLength + uiOffset; i++)
      {
        if (pcString[i] == '\0' && cToFind != '\0')
          return SIZE_MAX;
        else if (pcString[i] == cEscape)
          i++;
        else if (pcString[i] == cQoute)
          break;
      }
    else if (pcString[i] == cToFind)
      return i;
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findCharOf(const char* pcString, size_t uiLength, const char* pcToFind, size_t uiToFindSize, char& cFound)
{
  for (size_t i = 0; i < uiLength; i++)
  {
    for (size_t j = 0; j < uiToFindSize; j++)
    {
      if (pcString[i] == pcToFind[j])
      {
        cFound = pcToFind[j];
        return i;
      }
    }
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findLastChar(const char* pcString, size_t uiLength, char cToFind)
{
  size_t uiPos = SIZE_MAX;
  for (size_t i = 0; i < uiLength; i++)
  {
    if (pcString[i] == cToFind || pcString[i] == '\0')
      uiPos = i;
  }
  return uiPos;
}

size_t CcStringUtil::findNextWhiteSpace(const char* pcString, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    switch (pcString[i])
    {
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case '\f':
      case '\v':
        return i;
      case '\0':
        return SIZE_MAX;
    }
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findNextNotWhiteSpace(const char* pcString, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    switch (pcString[i])
    {
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case '\f':
      case '\v':
        continue;
      case '\0':
        return SIZE_MAX;
      default:
        return i;
    }
  }
  return SIZE_MAX;
}

bool CcStringUtil::extractKeyValue(const CcString& sString, CcString& sName, CcString& sValue, char cSeperator, char cEscapeChar)
{
  bool bSuccess = false;
  size_t uiOffset = sString.posNextNotWhitespace();
  if(uiOffset < sString.length())
  {
    size_t uiPosEqual = sString.find(cSeperator, uiOffset);
    if(uiPosEqual < sString.length())
    {
      bSuccess = true;
      sName = sString.substr(uiOffset, uiPosEqual-uiOffset).trim();
      // Start next search behind equal sign
      uiPosEqual++;

      size_t uiVarBegin = sString.posNextNotWhitespace(uiPosEqual);
      if(uiVarBegin < sString.length())
      {
        size_t uiVarEnd = SIZE_MAX;
        if (sString[uiVarBegin] == '"')
        {
          uiVarBegin++;
          uiVarEnd = CcStringUtil::findCharEscaped(sString, '"', cEscapeChar, uiVarBegin);
        }
        else if (sString[uiVarBegin] == '\'')
        {
          uiVarBegin++;
          uiVarEnd = CcStringUtil::findCharEscaped(sString, '\'', cEscapeChar, uiVarBegin);
        }
        // @todo implement failed html code with opening tag <
        else
        {
          size_t uiPosEnd = uiVarBegin;
          do
          {
            uiPosEnd = sString.posNextNotWhitespace(uiPosEnd);
            if(uiPosEnd < sString.length())
            {
              uiPosEnd++;
              uiVarEnd = uiPosEnd;
            }
          } while(uiPosEnd < sString.length());
        }
        if(uiVarEnd <= sString.length())
        {
          sValue = sString.substr(uiVarBegin, uiVarEnd-uiVarBegin).trim();
        }
        else
        {
          sValue = CcGlobalStrings::Empty;
        }
      }
      else
      {
        sValue = CcGlobalStrings::Empty;
      }
    }
  }
  return bSuccess;
}

bool CcStringUtil::isWhiteSpace(const char toTest)
{
  bool bRet = false;
  if (toTest == ' ' ||
    toTest == '\r' ||
    toTest == '\n' ||
    toTest == '\t'
    )
    bRet = true;
  return bRet;
}

bool CcStringUtil::isNumber(const char toTest)
{
  bool bRet = false;
  if (toTest >= '0' && toTest <= '9')
    bRet = true;
  return bRet;
}

size_t CcStringUtil::strlen(const wchar_t* pcString, size_t uiMaxLen)
{
  size_t uiRet = SIZE_MAX;
  for (size_t i = 0; i < uiMaxLen; i++)
  {
    if (pcString[i] == 0)
    {
      uiRet = i;
      break;
    }
  }
  return uiRet;
}

int CcStringUtil::strcmp(const wchar_t* pcString1, const wchar_t* pcString2, size_t uiLen)
{
  int iRet = 0;
  size_t i = 0;
  while (iRet == 0 && i < uiLen)
  {
    if (pcString1[i] == 0 && pcString2[i] == 0)
    {
      break;
    }
    else if (pcString1[i] < pcString2[i])
    {
      iRet = -static_cast<int>(i);
    }
    else if (pcString1[i] > pcString2[i])
    {
      iRet = static_cast<int>(i);
    }
    i++;
  }
  return iRet;
}

wchar_t* CcStringUtil::strchr(wchar_t* pcString, wchar_t cToFind)
{
  wchar_t* pcCurrent = pcString;
  while (*pcCurrent != 0)
  {
    if (*pcCurrent == cToFind)
      return pcCurrent;
    pcCurrent++;
  }
  return nullptr;
}

CcString& CcStringUtil::stripQuotes(CcString& sString, char cQuote)
{
  if (sString.size() > 1 &&
      sString[0] == cQuote &&
      sString.last() == cQuote)
  {
    sString = sString.substr(1, sString.length() - 2);
  }
  return sString;
}

size_t CcStringUtil::findCharEscaped(const wchar_t* pcString, wchar_t cToFind, wchar_t cEscape, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    if(pcString[i] == '\0' && cToFind != '\0')
      return SIZE_MAX;
    else if (pcString[i] == cEscape)
      i++;
    else if (pcString[i] == cToFind)
      return i;
  }
  return uiLength;
}

size_t CcStringUtil::findCharOf(const wchar_t* pcString, size_t uiLength, const wchar_t* pcToFind, size_t uiToFindSize, wchar_t& cFound)
{
  for (size_t i = 0; i < uiLength; i++)
  {
    for (size_t j = 0; j < uiToFindSize; j++)
    {
      if (pcString[i] == pcToFind[j])
      {
        cFound = pcToFind[j];
        return i;
      }
    }
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findNextWhiteSpace(const wchar_t* pcString, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    switch (pcString[i])
    {
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case '\f':
      case '\v':
        return i;
      case '\0':
        return SIZE_MAX;
    }
  }
  return SIZE_MAX;
}

size_t CcStringUtil::findNextNotWhiteSpace(const wchar_t* pcString, size_t uiOffset, size_t uiLength)
{
  for (size_t i = uiOffset; i < uiLength + uiOffset; i++)
  {
    switch (pcString[i])
    {
      case ' ':
      case '\r':
      case '\n':
      case '\t':
      case '\f':
      case '\b':
      case '\v':
        continue;
      case '\0':
        return SIZE_MAX;
      default:
        return i;
    }
  }
  return SIZE_MAX;
}

bool CcStringUtil::isWhiteSpace(const wchar_t toTest)
{
  bool bRet = false;
  if (toTest == ' ' ||
    toTest == '\r' ||
    toTest == '\n' ||
    toTest == '\t' ||
    toTest == '\f' ||
    toTest == '\b' ||
    toTest == '\v'
    )
    bRet = true;
  return bRet;
}

bool CcStringUtil::getBoolFromStirng(const CcString& sToParse, bool* pbOk)
{
  bool bRet = false;
  CcString sTrimmed = sToParse.getTrim();
  if (sTrimmed.compare(CcGlobalStrings::True, ESensitivity::CaseInsensitiv))
  {
    bRet = true;
    if (pbOk != nullptr)
    {
      *pbOk = true;
    }
  }
  else if (sTrimmed.compare(CcGlobalStrings::False, ESensitivity::CaseInsensitiv))
  {
    bRet = false;
    if (pbOk != nullptr)
    {
      *pbOk = true;
    }
  }
  if (sTrimmed.compare(CcGlobalStrings::On, ESensitivity::CaseInsensitiv))
  {
    bRet = true;
    if (pbOk != nullptr)
    {
      *pbOk = true;
    }
  }
  else if (sTrimmed.compare(CcGlobalStrings::Off, ESensitivity::CaseInsensitiv))
  {
    bRet = false;
    if (pbOk != nullptr)
    {
      *pbOk = true;
    }
  }
  else
  {
    bool bOkTemp;
    if (sTrimmed.toInt64(&bOkTemp) > 0 && bOkTemp)
      bRet = true;
    if (pbOk != nullptr)
    {
      *pbOk = bOkTemp;
    }
  }
  return bRet;
}

bool CcStringUtil::getBoolFromStirng(const CcWString& sToParse, bool* pbOk)
{
  return getBoolFromStirng(sToParse.getString(), pbOk);
}

CcString CcStringUtil::getOctalStringFromByte(char uiByte)
{
  CcString sRet(CcGlobalStrings::Seperators::BackSlash);
  char c1 = (uiByte & 0xc0) >> 6;
  char c2 = (uiByte & 0x38) >> 3;
  char c3 = (uiByte & 0x07);
  if (c1 > 0)
  {
    sRet.append(c1 + '0');
    sRet.append(c2 + '0');
  }
  else if (c2 > 0)
      sRet.append(c2 + '0');
  sRet.append(c3 + '0');
  return sRet;
}

CcString CcStringUtil::encodeBase64(const CcByteArray& toEncode)
{
  char c1, c2, c3, c4;
  size_t uiDiv3 = toEncode.size() / s_uiBase64Divider;
  size_t uiModSize = uiDiv3 * s_uiBase64Divider;
  size_t uiMod3 = toEncode.size() - (uiModSize);
  size_t uiStringPos = 0;
  size_t uiOutputSize = uiDiv3 * s_uiBase64OutputDivider;
  if(uiMod3 > 0)
  {
    uiOutputSize += s_uiBase64OutputDivider;
  }
  CcString sRet;
  sRet.reserve(uiOutputSize);
  for (size_t i = 0; i < uiModSize; i += s_uiBase64Divider)
  {
    c1 = toEncode[i] >> 2;
    c2  = 0x30 & (toEncode[i] << 4);
    c2 |= 0x0f & (toEncode[i + 1] >> 4);
    c3  = 0x3c & (toEncode[i + 1] << 2);
    c3 |= 0x03 & (toEncode[i + 2] >> 6);
    c4  = 0x3f & (toEncode[i + 2]);
    sRet[uiStringPos++] = getBase64EncodedStringChar(c1);
    sRet[uiStringPos++] = getBase64EncodedStringChar(c2);
    sRet[uiStringPos++] = getBase64EncodedStringChar(c3);
    sRet[uiStringPos++] = getBase64EncodedStringChar(c4);
  }
  switch (uiMod3)
  {
    case 0:
      break;
    case 1:
      c1 = toEncode[uiModSize] >> 2;
      c2 = 0x30 & (toEncode[uiModSize] << 4);
      sRet[uiStringPos++] = getBase64EncodedStringChar(c1);
      sRet[uiStringPos++] = getBase64EncodedStringChar(c2);
      sRet[uiStringPos++] = '=';
      sRet[uiStringPos++] = '=';
      break;
    case 2:
      c1  = toEncode[uiModSize] >> 2;
      c2  = 0x30 & (toEncode[uiModSize] << 4);
      c2 |= 0x0f & (toEncode[uiModSize + 1] >> 4);
      c3  = 0x3c & (toEncode[uiModSize + 1] << 2);
      sRet[uiStringPos++] = getBase64EncodedStringChar(c1);
      sRet[uiStringPos++] = getBase64EncodedStringChar(c2);
      sRet[uiStringPos++] = getBase64EncodedStringChar(c3);
      sRet[uiStringPos++] = '=';
      break;
  }
  return sRet;
}

CcByteArray CcStringUtil::decodeBase64(const CcString& toDecode)
{
  CcByteArray oRet;
  CcString sLastValue(toDecode.getTrim());
  uint8 uiPatternCnt = 0;
  while (sLastValue.length() > 0 && sLastValue.at(sLastValue.length() - 1) == '=')
  {
    uiPatternCnt++;
    sLastValue.remove(sLastValue.length() - 1);
  }
  uiPatternCnt %= 3;

  CcStringList slSplitted = sLastValue.splitEvery(4);
  uint8 uiMod4 = sLastValue.length() % 4;
  sLastValue.clear();
  if (uiMod4 != 0)
  {
    sLastValue = slSplitted.last();
    slSplitted.remove(slSplitted.size() - 1);
  }

  for (const CcString& sCurrent : slSplitted)
  {
    char pCharBlock[3];
    char temp;
    temp = getBase64DecodedStringChar(sCurrent[0]);
    pCharBlock[0]  = temp << 2;
    temp = getBase64DecodedStringChar(sCurrent[1]);
    pCharBlock[0] |= temp >> 4;
    pCharBlock[1]  = temp << 4;
    temp = getBase64DecodedStringChar(sCurrent[2]);
    pCharBlock[1] |= temp >> 2;
    pCharBlock[2]  = temp << 6;
    temp = getBase64DecodedStringChar(sCurrent[3]);
    pCharBlock[2] |= temp;
    oRet.add(pCharBlock, 3);
  }
  switch (uiMod4)
  {
    case 0:
      // Do nothing, already done!
      break;
    case 1:
    {
      char temp;
      temp = getBase64DecodedStringChar(sLastValue[0]);
      temp = temp << 2;
      oRet.add(temp);
      break;
    }
    case 2:
    {
      char pCharBlock[2];
      char temp;
      temp = getBase64DecodedStringChar(sLastValue[0]);
      pCharBlock[0] = temp << 2;
      temp = getBase64DecodedStringChar(sLastValue[1]);
      pCharBlock[0] |= temp >> 4;
      if (uiPatternCnt > 1)
      {
        oRet.add(pCharBlock, 1);
      }
      else
      {
        pCharBlock[1] = temp << 4;
        oRet.add(pCharBlock, 2);
      }
      break;
    }
    case 3:
    {
      char pCharBlock[3];
      char temp;
      temp = getBase64DecodedStringChar(sLastValue[0]);
      pCharBlock[0] = temp << 2;
      temp = getBase64DecodedStringChar(sLastValue[1]);
      pCharBlock[0] |= temp >> 4;
      pCharBlock[1] = temp << 4;
      temp = getBase64DecodedStringChar(sLastValue[2]);
      pCharBlock[1] |= temp >> 2;
      if (uiPatternCnt > 0)
      {
        oRet.add(pCharBlock, 2);
      }
      else
      {
        pCharBlock[2] = temp << 6;
        oRet.add(pCharBlock, 3);
      }
      break;
    }
    default:
      CCDEBUG("Big mistake, modulo didn't work");
  }
  return oRet;
}

char CcStringUtil::getBase64EncodedStringChar(char cIn)
{
  if(cIn < 64)
  {
    static const char pAlphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                      'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                      'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                      'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                      '4', '5', '6', '7', '8', '9', '+', '/'};
    return pAlphabet[static_cast<uchar>(cIn)];
  }
  return -1;
}

char CcStringUtil::getBase64EncodedUrlStringChar(char cIn)
{
  if(cIn < 64)
  {
    static const char pAlphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                      'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                      'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                      'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                      '4', '5', '6', '7', '8', '9', '-', '_'};
    return pAlphabet[static_cast<uchar>(cIn)];
  }
  return -1;
}

char CcStringUtil::getBase64DecodedStringChar(char cIn)
{
  char cOut = 0;
  if (cIn >= 'A' && cIn <= 'Z')
    cOut = cIn - 'A';
  else if (cIn >= 'a' && cIn <= 'z')
    cOut = (cIn - 'a') + 26;
  else if (cIn >= '0' && cIn <= '9')
    cOut = (cIn - '0') + 52;
  else if (cIn == '+')
    cOut = 62;
  else if (cIn == '/')
    cOut = 63;
  return cOut;
}

char CcStringUtil::getBase64DecodedUrlStringChar(char cIn)
{
  char cOut = 0;
  if (cIn >= 'A' && cIn <= 'Z')
    cOut = cIn - 'A';
  else if (cIn >= 'a' && cIn <= 'z')
    cOut = (cIn - 'a') + 26;
  else if (cIn >= '0' && cIn <= '9')
    cOut = (cIn - '0') + 52;
  else if (cIn == '-')
    cOut = 62;
  else if (cIn == '_')
    cOut = 63;
  return cOut;
}

//! Base 58 alphabet
static const char CcStringUtil_pszBase58[] = {'1', '2', '3', '4', '5', '6', '7', '8',
                                              '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                              'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q',
                                              'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
                                              'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                                              'h', 'i', 'j', 'k', 'm', 'n', 'o', 'p',
                                              'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                              'y', 'z'};

CcString CcStringUtil::encodeBase58(const CcByteArray& toEncode)
{
  return encodeBaseX(toEncode, CcStringUtil_pszBase58, 58);
}

CcByteArray CcStringUtil::decodeBase58(const CcString& toDecode)
{
  return decodeBaseX(toDecode, CcStringUtil_pszBase58, 58);
}

CcString CcStringUtil::escapeString(const CcString& toDecode)
{
  CcString sReturn = toDecode;
  sReturn.replace("\"", "\"");
  sReturn.replace("\\", "\\\\");
  sReturn.replace("\r", "\\\r");
  sReturn.replace("\n", "\\\n");
  sReturn.replace("\b", "\\\b");
  sReturn.replace("\t", "\\\t");
  sReturn.replace("\f", "\\\f");
  return sReturn;
}

CcString CcStringUtil::getExtensionFromPath(const CcString& sPath)
{
  size_t uiPosLast = sPath.findLast(CcGlobalStrings::Seperators::Dot);
  // jump over last /
  uiPosLast++;
  if (uiPosLast < sPath.length())
  {
    return sPath.substr(uiPosLast);
  }
  return sPath;
}

CcString CcStringUtil::getFilenameFromPath(const CcString& sPath)
{
  size_t uiPosLast = sPath.findLast(CcGlobalStrings::Seperators::Path);
  // jump over last /
  uiPosLast++;
  if(uiPosLast < sPath.length())
  {
    return sPath.substr(uiPosLast);
  }
  return sPath;
}

CcString CcStringUtil::getDirectoryFromPath(const CcString& sPath)
{
  size_t uiPosLast = sPath.findLast(CcGlobalStrings::Seperators::Path);
  // jump over last /
  if (uiPosLast < sPath.length())
  {
    return sPath.substr(0,uiPosLast);
  }
  return CcGlobalStrings::Seperators::Slash;
}

CcString CcStringUtil::getHumanReadableSizePerSeconds(uint64 uiSize, const CcDateTime& oTime)
{
  CcString sRate;
  if (oTime.getTimestampS() > 0)
  {
    uint64 fSizePerSec = uiSize / oTime.getTimestampS();
    sRate = CcStringUtil::getHumanReadableSize(fSizePerSec);
    sRate += "/s";
  }
  else if (oTime.getTimestampMs() > 0)
  {
    uint64 fSizePerSec = (uiSize * 1000) / oTime.getTimestampMs();
    sRate = CcStringUtil::getHumanReadableSize(fSizePerSec);
    sRate += "/s";
  }
  else if (oTime.getTimestampUs() > 0)
  {
    uint64 fSizePerSec = (uiSize * 1000 * 1000) / oTime.getTimestampUs();
    sRate = CcStringUtil::getHumanReadableSize(fSizePerSec);
    sRate += "/s";
  }
  else
  {
    sRate = CcStringUtil::getHumanReadableSize(uiSize);
    sRate += "/0";
  }
  return sRate;
}

#define CcStringUtil_kiB_Size     (1024ull)                       //!< Convert bytes to number of kilo bytes
#define CcStringUtil_MiB_Size     (CcStringUtil_kiB_Size * 1024)  //!< Convert bytes to number of Mega bytes
#define CcStringUtil_GiB_Size     (CcStringUtil_MiB_Size * 1024)  //!< Convert bytes to number of Giga bytes
#define CcStringUtil_TiB_Size     (CcStringUtil_GiB_Size * 1024)  //!< Convert bytes to number of Tera bytes
#define CcStringUtil_EiB_Size     (CcStringUtil_TiB_Size * 1024)  //!< Convert bytes to number of Exa bytes
#define CcStringUtil_ZiB_Size     (CcStringUtil_EiB_Size * 1024)  //!< Convert bytes to number of Zetabytes

CcString CcStringUtil::getHumanReadableSize(uint64 uiSize, uint8 uiPrecision)
{
  CcString sRet;
  double fSize = static_cast<double>(uiSize);
  if (uiSize >= CcStringUtil_ZiB_Size)
  {
    sRet = CcString::fromNumber(fSize / CcStringUtil_ZiB_Size, uiPrecision, true);
    sRet += " ZiB";
  }
  else if (uiSize >= CcStringUtil_TiB_Size)
  {
    sRet = CcString::fromNumber(fSize / CcStringUtil_TiB_Size, uiPrecision, true);
    sRet += " TiB";
  }
  else if (uiSize >= CcStringUtil_GiB_Size)
  {
    sRet = CcString::fromNumber(fSize / CcStringUtil_GiB_Size, uiPrecision, true);
    sRet += " GiB";
  }
  else if (uiSize >= CcStringUtil_MiB_Size)
  {
    sRet = CcString::fromNumber(fSize / CcStringUtil_MiB_Size, uiPrecision, true);
    sRet += " MiB";
  }
  else if (uiSize >= CcStringUtil_kiB_Size)
  {
    sRet = CcString::fromNumber(fSize / CcStringUtil_kiB_Size, uiPrecision, true);
    sRet += " kiB";
  }
  else
  {
    sRet = CcString::fromNumber(uiSize);
    sRet += " B";
  }
  return sRet;
}

CcStringPair CcStringUtil:: getKeyValue(const CcString& sLine, char cSeperator)
{
  CcStringPair oPair;
  size_t uiEqualSign = findChar(sLine.getCharString(), cSeperator);
  if (uiEqualSign > 0 &&
      uiEqualSign < sLine.length())
  {
    oPair.setKey(sLine.substr(0, uiEqualSign).trim());
    oPair.setValue(sLine.substr(uiEqualSign + 1).trim());
  }
  return oPair;
}

uint32 CcStringUtil::toUint32(const char* pcString, size_t uiLen, bool* pbOk, uint8 uiBase)
{
  uint32 uiRet = 0;
  bool bOk = false;
  size_t uiPos = 0;

  if (pcString[uiPos] == 'x' ||
    (uiPos < uiLen + 1 &&
     pcString[uiPos] == '0' &&
     pcString[uiPos + 1] == 'x')
      )
  {
    uiBase = 16;
  }
  if (uiBase == 16)
  {
    if (uiLen > 0 && pcString[uiPos] == 'x')   uiPos++;
    else if (uiLen > 1 && pcString[uiPos + 1] == 'x') uiPos += 2;
    while (uiPos < uiLen)
    {
      uint32 uiNextValue = UINT32_MAX;
      if (pcString[uiPos] >= '0' && pcString[uiPos] <= '9')
      {
        uiNextValue = pcString[uiPos] - '0';
      }
      else if (pcString[uiPos] >= 'a' && pcString[uiPos] <= 'f')
      {
        uiNextValue = (pcString[uiPos] - 'a') + 10;
      }
      else if (pcString[uiPos] >= 'A' && pcString[uiPos] <= 'F')
      {
        uiNextValue = (pcString[uiPos] - 'A') + 10;
      }
      if (uiRet <= (UINT32_MAX / 16))
      {
        if (uiNextValue < 0x10)
        {
          bOk = true;
          uint32 uiNext = uiRet << 4;
          uiNext = uiNext + uiNextValue;
          if (uiNext < uiRet)
          {
            bOk = false;
            break;
          }
          else
          {
            uiRet = uiNext;
            uiPos++;
          }
        }
        else
        {
          bOk = false;
          break;
        }
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  else if (uiPos < uiLen)
  {
    while (uiPos < uiLen &&
           pcString[uiPos] >= '0' &&
           pcString[uiPos] <= '9')
    {
      bOk = true;
      if (uiRet <= (UINT32_MAX / 10))
      {
        uint32 uiNext = uiRet * 10;
        uiNext = uiNext + pcString[uiPos] - '0';
        if (uiNext < uiRet)
        {
          bOk = false;
          break;
        }
        else
          uiRet = uiNext;
        uiPos++;
      }
      else
      {
        bOk = false;
        break;
      }
    }
    if(uiPos < uiLen &&
       !CcStringUtil::isWhiteSpace(pcString[uiPos]))
    {
      bOk = false;
    }
  }
  if (pbOk != nullptr)
  {
    *pbOk = bOk;
  }
  return uiRet;
}

uint64 CcStringUtil::toUint64(const char* pcString, size_t uiLen, bool* pbOk, uint8 uiBase)
{
  uint64 uiRet = 0;
  bool bOk = false;
  size_t uiPos = 0;

  if (pcString[uiPos] == 'x' ||
    (uiPos < uiLen + 1 &&
     pcString[uiPos] == '0' &&
     pcString[uiPos + 1] == 'x')
      )
  {
    uiBase = 16;
  }
  if (uiBase == 16)
  {
    if (uiLen > 0 && pcString[uiPos] == 'x')   uiPos++;
    else if (uiLen > 1 && pcString[uiPos + 1] == 'x') uiPos += 2;
    while (uiPos < uiLen)
    {
      uint64 uiNextValue = UINT64_MAX;
      if (pcString[uiPos] >= '0' && pcString[uiPos] <= '9')
      {
        uiNextValue = pcString[uiPos] - '0';
      }
      else if (pcString[uiPos] >= 'a' && pcString[uiPos] <= 'f')
      {
        uiNextValue = (pcString[uiPos] - 'a') + 10;
      }
      else if (pcString[uiPos] >= 'A' && pcString[uiPos] <= 'F')
      {
        uiNextValue = (pcString[uiPos] - 'A') + 10;
      }
      if (uiRet <= (UINT64_MAX / 16))
      {
        if (uiNextValue < 0x10)
        {
          bOk = true;
          uint64 uiNext = uiRet << 4;
          uiNext = uiNext + uiNextValue;
          if (uiNext < uiRet)
          {
            bOk = false;
            break;
          }
          else
          {
            uiRet = uiNext;
            uiPos++;
          }
        }
        else
        {
          bOk = false;
          break;
        }
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  else if (uiPos < uiLen)
  {
    while (uiPos < uiLen &&
           pcString[uiPos] >= '0' &&
           pcString[uiPos] <= '9')
    {
      bOk = true;
      if (uiRet <= (UINT64_MAX / 10))
      {
        uint64 uiNext = uiRet * 10;
        uiNext = uiNext + pcString[uiPos] - '0';
        if (uiNext < uiRet)
        {
          bOk = false;
          break;
        }
        else
          uiRet = uiNext;
        uiPos++;
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  if (pbOk != nullptr)
  {
    *pbOk = bOk;
  }
  return uiRet;
}

uint32 CcStringUtil::toUint32(const wchar_t* pcString, size_t uiLen, bool* pbOk, uint8 uiBase)
{
  uint32 uiRet = 0;
  bool bOk = false;
  size_t uiPos = 0;

  if (pcString[uiPos] == 'x' ||
    (uiPos < uiLen + 1 &&
     pcString[uiPos] == '0' &&
     pcString[uiPos + 1] == 'x')
      )
  {
    uiBase = 16;
  }
  if (uiBase == 16)
  {
    if (uiLen > 0 && pcString[uiPos] == 'x')   uiPos++;
    else if (uiLen > 1 && pcString[uiPos + 1] == 'x') uiPos += 2;
    while (uiPos < uiLen)
    {
      uint32 uiNextValue = UINT32_MAX;
      if (pcString[uiPos] >= '0' && pcString[uiPos] <= '9')
      {
        uiNextValue = pcString[uiPos] - '0';
      }
      else if (pcString[uiPos] >= 'a' && pcString[uiPos] <= 'f')
      {
        uiNextValue = (pcString[uiPos] - 'a') + 10;
      }
      else if (pcString[uiPos] >= 'A' && pcString[uiPos] <= 'F')
      {
        uiNextValue = (pcString[uiPos] - 'A') + 10;
      }
      if (uiRet <= (UINT32_MAX / 16))
      {
        if (uiNextValue < 0x10)
        {
          bOk = true;
          uint32 uiNext = uiRet << 4;
          uiNext = uiNext + uiNextValue;
          if (uiNext < uiRet)
          {
            bOk = false;
            break;
          }
          else
          {
            uiRet = uiNext;
            uiPos++;
          }
        }
        else
        {
          bOk = false;
          break;
        }
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  else if (uiPos < uiLen)
  {
    while (uiPos < uiLen &&
           pcString[uiPos] >= '0' &&
           pcString[uiPos] <= '9')
    {
      bOk = true;
      if (uiRet <= (UINT32_MAX / 10))
      {
        uint32 uiNext = uiRet * 10;
        uiNext = uiNext + pcString[uiPos] - '0';
        if (uiNext < uiRet)
        {
          bOk = false;
          break;
        }
        else
          uiRet = uiNext;
        uiPos++;
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  if (pbOk != nullptr)
  {
    *pbOk = bOk;
  }
  return uiRet;
}

uint64 CcStringUtil::toUint64(const wchar_t* pcString, size_t uiLen, bool* pbOk, uint8 uiBase)
{
  uint64 uiRet = 0;
  bool bOk = false;
  size_t uiPos = 0;

  if (pcString[uiPos] == 'x' ||
    (uiPos < uiLen + 1 &&
     pcString[uiPos] == '0' &&
     pcString[uiPos + 1] == 'x')
      )
  {
    uiBase = 16;
  }
  if (uiBase == 16)
  {
    if (uiLen > 0 && pcString[uiPos] == 'x')   uiPos++;
    else if (uiLen > 1 && pcString[uiPos + 1] == 'x') uiPos += 2;
    while (uiPos < uiLen)
    {
      uint64 uiNextValue = UINT64_MAX;
      if (pcString[uiPos] >= '0' && pcString[uiPos] <= '9')
      {
        uiNextValue = pcString[uiPos] - '0';
      }
      else if (pcString[uiPos] >= 'a' && pcString[uiPos] <= 'f')
      {
        uiNextValue = (pcString[uiPos] - 'a') + 10;
      }
      else if (pcString[uiPos] >= 'A' && pcString[uiPos] <= 'F')
      {
        uiNextValue = (pcString[uiPos] - 'A') + 10;
      }
      if (uiRet <= (UINT64_MAX / 16))
      {
        if (uiNextValue < 0x10)
        {
          bOk = true;
          uint64 uiNext = uiRet << 4;
          uiNext = uiNext + uiNextValue;
          if (uiNext < uiRet)
          {
            bOk = false;
            break;
          }
          else
          {
            uiRet = uiNext;
            uiPos++;
          }
        }
        else
        {
          bOk = false;
          break;
        }
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  else if (uiPos < uiLen)
  {
    while (uiPos < uiLen &&
           pcString[uiPos] >= '0' &&
           pcString[uiPos] <= '9')
    {
      bOk = true;
      if (uiRet <= (UINT64_MAX / 10))
      {
        uint64 uiNext = uiRet * 10;
        uiNext = uiNext + pcString[uiPos] - '0';
        if (uiNext < uiRet)
        {
          bOk = false;
          break;
        }
        else
          uiRet = uiNext;
        uiPos++;
      }
      else
      {
        bOk = false;
        break;
      }
    }
  }
  if (pbOk != nullptr)
  {
    *pbOk = bOk;
  }
  return uiRet;
}

CcString CcStringUtil::fromUint64(uint64 uiValue, uint8 uiBase)
{
  CcString sRet;
  do
  {
    uint8 uiCurrentValue = uiValue % uiBase;
    if(uiCurrentValue < 10)
    {
      sRet.append('0' + uiCurrentValue);
    }
    else if(uiCurrentValue < 36)
    {
      sRet.append('a' + (uiCurrentValue - 10));
    }
    uiValue = uiValue / uiBase;
  } while (uiValue > 0);
  CcStatic::swap(sRet.getCharString(), sRet.length());
  return sRet;
}

CcString CcStringUtil::fromUint32(uint32 uiValue, uint8 uiBase)
{
  CcString sRet;
  do
  {
  uint8 uiCurrentValue = uiValue % uiBase;
    if(uiCurrentValue < 10)
    {
      sRet.append('0' + uiCurrentValue);
    }
    else if(uiCurrentValue < 36)
    {
      sRet.append('a' + (uiCurrentValue - 10));
    }
    uiValue = uiValue / uiBase;
  } while (uiValue > 0);
  CcStatic::swap(sRet.getCharString(), sRet.length());
  return sRet;
}

CcString CcStringUtil::fromInt64(int64 iValue, uint8 uiBase)
{
  CcString sRet;
  if(iValue < 0)
  {
    sRet.append('-');
    iValue = -iValue;
  }
  sRet.append(CcStringUtil::fromUint64(static_cast<uint64>(iValue), uiBase));
  return sRet;
}

CcString CcStringUtil::fromInt32(int32 iValue, uint8 uiBase)
{
  CcString sRet;
  if(iValue < 0)
  {
    sRet.append('-');
    iValue = -iValue;
  }
  sRet.append(CcStringUtil::fromUint32(static_cast<uint32>(iValue), uiBase));
  return sRet;
}

CcString CcStringUtil::fromFloat(float fValue, uint8 uiBase, bool bDisableExponent)
{
  CCUNUSED(bDisableExponent);
  CcString sRet;
  if (fValue < 0)
  {
    fValue = -fValue;
    sRet.append(CcGlobalStrings::Negative);
  }
  int64 iHighValue = static_cast<int64>(fValue);
  fValue = fValue - iHighValue;
  sRet.appendNumber(iHighValue);
  if (fValue != 0) sRet.append(".");
  while (fValue != 0)
  {
    fValue *= uiBase;
    uint8 uiTempValue = static_cast<uint8>(fValue);
    if (uiTempValue < 10)
    {
      sRet.append('0' + uiTempValue);
    }
    else if (uiTempValue < 36)
    {
      sRet.append('a' + (uiTempValue - 10));
    }
    fValue = fValue - uiTempValue;
  }
  return sRet;
}

CcString CcStringUtil::fromDouble(double fValue, uint8 uiBase, bool bDisableExponent)
{
  CCUNUSED(bDisableExponent);
  CcString sRet = CcStringUtil::fromInt32(static_cast<int32>(fValue), uiBase);
  if (fValue < 0) fValue = -fValue;
  return sRet;
}

CcString CcStringUtil::fromUtf16(const uint16* pUtf16, size_t uiLen)
{
  return CcString().fromUtf16(pUtf16, uiLen);
}

bool CcStringUtil::cmpWithLower(const CcString& sToCompare, const CcString& sToLowerCompare)
{
  return sToCompare.length() == sToLowerCompare.length() && sToCompare == sToLowerCompare;
}

CcString CcStringUtil::findArgument(const CcString& sString, size_t& uiPosition)
{
  CcString sReturn = "";
  if (sString.length() > uiPosition)
  {
    size_t uiNext = CcStringUtil::findNextNotWhiteSpace(sString, uiPosition);
    if (uiNext < sString.length())
    {
      uiPosition = uiNext;
      if (sString[uiPosition] == CcGlobalStrings::Seperators::Quote[0])
      {
        // Quoted String found
        uiPosition++;
        bool bComplete = false;
        size_t uiFirstPos = uiPosition;
        while (bComplete == false)
        {
          uiNext = findChar(sString, CcGlobalStrings::Seperators::Quote[0], uiPosition);
          if (uiNext < sString.length())
          {
            uiPosition = uiNext;
            // Check for escaped Quote
            if (uiNext > 0 && sString[uiPosition - 1] == CcGlobalStrings::Seperators::BackSlash[0])
            {
              // We have an escaped Quote, continue
              sReturn += sString.substr(uiFirstPos, (uiPosition - 1) - uiFirstPos);
              sReturn += CcGlobalStrings::Seperators::Quote;
              uiFirstPos = ++uiPosition;
            }
            else
            {
              sReturn += sString.substr(uiFirstPos, uiPosition - uiFirstPos);
              bComplete = true;
              uiPosition++;
            }
          }
          else
          {
            sReturn = sString.substr(uiFirstPos);
            uiPosition = sString.length();
            bComplete = true;
          }
        }
      }
      else if (sString[uiPosition] == CcGlobalStrings::Seperators::SingleQuote[0])
      {
        // SingleQuote String found
        uiPosition++;
        bool bComplete = false;
        size_t uiFirstPos = uiPosition;
        while (bComplete == false)
        {
          uiNext = findChar(sString, CcGlobalStrings::Seperators::SingleQuote[0], uiPosition);
          if (uiNext < sString.length())
          {
            uiPosition = uiNext;
            // Check for escaped SingleQuote
            if (uiNext > 0 && sString[uiPosition - 1] == CcGlobalStrings::Seperators::BackSlash[0])
            {
              // We have an escaped SingleQuote, continue
              sReturn += sString.substr(uiFirstPos, (uiPosition - 1) - uiFirstPos);
              sReturn += CcGlobalStrings::Seperators::SingleQuote;
              uiFirstPos = ++uiPosition;
            }
            else
            {
              sReturn += sString.substr(uiFirstPos, uiPosition - uiFirstPos);
              bComplete = true;
              uiPosition++;
            }
          }
          else
          {
            sReturn = sString.substr(uiFirstPos);
            uiPosition = sString.length();
            bComplete = true;
          }
        }
      }
      else
      {
        uiNext = CcStringUtil::findNextWhiteSpace(sString, uiPosition);
        if (uiNext < sString.length())
        {
          sReturn = sString.substr(uiPosition, uiNext - uiPosition);
          uiPosition = uiNext;
        }
        else
        {
          sReturn = sString.substr(uiPosition);
          uiPosition = sString.length();
        }
      }
    }
  }

  return sReturn;
}

CcString CcStringUtil::encodeBaseX(const CcByteArray& toEncode, const char* pcAlphabet, uint8 uiBaseSize)
{
  CcString oRet;
  size_t uiDigitSize = toEncode.size() * 138 / 100 + 1; // log(256) / log(58), rounded up.
  // Next requires initilized ByteArray with 0!
  CcByteArray digits(uiDigitSize);
  size_t uiLength = 0;
  size_t uiZeroes = 0;
  if (toEncode.size() > 0)
  {
    while (uiZeroes < toEncode.size())
    {
      if (toEncode.at(uiZeroes) == 0)
        uiZeroes++;
      else
        break;
    }
    for (size_t uiBaseCnt = uiZeroes; uiBaseCnt < toEncode.size(); uiBaseCnt++)
    {
      uint16 carry = (uint16)toEncode.at(uiBaseCnt);
      size_t i = 0;
      for (size_t uiCnt = digits.size(); (uiCnt > 0) && (carry != 0 || i < uiLength); uiCnt--)
      {
        carry += ((uint16)digits[uiCnt-1]) << 8;
        (uchar&)digits[uiCnt-1] = carry % uiBaseSize;
        carry = (carry / uiBaseSize);
        i++;
      }
      uiLength = i;
    }

    digits.remove(0, digits.size()-uiLength);
    while (digits.size() > 0 && digits[0] == 0)
      digits.remove(0, 1);
    for (size_t uiCnt = 0; uiCnt < uiZeroes; uiCnt++)
      oRet.append("1");
    for (char& cValue : digits)
    {
      oRet.append(pcAlphabet + cValue, 1);
    }
  }
  return oRet;
}

CcStringList CcStringUtil::getArguments(const CcString& sLine)
{
  size_t uiPosition =0;
  CcStringList oArgs;
  do
  {
    CcString sArg = findArgument(sLine, uiPosition);
    if(sArg.length())
    {
      oArgs.add(sArg);
    }
    else
    {
      break;
    }
  } while(uiPosition < sLine.length());
  return oArgs;
}

const CcString& CcStringUtil::setPath(CcString& sOld, const CcString& sNew)
{
  if(CcStringUtil::isPathAbsolute(sNew))
  {
    sOld = sNew;
  }
  else
  {
    sOld.appendPath(sNew);
    sOld.normalizePath();
  }
  return sOld;
}

bool CcStringUtil::isPathAbsolute(const CcString& sPath)
{
#ifdef WINDOWS
  return sPath.startsWith(CcGlobalStrings::Seperators::Slash) ||
    (sPath.size() > 1 && sPath[1] == ':') ||
    (sPath.size() > 1 && sPath[1] == '\\' && sPath[1] == '\\');
#else
  return sPath.startsWith(CcGlobalStrings::Seperators::Slash);
#endif
}

//https://github.com/gghez/meteor-base58/blob/master/basex.js
CcByteArray CcStringUtil::decodeBaseX(const CcString& toDecode, const char* pcAlphabet, uint8 uiBaseSize)
{
  CcByteArray oReturn;
  CcByteArray oEncodedString;
  size_t uiSize = toDecode.length()*733 / 1000 + 1;
  size_t uiZeroes = 0;
  size_t uiLength = 0;
  CcByteArray oDecoded(uiSize);
  for (size_t uiCnt = 0; uiCnt < toDecode.length(); uiCnt++)
  {
    uint8 i = 0;
    for (; i < uiBaseSize; i++)
    {
      if (pcAlphabet[i] == toDecode[uiCnt])
      {
        oEncodedString.append((char)i);
        break;
      }
    }
    if (i >= uiBaseSize)
      CCERROR("Wrong Base58 String: " + toDecode);
  }
  while (uiZeroes < oEncodedString.size())
  {
    if (oEncodedString.at(uiZeroes) == 0)
      uiZeroes++;
    else
      break;
  }

  for (size_t uiBaseCnt = uiZeroes; uiBaseCnt < oEncodedString.size(); uiBaseCnt++)
  {
    uint16 carry = oEncodedString[uiBaseCnt];
    size_t i = 0;
    for (size_t uiCnt = oDecoded.size(); (uiCnt > 0) && (carry != 0 || i < uiLength); uiCnt--)
    {
      carry += ((uchar&)oDecoded[uiCnt - 1]) * uiBaseSize;
      (uchar&)oDecoded[uiCnt - 1] = carry & 0xff;
      carry >>= 8;
      i++;
    }
    uiLength = i;
  }

  oDecoded.remove(0, oDecoded.size() - uiLength);
  while (oDecoded.size() > 0 && oDecoded[0] == 0)
    oDecoded.remove(0, 1);
  for (size_t uiCnt = 0; uiCnt < uiZeroes; uiCnt++)
    oReturn.append('\0');
  oReturn.append(oDecoded);
  return oReturn;
}
