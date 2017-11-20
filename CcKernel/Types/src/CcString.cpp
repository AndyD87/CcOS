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
 * @brief      Implementation of class CcString
 */
#include "CcString.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcStringList.h"
#include "CcWString.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdarg>
#include "CcGlobalStrings.h"

#ifndef WIN32
  #define TO_STRING_DEPRECATED
#endif

const size_t c_uiDefaultMultiplier = 16;

CcString::CcString()
{
  reserve(0);
}

CcString::CcString(const CcString& rhs)
{
  operator=(rhs);
}

CcString::CcString(size_t uiLength, const char cDefaultChar)
{
  reserve(uiLength);
  memset(m_pBuffer, cDefaultChar, uiLength);
  m_uiLength = uiLength;
  m_pBuffer[m_uiLength] = 0;
}

CcString::CcString(const char* cString)
{
  size_t uiLength = CcStringUtil::strlen(cString);
  append(cString, uiLength);
}

CcString::CcString(wchar_t * wstr) :
  CcString()
{
  appendWchar(wstr);
}

CcString::CcString(wchar_t * wstr, size_t uiLength) :
  CcString()
{
  appendWchar(wstr ,uiLength);
}

CcString::CcString(const char* cString, size_t uiLength)
{
  append(cString, uiLength);
}

CcString::CcString(const char cChar)
{
  append(&cChar, 1);
}

CcString::CcString(const CcByteArray& baString)
{
  append(baString.getArray(), baString.size());
}

CcString::~CcString() 
{
  deleteBuffer();
}

CcString& CcString::format(const char* sFormat, ...)
{
  char cString[1024];
  va_list argptr;
  va_start(argptr, sFormat);
#ifndef WIN32
  ::sprintf(cString, sFormat, argptr);
#else
  sprintf_s(cString, sFormat, argptr);
#endif
  va_end(argptr);
  set(cString);
  return *this;
}

CcString& CcString::remove(size_t uiPos, size_t uiLength)
{
  if (uiPos < m_uiLength)
  {
    if (uiLength > m_uiLength - uiPos)
      uiLength = m_uiLength - uiPos;
    size_t uiBeginLast = uiPos + uiLength;
    size_t uiLengthLast = m_uiLength - uiBeginLast;
    for(size_t i=0; i< uiLengthLast; i++)
      m_pBuffer[uiPos+i] = m_pBuffer[uiBeginLast+i];
    m_uiLength -= uiLength;
    m_pBuffer[m_uiLength] = 0;
  }
  return *this;
}

CcString CcString::substr(size_t pos, size_t len) const
{
  CcString sRet;
  if (pos < m_uiLength)
  {
    if (len > m_uiLength      ||
        len + pos > m_uiLength)
    {
      sRet.append(m_pBuffer + pos, m_uiLength - pos);
    }
    else
    {
      sRet.append(m_pBuffer + pos, len);
    }
  }
  return sRet;
}

CcString CcString::replace(const CcString& needle, const CcString& replace) const
{
  size_t pos=0;
  CcString sRet(*this);
  while (pos < sRet.length())
  {
    pos = sRet.find(needle, pos);
    if (pos != SIZE_MAX)
    {
      sRet.erase(pos, needle.length());
      sRet.insert(pos, replace);
      pos += replace.length();
    }
  }
  return sRet;
}

CcString CcString::getStringBetween(const CcString& preStr, const CcString& postStr, size_t offset, size_t *pos) const
{
  CcString sRet;
  size_t posFirst = findFirstOf(preStr.m_pBuffer, preStr.m_uiLength, offset) ;
  if (posFirst != SIZE_MAX)
  {
    posFirst += preStr.length();
    size_t posSecond = findFirstOf(postStr.m_pBuffer, postStr.m_uiLength, posFirst);
    if (posSecond != SIZE_MAX)
    {
      size_t len = posSecond - posFirst;
      sRet.append(substr(posFirst, len));
      if (pos != 0)
        *pos = posFirst + preStr.length();
    }
    else
    {
      sRet = substr(posFirst);
    }
  }
  return sRet;
}

