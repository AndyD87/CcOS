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
 * @brief      Implementation of class CcString
 */
#include "CcString.h"
#include "CcByteArray.h"
#include "CcStringUtil.h"
#include "CcStringList.h"
#include "CcWString.h"
#include "CcStatic.h"
#include "CcGlobalStrings.h"
#include "CcIp.h"
#include "CcKernel.h"

#ifdef FULL_OS_AVAILABLE
  #include <stdlib.h>
  #include <stdio.h>
  #include <cstdarg>
  #include <sstream>
  #include <iomanip>
  #include <algorithm>
#else
  #ifdef GENERIC
    #include <cstdlib>
  #endif
  #include <errno.h>
#endif

#ifndef WINDOWS
  #define TO_STRING_DEPRECATED
#endif

const size_t c_uiDefaultMultiplier = 16;

CcString::CcString()
{
}

CcString::CcString(size_t uiLength, const char cDefaultChar)
{
  reserve(uiLength, cDefaultChar);
}

CcString::CcString(const char* cString)
{
  size_t uiLength = CcStringUtil::strlen(cString);
  append(cString, uiLength);
}

CcString::CcString(wchar_t * wstr)
{
  appendWchar(wstr);
}

CcString::CcString(wchar_t * wstr, size_t uiLength)
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

void CcString::reserve(size_t uiLength)
{
  allocateBuffer(uiLength);
  m_uiLength = uiLength;
  m_pBuffer[uiLength] = 0;
}

void CcString::reserve(size_t uiLength, const char cDefaultChar)
{
  size_t uiLastLength = m_uiLength;
  allocateBuffer(uiLength);
  m_uiLength = uiLength;
  if(m_uiLength > uiLastLength)
  {
    CcStatic::memset(m_pBuffer + uiLastLength, cDefaultChar, m_uiLength - uiLastLength);
  }
  m_pBuffer[m_uiLength] = 0;
}

