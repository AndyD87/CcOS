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
 * @brief     Implementation of Class CcStringUtil
 */
#include "CcStringUtil.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcByteArray.h"
#include <math.h>

static const uint8 s_uiBase64Divider = 3;

size_t CcStringUtil::strlen(const char* pcString, size_t uiMaxLen)
{
  int i = 0;
  for (; i < uiMaxLen; i++)
    if (pcString[i] == 0)
      return i;
  return SIZE_MAX;
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

bool CcStringUtil::getBoolFromStirng(const CcString& sToParse, bool* pbOk)
{
  bool bRet = false;
  if (sToParse.trim().compare("TRUE", ESensitivity::CaseInsensitiv))
  {
    bRet = true;
    if (pbOk != NULL)
    {
      *pbOk = true;
    }
  }
  else if (sToParse.trim().compare("FALSE", ESensitivity::CaseInsensitiv))
  {
    bRet = false;
    if (pbOk != NULL)
    {
      *pbOk = true;
    }
  }
  else
  {
    bool bOkTemp;
    if (sToParse.toInt64(&bOkTemp) > 0 && bOkTemp)
      bRet = true;
    if (pbOk != NULL) 
    { 
      *pbOk = bOkTemp;
    }
  }
  return bRet;
}

CcString CcStringUtil::getOctalStringFromByte(char uiByte)
{
  CcString sRet("\\");
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
  CcString sRet;
  char c1, c2, c3, c4;
  size_t uiDiv3 = toEncode.size() / s_uiBase64Divider;
  size_t uiModSize = uiDiv3 * s_uiBase64Divider;
  size_t uiMod3 = toEncode.size() - (uiModSize);
  for (size_t i = 0; i < uiModSize; i += s_uiBase64Divider)
  {
    c1 = toEncode[i] >> 2;
    c2  = 0x30 & (toEncode[i] << 4);
    c2 |= 0x0f & (toEncode[i + 1] >> 4);
    c3  = 0x3c & (toEncode[i + 1] << 2);
    c3 |= 0x03 & (toEncode[i + 2] >> 6);
    c4  = 0x3f & (toEncode[i + 2]);
    sRet.append(getBase64EncodedStringChar(c1));
    sRet.append(getBase64EncodedStringChar(c2));
    sRet.append(getBase64EncodedStringChar(c3));
    sRet.append(getBase64EncodedStringChar(c4));
  }
  switch (uiMod3)
  {
    case 0:
      break;
    case 1:
      c1 = toEncode[uiModSize] >> 2;
      c2 = 0x30 & (toEncode[uiModSize] << 4);
      sRet.append(getBase64EncodedStringChar(c1));
      sRet.append(getBase64EncodedStringChar(c2));
      sRet.append('=');
      sRet.append('=');
      break;
    case 2:
      c1  = toEncode[uiModSize] >> 2;
      c2  = 0x30 & (toEncode[uiModSize] << 4);
      c2 |= 0x0f & (toEncode[uiModSize + 1] >> 4);
      c3  = 0x3c & (toEncode[uiModSize + 1] << 2);
      sRet.append(getBase64EncodedStringChar(c1));
      sRet.append(getBase64EncodedStringChar(c2));
      sRet.append(getBase64EncodedStringChar(c3));
      sRet.append('=');
      break;
  }
  return sRet;
}

CcByteArray CcStringUtil::decodeBase64(const CcString& toDecode)
{
  CcByteArray oRet;
  CcString sLastValue(toDecode.trim());
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
  char cOut = 0;
  if (cIn < 26)
    cOut = 'A' + cIn;
  else if (cIn < 52)
    cOut = 'a' + (cIn - 26);
  else if (cIn < 62)
    cOut = '0' + (cIn - 52);
  else if (cIn == 62)
    cOut = '+';
  else if (cIn == 63)
    cOut = '/';
  return cOut;
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

static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

CcString CcStringUtil::encodeBase58(const CcByteArray& toEncode)
{
  return encodeBaseX(toEncode, pszBase58, 58);
}

CcByteArray CcStringUtil::decodeBase58(const CcString& toDecode)
{
  return decodeBaseX(toDecode, pszBase58, 58);
}

CcString CcStringUtil::getFilenameFromPath(const CcString& sPath)
{
  size_t uiPosLast = sPath.findLast('/');
  // jump over last /
  uiPosLast++;
  if(uiPosLast < sPath.length())
  {
    return sPath.substr(uiPosLast);
  }
  return sPath;
}

CcString CcStringUtil::encodeBaseX(const CcByteArray& toEncode, const char* pcAlphabet, uint8 uiBaseSize)
{
  CcString oRet;
  size_t uiDigitSize = toEncode.size() * 138 / 100 + 1; // log(256) / log(58), rounded up.
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
      uint16 carry = (uchar&)toEncode.at(uiBaseCnt);
      size_t i = 0;
      for (size_t uiCnt = digits.size(); (uiCnt > 0) && (carry != 0 || i < uiLength); uiCnt--)
      {
        carry += (uchar&)digits[uiCnt-1] << 8;
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
      carry += (uchar&)oDecoded[uiCnt - 1] * uiBaseSize;
      (uchar&)oDecoded[uiCnt - 1] = carry % 0x100;
      carry = (carry / 0x100);
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