CcString CcString::getLastLine(void) const
{
  size_t posR = findLast("\r");
  size_t posN = findLast(CcGlobalStrings::EolShort);
  if (posR > posN)
    return substr(posR);
  else if (posN > posR)
    return substr(posN);
  else
    return *this;
}

bool CcString::compare(const CcString& sToCompare, ESensitivity bSensitivity) const
{
  if(bSensitivity==ESensitivity::CaseSensitiv)
  {
    if (sToCompare.m_uiLength == m_uiLength)
    {
      for (size_t uiCnt = 0; uiCnt < m_uiLength; uiCnt++)
      {
        if (sToCompare.m_pBuffer[uiCnt] != m_pBuffer[uiCnt])
          return false;
      }
      return true;
    }
  }
  else
  {
    CcString cs1(*this);
    cs1.toLower();
    CcString cs2(sToCompare);
    cs2.toLower();
    return cs1.compare(cs2, ESensitivity::CaseSensitiv);
  }
  return false;
}

size_t CcString::findLast(const CcString& sToFind) const
{
  size_t iRet = SIZE_MAX;
  size_t temp=0;
  while ((temp = find(sToFind, temp)) != SIZE_MAX)
  {
    iRet = temp;
    temp++;
  }
  return iRet;
}

size_t CcString::find(const CcString& sToFind, size_t offset) const
{
  return findFirstOf(sToFind.m_pBuffer, sToFind.m_uiLength, offset);
}

size_t CcString::findFirstOf(const char* pcString, size_t uiLength, size_t uiOffset) const
{
  size_t uiRet = SIZE_MAX;
  if (uiOffset < m_uiLength)
  {
    for (size_t uiCntLocal = uiOffset; uiCntLocal + uiLength <= m_uiLength && uiRet == SIZE_MAX; uiCntLocal++)
    {
      size_t uiCntInput = 0;
      for (uiCntInput = 0; uiCntInput < uiLength; uiCntInput++)
      {
        if (m_pBuffer[uiCntLocal + uiCntInput] != pcString[uiCntInput])
          break;
      }
      if (uiCntInput == uiLength)
        uiRet = uiCntLocal;
    }
  }
  return uiRet;
}

bool CcString::isStringAtOffset(const CcString& sToCompare, size_t uiOffset) const
{
  if (sToCompare.m_uiLength + uiOffset > m_uiLength)
  {
    return false;
  }
  else {
    for (size_t i = 0; i < sToCompare.length(); i++)
    {
      if (sToCompare.at(i) != at(i + uiOffset))
        return false;
    }
  }
  return true;
}


bool CcString::startsWith(const CcString& sToCompare) const
{
  return isStringAtOffset(sToCompare, 0);
}

bool CcString::endsWith(const CcString& sToCompare) const
{
  return isStringAtOffset(sToCompare, m_uiLength - sToCompare.m_uiLength);
}