CcString& CcString::format(const char* sFormat, ...)
{
  char cString[1024];
#if defined(WINDOWS) && !defined(__GNUC__) && !defined(_KERNEL_MODE)
  va_list argptr;
  va_start(argptr, sFormat);
  sprintf_s(cString, sFormat, argptr);
  va_end(argptr);
  set(cString);
#elif !defined(FULL_OS_AVAILABLE)
  // Not possible, set string to length 0
  cString[0]='\0';
  CCUNUSED(sFormat);
#else
  va_list argptr;
  va_start(argptr, sFormat);
  ::sprintf(cString, sFormat, argptr);
  va_end(argptr);
#endif
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

CcString& CcString::replace(const CcString& needle, const CcString& replace)
{
  size_t pos=0;
  while (pos < length())
  {
    pos = find(needle, pos);
    if (pos != SIZE_MAX)
    {
      erase(pos, needle.length());
      insert(pos, replace);
      pos += replace.length();
    }
  }
  return *this;
}

CcString CcString::getStringBetween(const CcString& preStr, const CcString& postStr, size_t offset, size_t *pos) const
{
  CcString sRet;
  size_t posFirst = find(preStr.m_pBuffer, preStr.m_uiLength, offset) ;
  if (posFirst != SIZE_MAX)
  {
    posFirst += preStr.length();
    size_t posSecond = find(postStr.m_pBuffer, postStr.m_uiLength, posFirst);
    if (posSecond != SIZE_MAX)
    {
      size_t len = posSecond - posFirst;
      sRet.append(CCMOVE(substr(posFirst, len)));
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

CcString CcString::getLastLine() const
{
  size_t posR = findLast(CcGlobalStrings::EolCr);
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

bool CcString::compare(const char* sToCompare) const
{
  bool bSuccess = true;
  size_t uiCnt;
  for (uiCnt = 0; uiCnt < m_uiLength; uiCnt++)
  {
    if (sToCompare[uiCnt] == 0)
    {
      bSuccess = false;
      break;
    }
    if (sToCompare[uiCnt] != m_pBuffer[uiCnt])
    {
      bSuccess = false;
      break;
    }
  }
  if(uiCnt != m_uiLength)
    bSuccess = false;
  return bSuccess;
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
  return find(sToFind.m_pBuffer, sToFind.m_uiLength, offset);
}

size_t CcString::find(const char* pcString, size_t uiLength, size_t uiOffset) const
{
  size_t uiRet = SIZE_MAX;
  if (uiOffset < m_uiLength)
  {
    for (; uiOffset + uiLength <= m_uiLength && uiRet == SIZE_MAX; uiOffset++)
    {
      size_t uiCntInput = 0;
      for (uiCntInput = 0; uiCntInput < uiLength; uiCntInput++)
      {
        if (m_pBuffer[uiOffset + uiCntInput] != pcString[uiCntInput])
          break;
      }
      if (uiCntInput == uiLength)
        uiRet = uiOffset;
    }
  }
  return uiRet;
}

bool CcString::isStringAtOffset(const CcString& sToCompare, size_t uiOffset, ESensitivity eSensitivity) const
{
  if (sToCompare.m_uiLength + uiOffset > m_uiLength)
  {
    return false;
  }
  else
  {
    if (eSensitivity == ESensitivity::CaseSensitiv)
    {
      for (size_t i = 0; i < sToCompare.length(); i++)
      {
        if (sToCompare.at(i) != at(i + uiOffset))
          return false;
      }
    }
    else
    {
      CcString sStringA((*this).substr(uiOffset, sToCompare.length()));
      CcString sStringB(sToCompare);
      if (sStringA.toLower() != sStringB.toLower())
      {
        return false;
      }
    }
  }
  return true;
}


bool CcString::startsWith(const CcString& sToCompare, ESensitivity eSensitivity) const
{
  return isStringAtOffset(sToCompare, 0, eSensitivity);
}

bool CcString::endsWith(const CcString& sToCompare, ESensitivity eSensitivity) const
{
  bool bEndsWith = false;
  if(m_uiLength >= sToCompare.m_uiLength)
    bEndsWith = isStringAtOffset(sToCompare, m_uiLength - sToCompare.m_uiLength, eSensitivity);
  return bEndsWith;
}

uint64 CcString::toUint64( bool *pbOk, uint8 uiBase) const
{
  uint64 uiRet = 0;
  size_t uiPos = posNextNotWhitespace();
  if (uiPos < length())
  {
    uiRet = CcStringUtil::toUint64(getCharString() + uiPos, length() - uiPos, pbOk, uiBase);
  }
  else if (pbOk != nullptr)
  {
    *pbOk = false;
  }
  return uiRet;
}

uint32 CcString::toUint32( bool *pbOk, uint8 uiBase) const
{
  uint32 uiRet = 0;
  size_t uiPos = posNextNotWhitespace();
  if (uiPos < length())
  {
    uiRet = CcStringUtil::toUint32(getCharString() + uiPos, length() - uiPos, pbOk, uiBase);
  }
  else if (pbOk != nullptr)
  {
    *pbOk = false;
  }
  return uiRet;
}

uint16 CcString::toUint16(bool *pbOk, uint8 uiBase) const
{
  uint32 uiTemp = toUint32(pbOk, uiBase);
  if (uiTemp > UINT16_MAX)
  {
    if (pbOk != nullptr)
    {
      *pbOk = false;
    }
  }
  return static_cast<uint16>(uiTemp);
}

uint8 CcString::toUint8(bool *pbOk, uint8 uiBase) const
{
  uint32 uiTemp = toUint32(pbOk, uiBase);
  if (uiTemp > UINT8_MAX)
  {
    if (pbOk != nullptr)
    {
      *pbOk = false;
    }
  }
  return static_cast<uint8>(uiTemp);
}

int64 CcString::toInt64( bool *pbOk, uint8 uiBase) const
{
  int64 uiRet = 0;
  bool bNeg = false;
  size_t uiPos = posNextNotWhitespace();
  if (uiPos < length() && m_pBuffer[uiPos] == '-')
  {
    bNeg = true;
    uiPos++;
  }
  if (uiPos < length())
  {
    uiRet = CcStringUtil::toUint64(getCharString() + uiPos, length() - uiPos, pbOk, uiBase);
    if (bNeg)
      uiRet = 0 - uiRet;
  }
  else if (pbOk != nullptr)
  {
    *pbOk = false;
  }
  return uiRet;
}

int32 CcString::toInt32(bool *pbOk, uint8 uiBase) const
{
  int32 uiRet = 0;
  bool bNeg = false;
  size_t uiPos = posNextNotWhitespace();
  if (uiPos < length() && m_pBuffer[uiPos] == '-')
  {
    bNeg = true;
    uiPos++;
  }
  if (uiPos < length())
  {
    uiRet = CcStringUtil::toUint32(getCharString() + uiPos, length() - uiPos, pbOk, uiBase);
    if (bNeg)
      uiRet = 0 - uiRet;
  }
  else if (pbOk != nullptr)
  {
    *pbOk = false;
  }
  return uiRet;
}

int16 CcString::toInt16(bool *pbOk, uint8 uiBase) const
{
  return static_cast<int16>(toInt32(pbOk, uiBase));
}

int8 CcString::toInt8(bool *pbOk, uint8 uiBase) const
{
  return static_cast<int8>(toInt32(pbOk, uiBase));
}

float CcString::toFloat(bool* bOk) const
{
  float fRet = 0;
#if defined(WINDOWSKERNEL)
  if (bOk != nullptr)
  {
    *bOk = false;
  }
#elif defined(FULL_OS_AVAILABLE)
  fRet = strtof(m_pBuffer, nullptr);
  if (bOk != nullptr)
  {
    if (errno != ERANGE)
      *bOk = true;
    else
      *bOk = false;
  }
#else
  if (bOk != nullptr)
  {
    *bOk = false;
  }
#endif
  return fRet;
}

double CcString::toDouble(bool* bOk) const
{
  double fRet = 0;
#if defined(WINDOWSKERNEL)
  if (bOk != nullptr)
  {
    *bOk = false;
  }
#elif defined(FULL_OS_AVAILABLE)
  fRet = strtod(m_pBuffer, nullptr);
  if (bOk != nullptr)
  {
    if (errno != ERANGE)
      *bOk = true;
    else
      *bOk = false;
  }
#else
  if (bOk != nullptr)
  {
    *bOk = false;
  }
#endif
  return fRet;
}

bool CcString::toBool(bool* bOk) const
{
  return CcStringUtil::getBoolFromStirng(*this, bOk);
}

CcString& CcString::toUpper()
{
  size_t uiLength = m_uiLength;
  while (uiLength--)
  {
#ifdef FULL_OS_AVAILABLE
    m_pBuffer[uiLength] = (char)::toupper(m_pBuffer[uiLength]);
#else
    for(size_t uiPos = 0; uiPos < uiLength; uiPos++)
    {
      if(m_pBuffer[uiPos] >= 'a' && m_pBuffer[uiPos] <= 'z')
        m_pBuffer[uiPos] = m_pBuffer[uiPos] - ('a' - 'A');
    }
#endif
  }
  return *this;
}

CcString CcString::getUpper() const
{
  return CcString(*this).toUpper();
}

CcString& CcString::toLower()
{
  size_t uiLength = m_uiLength;
  while (uiLength--)
  {
#ifdef FULL_OS_AVAILABLE
    m_pBuffer[uiLength] = (char)::tolower(m_pBuffer[uiLength]);
#else
    for(size_t uiPos = 0; uiPos < uiLength; uiPos++)
    {
      if(m_pBuffer[uiPos] >= 'A' && m_pBuffer[uiPos] <= 'Z')
        m_pBuffer[uiPos] = m_pBuffer[uiPos] + ('a' - 'A');
    }
#endif
  }
  return *this;
}

CcString CcString::getLower() const
{
  return CcString(*this).toLower();
}

CcString& CcString::append(const CcString& toAppend)
{
  return append(toAppend.m_pBuffer, toAppend.m_uiLength);
}

CcString& CcString::append(const char* toAppend)
{
  return append(toAppend, CcStringUtil::strlen(toAppend));
}

CcString& CcString::append(const char toAppend)
{
  return append(&toAppend, 1);
}

CcString& CcString::append(const char *toAppend, size_t length)
{
  allocateBuffer(m_uiLength + length);
  CcStatic::memcpy(m_pBuffer + m_uiLength, toAppend, length);
  m_uiLength += length;
  m_pBuffer[m_uiLength] = 0;
  return *this;
}

CcString& CcString::append(const CcByteArray &toAppend, size_t pos, size_t len)
{
  if (len == SIZE_MAX)
    len = toAppend.size() - pos;
  char* arr = toAppend.getArray(pos);
  return append(arr, len);
}

CcString& CcString::appendWchar(const wchar_t* pStr)
{
  if (pStr != NULL)
  {
    size_t i = 0;
    while (pStr[i] != 0) i++;
    appendWchar(pStr, i);
  }
  return *this;
}

CcString& CcString::appendWchar(const wchar_t toAppend)
{
  return appendWchar(&toAppend, 1);
}

CcString& CcString::appendWchar(const wchar_t* pStr, size_t uiLength)
{
  return append(CcString().fromUnicode(pStr, uiLength));
}

CcString& CcString::appendNumber(uint8 number, uint8 uiBase)
{
  return append(CcStringUtil::fromUint32(static_cast<uint32>(number), uiBase));
}

CcString& CcString::appendNumber(int8 number, uint8 uiBase)
{
  return append(CcStringUtil::fromInt32(static_cast<int32>(number), uiBase));
}

CcString& CcString::appendNumber(uint16 number, uint8 uiBase)
{
  return append(CcStringUtil::fromUint32(static_cast<uint32>(number), uiBase));
}

CcString& CcString::appendNumber(int16 number, uint8 uiBase)
{
  return append(CcStringUtil::fromInt32(static_cast<int32>(number), uiBase));
}

CcString& CcString::appendNumber(uint32 number, uint8 uiBase)
{
  return append(CcStringUtil::fromUint32(number, uiBase));
}

CcString& CcString::appendNumber(int32 number, uint8 uiBase)
{
  return append(CcStringUtil::fromInt32(number, uiBase));
}

CcString& CcString::appendNumber(uint64 number, uint8 uiBase)
{
  return append(CcStringUtil::fromUint64(number, uiBase));
}

CcString& CcString::appendNumber(int64 number, uint8 uiBase)
{
  return append(CcStringUtil::fromInt64(number, uiBase));
}

CcString& CcString::appendNumber(float number, uint8 uiPrecision, bool bDisableExponent)
{
#ifndef FULL_OS_AVAILABLE
  return append(CcStringUtil::fromFloat(number, uiPrecision, bDisableExponent));
#else
  std::ostringstream os;
  if (bDisableExponent)
    os << std::fixed << std::setprecision(uiPrecision) << number;
  else
    os << std::setprecision(uiPrecision) << number;
  append(os.str().c_str());
  if (number == 0)
  {
    append(".0");
  }
  return *this;
#endif
}

CcString& CcString::appendNumber(double number, uint8 uiPrecision, bool bDisableExponent)
{
#ifndef FULL_OS_AVAILABLE
  return append(CcStringUtil::fromDouble(number, uiPrecision, bDisableExponent));
#else
  std::ostringstream os;
  if(bDisableExponent)
    os << std::fixed << std::setprecision(uiPrecision) << number;
  else
    os << std::setprecision(uiPrecision) << number;
  append(os.str().c_str());
  if (number == 0)
  {
    append(".0");
  }
  return *this;
#endif
}

CcString& CcString::appendSize(size_t number, uint8 uiBase)
{
  return append(CcStringUtil::fromUint64(number, uiBase));
}

CcString& CcString::appendInt(int number, uint8 uiBase)
{
  return append(CcStringUtil::fromInt64(number, uiBase));
}

CcString& CcString::appendUint(uint number, uint8 uiBase)
{
  return append(CcStringUtil::fromUint64(number, uiBase));
}

CcString& CcString::set(const CcString& toSet)
{
  clear();
  return append(toSet);
}

CcString& CcString::set(const char* toSet)
{
  clear();
  return append(toSet);
}

CcString& CcString::set(const char toSet)
{
  clear();
  return append(toSet);
}

CcString& CcString::set(const char *toSet, size_t length)
{
  clear();
  return append(toSet, length);
}

CcString& CcString::set(const CcByteArray &toSet, size_t pos, size_t len)
{
  clear();
  return append(toSet, pos, len);
}

CcString& CcString::setWchar(const wchar_t* pStr)
{
  clear();
  return appendWchar(pStr);
}

CcString& CcString::setWchar(const wchar_t toSet)
{
  clear();
  return appendWchar(toSet);
}

CcString& CcString::setWchar(const wchar_t* pStr, size_t uiLength)
{
  clear();
  return appendWchar(pStr, uiLength);
}

CcString& CcString::setNumber(uint8 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(int8 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(uint16 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(int16 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(uint32 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(int32 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(uint64 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(int64 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcString& CcString::setNumber(float number)
{
  clear();
  return appendNumber(number);
}

CcString& CcString::setNumber(double number)
{
  clear();
  return appendNumber(number);
}

CcString& CcString::setSize(size_t number, uint8 uiBase)
{
  clear();
  return appendSize(number, uiBase);
}

CcString& CcString::setInt(int number, uint8 uiBase)
{
  clear();
  return appendInt(number, uiBase);
}

CcString& CcString::setUint(uint number, uint8 uiBase)
{
  clear();
  return appendUint(number, uiBase);
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

CcByteArray CcString::getByteArray() const
{
  CcByteArray ca(getCharString(), length());
  return ca;
}

CcString CcString::getOsPath() const
{
#ifdef WINDOWS
  return getReplace(CcGlobalStrings::Seperators::Path, CcGlobalStrings::Seperators::BackSlash);
#else
  return *this;
#endif
}

CcStringList CcString::split(const CcString& delimiter, bool bKeepEmpty) const
{
  CcStringList slRet;
  size_t save = 0;
  size_t offset = find(delimiter);
  while (offset != SIZE_MAX)
  {
    CcString sValue = substr(save, offset - save);
    if (bKeepEmpty == true || sValue.length() > 0)
    {
      slRet.append(CCMOVE(sValue));
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

CcStringList CcString::splitLines(bool bKeepEmptyLines) const
{
  CcStringList slRet;
  size_t save = 0;
  size_t offsetN;
  size_t offsetR;
  do
  {
    offsetN = find(CcGlobalStrings::EolShort, save);
    offsetR = find(CcGlobalStrings::EolCr, save);
    if (offsetR < offsetN)
    {
      size_t uiLength = offsetR - save;
      if (bKeepEmptyLines || uiLength > 0)
      {
        slRet.append(substr(save, uiLength));
      }
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
      size_t uiLength = offsetN - save - 1;
      if (bKeepEmptyLines || uiLength > 0)
      {
        slRet.append(substr(save, uiLength));
      }
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
    else if (cString[i] < 0xc0)
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

CcString& CcString::fromUtf16(const uint16* cString, size_t uiLength)
{
  clear();
  for (size_t i = 0; i < uiLength; i++)
  {
    if (cString[i] < 0x80)
    {
      append(static_cast<uchar>(cString[i]));
    }
    else if (cString[i] < 0xc0)
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

CcString CcString::fromNumber(float number, uint8 uiPrecision, bool bDisableExponent)
{
  CcString sRet;
  sRet.appendNumber(number, uiPrecision, bDisableExponent);
  return sRet;
}

CcString CcString::fromNumber(double number, uint8 uiPrecision, bool bDisableExponent)
{
  CcString sRet;
  sRet.appendNumber(number, uiPrecision, bDisableExponent);
  return sRet;
}

CcString CcString::fromSize(size_t number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendSize(number, uiBase);
  return sRet;
}

CcString CcString::fromInt(int number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendInt(number, uiBase);
  return sRet;
}

CcString CcString::fromUint(uint number, uint8 uiBase)
{
  CcString sRet;
  sRet.appendUint(number, uiBase);
  return sRet;
}

CcString& CcString::normalizePath()
{
  replace(CcGlobalStrings::Seperators::BackSlash, CcGlobalStrings::Seperators::Path);
  replace(CcGlobalStrings::Seperators::DoubleSlashes, CcGlobalStrings::Seperators::Path);
  replace("/./", CcGlobalStrings::Seperators::Path);
  while(startsWith("./")) remove(0, 2);
  if (contains("/../") ||
      startsWith("../") ||
      endsWith("/..") )
  {
    bool bStartedWithSlash = startsWith(CcGlobalStrings::Seperators::Path);
    CcStringList slPath = split(CcGlobalStrings::Seperators::Path);
    for (size_t i = 0; i < slPath.size(); i++)
    {
      if (slPath[i] == "")
      {
        slPath.remove(i);
        i--;
      }
      else if (slPath[i] == "..")
      {
        if (i > 0)
        {
          slPath.remove(i);
          i--;
          slPath.remove(i);
          i--;
        }
      }
    }
    if (!bStartedWithSlash) clear();
    else                    set(CcGlobalStrings::Seperators::Path);
    append(slPath.collapse(CcGlobalStrings::Seperators::Path));
  }
  return *this;
}

CcString CcString::extractFilename() const
{
  CcString sRet;
  size_t pos1 = findLast(CcGlobalStrings::Seperators::Path);
  size_t pos2 = findLast(CcGlobalStrings::Seperators::BackSlash);
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
    else
    {
      subStrPos = pos1;
    }
  }
  else
  {
    subStrPos = pos2;
  }
  if (subStrPos != SIZE_MAX)
    sRet = substr(subStrPos);
  else
    sRet = *this;
  return sRet;
}

CcString CcString::extractPath()
{
  CcString sRet;
  size_t pos1 = findLast(CcGlobalStrings::Seperators::Path);
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

CcString &CcString::appendPath(const CcString& sToAppend)
{
  if (sToAppend.endsWith(CcGlobalStrings::Seperators::Path))
  {
    if (sToAppend.length() > 1)
    {
      appendPath(sToAppend.substr(0, sToAppend.length() - 1));
    }
  }
  else
  {
    CcString seperator;
    if (sToAppend.length() > 0)
    {
      if (length() > 0 &&
        at(length() - 1) != CcGlobalStrings::Seperators::Path[0])
      {
        seperator = CcGlobalStrings::Seperators::Path;
      }
      if (sToAppend.startsWith(CcGlobalStrings::Seperators::Path))
      {
        if (seperator.length() == 0)
        {
          append(sToAppend.substr(1));
        }
        else
        {
          append(sToAppend);
        }
      }
      else
      {
        if (sToAppend.startsWith("./"))
        {
          append(seperator);
          append(sToAppend.substr(2));
        }
        else
        {
          append(seperator);
          append(sToAppend);
        }
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

CcString& CcString::trimL()
{
  while ( 0 != length() &&
          CcStringUtil::isWhiteSpace(at(0)))
  {
    remove(0);
  }
  return *this;
}

CcString& CcString::trimR()
{
  size_t pos = length() - 1;
  while (pos < length() &&
          CcStringUtil::isWhiteSpace(at(pos))
        )
  {
    remove(pos);
    pos--;
  }
  return *this;
}

void CcString::transfer(char* pData, size_t uiCount)
{
  clear();
  m_pBuffer = pData;
  m_uiLength = uiCount;
  m_uiReserved = uiCount;
}

void CcString::extract(char*& pData, size_t& uiCount, size_t& uiReserved)
{
  pData = m_pBuffer;
  uiCount = m_uiLength;
  uiReserved = m_uiReserved;
  m_pBuffer = nullptr;
  m_uiLength = 0;
  m_uiReserved = 0;
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

CcString& CcString::setOsPath(const CcString&  sPathToSet)
{
  return set(sPathToSet.getReplace(CcGlobalStrings::Seperators::BackSlash, CcGlobalStrings::Seperators::Slash).getCharString());
}

CcString& CcString::appendIp(const CcIp& oAddr)
{
  append(oAddr.getString());
  return *this;
}

CcString& CcString::insert(size_t pos, const char* pcToInsert, size_t uiLength)
{
  allocateBuffer(m_uiLength + uiLength);
  size_t uiNewEnd = m_uiLength + uiLength;
  for (size_t uiCnt = 0; uiCnt <= m_uiLength-pos; uiCnt++)
  {
    m_pBuffer[uiNewEnd - uiCnt] = m_pBuffer[m_uiLength - uiCnt];
  }
  CcStatic::memcpy(m_pBuffer + pos, pcToInsert, uiLength);
  m_uiLength += uiLength;
  return *this;
}

CcString& CcString::insert(size_t pos, const CcString& toInsert)
{
   return insert(pos, toInsert.getCharString(), toInsert.length());
}

void CcString::mlock()
{
  if(m_pBuffer != nullptr)
  {
    CcStatic::mlock(m_pBuffer, m_uiReserved);
  }
}

void CcString::munlock()
{
  if(m_pBuffer != nullptr)
  {
    CcStatic::munlock(m_pBuffer, m_uiReserved);
  }
}

void CcString::clear()
{
  allocateBuffer(0);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

void CcString::clearSave()
{
  CcStatic::memset(m_pBuffer, 0, m_uiReserved);
  allocateBuffer(0);
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

CcString& CcString::operator=(CcString&& oToMove) NOEXCEPT
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

CcString& CcString::operator=(const CcString& sToCopy)
{
  if (this != &sToCopy)
  {
    set(sToCopy);
  }
  return *this;
}

CcString& CcString::operator=(CcByteArray&& oToMove) NOEXCEPT
{
  deleteBuffer();
  oToMove.extract(m_pBuffer, m_uiLength);
  m_uiReserved = m_uiLength;
  return *this;
}

void CcString::allocateBuffer(size_t uiSize)
{
  if(uiSize == 0)
  {
    deleteBuffer();
    CCNEWARRAY(m_pBuffer, char, c_uiDefaultMultiplier);
    m_pBuffer[0] = 0;
    m_uiLength = 0;
    m_uiReserved = c_uiDefaultMultiplier;
  }
  else if (uiSize + 1 > m_uiReserved)
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
    CCNEWARRAYTYPE(pBuffer, char, uiNewLen);
    size_t uiOldLen = m_uiLength;
    CcStatic::memcpy(pBuffer, m_pBuffer, sizeof(char)*m_uiLength);
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
  if(m_uiReserved)
  {
    CCDELETEARR(m_pBuffer);
  }
  m_uiLength = 0;
  m_uiReserved = 0;
}