uint64 CcString::toUint64( bool *bOk) const
{
  uint64 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = std::stoull(std::string(m_pBuffer, m_uiLength), &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

uint32 CcString::toUint32( bool *bOk) const
{
  uint32 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = std::stoul(std::string(m_pBuffer, m_uiLength), &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

uint16 CcString::toUint16(bool *bOk) const
{
  uint16 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = 0xffff & std::stoul(std::string(m_pBuffer, m_uiLength), &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

uint8 CcString::toUint8(bool *bOk) const
{
  uint8 uiRet = 0;
  try
  {
    size_t uiCharCount;
    uiRet = 0xff & std::stoul(std::string(m_pBuffer, m_uiLength), &uiCharCount, 0);
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return uiRet;
}

int64 CcString::toInt64( bool *bOk)const
{
  int64 iRet = 0;
  try
  {
    iRet = std::stoll(std::string(m_pBuffer, m_uiLength));
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

int32 CcString::toInt32( bool *bOk)const
{
  int32 iRet = 0;
  try
  {
    iRet = std::stol(std::string(m_pBuffer, m_uiLength));
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

int16 CcString::toInt16(bool *bOk)const
{
  int16 iRet = 0;
  try
  {
    iRet = 0x0000ffff & std::stoi(std::string(m_pBuffer, m_uiLength));
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

int8 CcString::toInt8(bool *bOk) const
{
  int8 iRet = 0;
  try
  {
    iRet = 0x000000ff & std::stoi(std::string(m_pBuffer, m_uiLength));
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return iRet;
}

float CcString::toFloat(bool* bOk) const
{
  float fRet = 0;
  try
  {
    fRet = std::stof(std::string(m_pBuffer, m_uiLength));
    if (bOk != 0)
      *bOk = true;
  }
  catch(...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return fRet;
}

double CcString::toDouble(bool* bOk) const
{
  double fRet = 0;
  try
  {
    fRet = std::stod(std::string(m_pBuffer, m_uiLength));
    if (bOk != 0)
      *bOk = true;
  }
  catch (...)
  {
    if (bOk != 0)
      *bOk = false;
  }
  return fRet;
}

bool CcString::toBool(bool* bOk) const
{
  return CcStringUtil::getBoolFromStirng(*this, bOk);
}

CcString& CcString::toUpper(void)
{
  size_t uiLength = m_uiLength;
  while (uiLength--)
  {
    m_pBuffer[uiLength] = (char)::toupper(m_pBuffer[uiLength]);
  }
  return *this;
}

CcString CcString::getUpper(void) const
{
  return CcString(*this).toUpper();
}

CcString& CcString::toLower(void)
{
  size_t uiLength = m_uiLength;
  while (uiLength--)
  {
    m_pBuffer[uiLength] = (char)::tolower(m_pBuffer[uiLength]);
  }
  return *this;
}

CcString CcString::getLower(void) const
{
  return CcString(*this).toLower();
}

CcString& CcString::appendNumber(uint8 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << (uint) number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string((uint) number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(int8 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << (int) number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string((int) number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(uint16 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(int16 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(uint32 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(int32 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(uint64 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(int64 number, uint8 uiBase)
{
  std::stringstream stream;
  std::string sTemp;
  switch (uiBase)
  {
    case 16:
      stream << std::hex << number;
      sTemp = stream.str();
      break;
    default:
      sTemp = std::to_string(number);
      break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcString& CcString::appendNumber(float number)
{
  std::ostringstream os;
  os << std::setprecision(std::numeric_limits<unsigned>::digits10 + 1) << number;
  append(os.str().c_str());
  if (number == 0)
  {
    append(".0");
  }
  return *this;
}

CcString& CcString::appendNumber(double number)
{
  std::ostringstream os;
  os << std::setprecision(std::numeric_limits<unsigned>::digits10 + 1) << number;
  append(os.str().c_str());
  if (number == 0)
  {
    append(".0");
  }
  return *this;
}

size_t CcString::length( void ) const
{
  return m_uiLength;
}

size_t CcString::posNextNotWhitespace(size_t offset) const
{
  size_t uiRet = SIZE_MAX;
  for (; offset < length() && uiRet == SIZE_MAX; offset++)
  {
    switch (at(offset))
    {
      case ' ':
        continue;
      case '\t':
        continue;
      case '\n':
        continue;
      case '\r':
        continue;
      case '\f':
        continue;
      case '\v':
        continue;
      default:
        uiRet = offset;
    }
  }
  return uiRet;
}

size_t CcString::posNextWhitespace(size_t offset) const
{
  size_t uiRet = SIZE_MAX;
  for (; offset < length() && uiRet == SIZE_MAX; offset++)
  {
    switch (at(offset))
    {
      case ' ':
        uiRet = offset;
        break;
      case '\t':
        uiRet = offset;
        break;
      case '\n':
        uiRet = offset;
        break;
      case '\r':
        uiRet = offset;
        break;
      case '\f':
        uiRet = offset;
        break;
      case '\v':
        uiRet = offset;
        break;
    }
  }
  return uiRet;
}

char& CcString::at(size_t pos) const
{
  return m_pBuffer[pos];
}

const char* CcString::getCharString(void) const 
{
  return m_pBuffer;
}

char* CcString::getCharString(void)
{
  return m_pBuffer;
}

CcByteArray CcString::getByteArray(void) const
{
  CcByteArray ca(getCharString(), length());
  return ca;
}

CcString CcString::getOsPath() const
{
#ifdef WIN32
    return replace('/', '\\');
#else
    return *this;
#endif
}

CcStringList CcString::split(const CcString& delimiter) const
{
  CcStringList slRet;
  size_t save = 0;
  size_t offset = find(delimiter);
  while (offset != SIZE_MAX)
  {
    if (offset != save)
    {
      slRet.append(substr(save, offset -save));
    }
    save = offset + delimiter.length();
    offset = find(delimiter, save);
  }
  if (save != length())
    slRet.append(substr(save));
  return slRet;
}

CcStringList CcString::splitEvery(size_t uiNumber) const
{
  CcStringList slRet;
  size_t i = 0;
  for (; i < length(); i+=uiNumber)
  {
    slRet.add(substr(i, uiNumber));
  }
  return slRet;
}

CcStringList CcString::splitLines() const
{
  CcStringList slRet;
  size_t save = 0;
  size_t offsetN;
  size_t offsetR;
  do
  {
    offsetN = find("\n", save);
    offsetR = find("\r", save);
    if (offsetR < offsetN)
    {
      slRet.append(substr(save, offsetR - save));
      if (offsetR < length()-1 && 
          at(offsetR + 1) )
      {
        save = offsetR + 2;
      }
      else
      {
        save = offsetR + 1;
      }
    }
    else if (offsetN < offsetR)
    {
      slRet.append(substr(save, offsetN - save - 1));
      save = offsetN + 1;
    }
    else
    {
      slRet.append(substr(save));
      save = SIZE_MAX;
    }
  } while (save != SIZE_MAX);
  return slRet;
}

CcString& CcString::fromLatin1(const char* cString, size_t uiLength)
{
  clear();
  for (size_t i = 0; i < uiLength; i++)
  {
    if (static_cast<uchar>(cString[i]) > 0x7f)
    {
      if (static_cast<uchar>(cString[i]) > 0xbf)
      {
        append(static_cast<uchar>(0xc3));
        append(static_cast<uchar>(cString[i]) - 0x40);
      }
      else
      {
        append(static_cast<uchar>(0xc2));
        append(static_cast<uchar>(cString[i]));
      }
    }
    else
    {
      append(static_cast<uchar>(cString[i]));
    }
  }
  return *this;
}

CcString CcString::getLatin1() const
{
  CcString sRet;
  for (size_t i = 0; i < length(); i++)
  {
    if (static_cast<uchar>(at(i)) > 0x7f)
    {
      if (static_cast<uchar>(at(i)) == 0xc3 && i < length())
      {
        sRet.append(at(i+1) + 0x40);
        i++;
      }
      else if (static_cast<uchar>(at(i)) == 0xc2 && i < length())
      {
        sRet.append(static_cast<uchar>(at(i+1)));
        i++;
      }
    }
    else
    {
      sRet.append(at(i));
    }
  }
  return sRet;
}

CcString& CcString::fromUnicode(const wchar_t* cString, size_t uiLength)
{
  clear();
  for (size_t i = 0; i < uiLength; i++)
  {
    if (cString[i] < 0x80)
    {
      append(static_cast<uchar>(cString[i]));
    }
    else if (static_cast<uchar>(cString[i]) < 0xc0)
    {
      append(static_cast<uchar>(0xc2));
      append(static_cast<uchar>(cString[i]));
    }
    else if (cString[i] < 0x800)
    {
      uint8 uiHead = cString[i] >> 8;
      uint8 uiTemp = cString[i] & 0xff;
      if (uiTemp < 0x40)
      {
        append(static_cast<uchar>(0xc4 + uiHead));
        append(static_cast<uchar>((cString[i] & 0xff) + 0x80));
      }
      else if (uiTemp < 0x80)
      {
        append(static_cast<uchar>(0xc5 + uiHead));
        append(static_cast<uchar>((cString[i] & 0xff) + 0x40));
      }
      else if (uiTemp < 0xC0)
      {
        append(static_cast<uchar>(0xc6 + uiHead));
        append(static_cast<uchar>((cString[i] & 0xff)));
      }
      else
      {
        append(static_cast<uchar>(0xc3 + uiHead));
        append(static_cast<uchar>(cString[i] & 0xff) - 0x40);
      }
    }
    else
    {
      uint8 uiMaster = static_cast<uint8>(cString[i] >> 12);
      uint8 uiHead = static_cast<uint8>((cString[i] & 0x0fff) >> 6);
      append(static_cast<uchar>(0xe0 + uiMaster));
      append(static_cast<uchar>(0x80 + uiHead));
      append(static_cast<uchar>(cString[i] & 0x3f) + 0x80);
    }
  }
  return *this;
}

CcString& CcString::fromUnicode(const CcWString& sString)
{
  return fromUnicode(sString.getWcharString(), sString.length());
}

CcWString CcString::getWString() const
{
  return CcWString(*this);
}

CcString CcString::fromNumber(uint8 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint16 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint32 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(uint64 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int8 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int16 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int32 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(int64 number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendNumber(number, uiBase);
  return sRet;
}

CcString CcString::fromNumber(float number)
{
  CcString sRet;
  sRet.appendNumber(number);
  return sRet;
}

CcString CcString::fromNumber(double number)
{
  CcString sRet;
  sRet.appendNumber(number);
  return sRet;
}

CcString &CcString::normalizePath(void)
{
  *this = replace("\\", "/");
  return *this;
}

CcString CcString::extractFilename(void) const
{
  CcString sRet;
  size_t pos1 = findLast("/");
  size_t pos2 = findLast("\\");
  size_t subStrPos = SIZE_MAX;
  if (pos1 != SIZE_MAX)
  {
    if (pos2 != SIZE_MAX)
    {
      if (pos1 > pos2)
        subStrPos = pos1;
      else
        subStrPos = pos2;
    }
    else{
      subStrPos = pos1;
    }
  }
  else{
    subStrPos = pos2;
  }
  if (subStrPos != SIZE_MAX)
    sRet = substr(subStrPos);
  else 
    sRet = *this;
  return sRet;
}

CcString CcString::extractPath(void)
{
  CcString sRet;
  size_t pos1 = findLast("/");
  if(pos1 != SIZE_MAX)
  {
    sRet = substr(0, pos1 + 1);
  }
  else
  {
    sRet = '/';
  }
  return sRet;
}

CcString &CcString::appendPath(const CcString& toAppend)
{
  CcString seperator;
  if (toAppend.length() > 0)
  {
    if (length() > 0 &&
      at(length() - 1) != '/')
    {
      seperator = "/";
    }
    if (toAppend.startsWith("/") ||
      (toAppend.length() > 1 && toAppend.at(1) == ':'))
    {
      clear();
      append(toAppend);
    }
    else
    {
      if (toAppend.startsWith("./"))
      {
        append(seperator);
        append(toAppend.substr(2));
      }
      else
      {
        append(seperator);
        append(toAppend);
      }
    }
  }
  return *this;
}

CcString& CcString::fillBegin(const CcString& sFillString, size_t uiCount)
{
  CcString sBeginning;
  for (size_t i = 0; i < uiCount; i++)
  {
    sBeginning << sFillString;
  }
  *this = sBeginning + *this;
  return *this;
}

CcString& CcString::fillBeginUpToLength(const CcString& sFillString, size_t uiCount)
{
  if (uiCount > length())
    fillBegin(sFillString, uiCount - length());
  return *this;
}

CcString& CcString::fillEnd(const CcString& sFillString, size_t uiCount)
{
  CcString sEnding;
  for (size_t i = 0; i < uiCount; i++)
  {
    sEnding.append(sFillString);
  }
  this->append(sEnding);
  return *this;
}

CcString& CcString::fillEndUpToLength(const CcString& sFillString, size_t uiCount)
{
  return fillEnd(sFillString, uiCount - length());
}

CcString CcString::trimL(void) const
{
  CcString sRet = *this;
  while ( 0 != sRet.length() &&
          CcStringUtil::isWhiteSpace(at(0)))
  {
    sRet.remove(0);
  }
  return sRet;
}

CcString CcString::trimR(void) const
{
  CcString sRet = *this;
  size_t pos = sRet.length() - 1;
  while (pos < sRet.length() &&
          CcStringUtil::isWhiteSpace(at(pos))
        )
  {
    sRet.remove(pos);
    pos--;
  }
  return sRet;
}

CcString& CcString::append(const CcString& toAppend)
{
  return append(toAppend.m_pBuffer, toAppend.m_uiLength);
}

CcString& CcString::append(const char* toAppend)
{
  return append(toAppend, CcStringUtil::strlen(toAppend));
}

CcString& CcString::appendWchar(const wchar_t* str)
{
  if (str != NULL)
  {
    size_t i = 0;
    while (str[i] != 0) i++;
    appendWchar(str, i);
  }
  return *this;
}

CcString& CcString::append(const char toAppend)
{
  return append(&toAppend, 1);
}

CcString& CcString::append(const char *toAppend, size_t length)
{
  reserve(m_uiLength + length);
  memcpy(m_pBuffer + m_uiLength, toAppend, length);
  m_uiLength += length;
  m_pBuffer[m_uiLength] = 0;
  return *this;
}

CcString& CcString::appendWchar(const wchar_t toAppend)
{
  return appendWchar(&toAppend, 1);
}

CcString& CcString::appendWchar(const wchar_t* str, size_t uiLength)
{
  return append(CcString().fromUnicode(str, uiLength));
}

CcString& CcString::append(const CcByteArray &toAppend, size_t pos, size_t len)
{
  if (len == SIZE_MAX)
    len = toAppend.size()-pos;
  char* arr = toAppend.getArray(pos);
  return append(arr, len);
}

CcString& CcString::prepend(const CcString& toAppend)
{
  return insert(0, toAppend.m_pBuffer, toAppend.m_uiLength);
}

CcString& CcString::prepend(const char* toAppend)
{
  return insert(0, toAppend, CcStringUtil::strlen(toAppend));
}

CcString& CcString::prepend(const char toAppend)
{
  return insert(0, &toAppend, 1);
}

CcString& CcString::prepend(const char *toAppend, size_t length)
{
  return insert(0, toAppend, length);
}

CcString& CcString::prepend(const CcByteArray &toAppend, size_t pos, size_t len)
{
  if (len == SIZE_MAX)
    len = toAppend.size() - pos;
  char* arr = toAppend.getArray(pos);
  return insert(0, arr, len);
}

CcString& CcString::setOsPath(const CcString & sPathToSet)
{
  return set(sPathToSet.replace('\\', '/').getCharString());
}

CcString& CcString::appendIPv4(const ipv4_t &ipAddr)
{
  appendNumber(ipAddr.ip4);
  append('.');
  appendNumber(ipAddr.ip3);
  append('.');
  appendNumber(ipAddr.ip2);
  append('.');
  appendNumber(ipAddr.ip1);
  return *this;
}

CcString& CcString::insert(size_t pos, const char* pcToInsert, size_t uiLength)
{
  reserve(m_uiLength + uiLength);
  size_t uiNewEnd = m_uiLength + uiLength;
  for (size_t uiCnt = 0; uiCnt <= m_uiLength-pos; uiCnt++)
  {
    m_pBuffer[uiNewEnd - uiCnt] = m_pBuffer[m_uiLength - uiCnt];
  }
  memcpy(m_pBuffer + pos, pcToInsert, uiLength);
  m_uiLength += uiLength;
  return *this;
}

CcString& CcString::insert(size_t pos, const CcString& toInsert)
{
   return insert(pos, toInsert.getCharString(), toInsert.length());
}

void CcString::clear( void )
{
  reserve(0);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

CcString &CcString::erase(size_t pos, size_t len)
{
  if (len+pos > m_uiLength)
    len = m_uiLength -pos;
  remove(pos, len);
  return *this;
}

bool CcString::operator<(const CcString& oToCompare)
{
  size_t uiLowest = (m_uiLength < oToCompare.m_uiLength) ? m_uiLength : oToCompare.m_uiLength;
  for (size_t uiCnt = 0; uiCnt < uiLowest; uiCnt++)
  {
    if (m_pBuffer[uiCnt] < oToCompare.m_pBuffer[uiCnt])
      return true;
    else if (m_pBuffer[uiCnt] > oToCompare.m_pBuffer[uiCnt])
      return false;
  }
  if (m_uiLength < oToCompare.m_uiLength)
    return true;
  else
    return false;
}

bool CcString::operator>(const CcString& oToCompare)
{
  size_t uiLowest = (m_uiLength < oToCompare.m_uiLength) ? m_uiLength : oToCompare.m_uiLength;
  for (size_t uiCnt = 0; uiCnt < uiLowest; uiCnt++)
  {
    if (m_pBuffer[uiCnt] > oToCompare.m_pBuffer[uiCnt])
      return true;
    else if (m_pBuffer[uiCnt] > oToCompare.m_pBuffer[uiCnt])
      return false;
  }
  if (m_uiLength > oToCompare.m_uiLength)
    return true;
  else
    return false;
}

CcString& CcString::operator=(CcString&& oToMove)
{
  if (this != &oToMove)
  {
    deleteBuffer();
    m_pBuffer = oToMove.m_pBuffer;
    m_uiLength = oToMove.m_uiLength;
    m_uiReserved = oToMove.m_uiReserved;
    oToMove.m_pBuffer = nullptr;
    oToMove.m_uiLength = 0;
    oToMove.m_uiReserved = 0;
  }
  return *this;
}

void CcString::reserve(size_t uiSize)
{
  if (uiSize + 1 > m_uiReserved)
  {
    size_t uiNewLen = uiSize + 1;
    size_t uiMultiplier = uiNewLen / c_uiDefaultMultiplier;
    if ((uiNewLen % c_uiDefaultMultiplier) > 0)
    {
      uiNewLen = c_uiDefaultMultiplier * (uiMultiplier+1);
    }
    else
    {
      uiNewLen = c_uiDefaultMultiplier * uiMultiplier;
    }
    char* pBuffer = new char[uiNewLen];
    CCMONITORNEW(pBuffer);
    size_t uiOldLen = m_uiLength;
    memcpy(pBuffer, m_pBuffer, sizeof(char)*m_uiLength);
    deleteBuffer();
    m_pBuffer = pBuffer;

    m_uiLength            = uiOldLen;
    m_uiReserved          = uiNewLen;
    m_pBuffer[m_uiLength] = 0;
  }
  else
  {
    // keep reserved size.
  }
}

void CcString::deleteBuffer()
{
  if (m_pBuffer != nullptr)
  {
    CCMONITORDELETE(m_pBuffer);
    delete m_pBuffer;
    m_pBuffer = nullptr;
    m_uiLength = 0;
    m_uiReserved = 0;
  }
}
