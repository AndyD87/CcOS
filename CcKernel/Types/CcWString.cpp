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
 * @brief     Implemtation of class CcWString
 */
#include "CcWString.h"
#include "CcString.h"
#include "CcStatic.h"
#include "CcStringUtil.h"
#ifdef FULL_OS_AVAILABLE
  #include <cctype>
#endif

const size_t c_uiDefaultMultiplier = 16;

CcWString::CcWString()
{
}

CcWString::CcWString( const CcWString& oToCopy )
{
  operator=(oToCopy);
}

CcWString::CcWString( CcWString&& oToMove )
{
  operator=(CCMOVE(oToMove));
}

CcWString::CcWString(const CcString& sString)
{
  fromString(sString);
}

CcWString::CcWString(const char* pcString, size_t uiLength)
{
  fromString(pcString, uiLength);
}

CcWString::CcWString(const wchar_t* wcString)
{
  size_t uiLength = CcStringUtil::strlen(wcString);
  if (uiLength != SIZE_MAX)
  {
    append(wcString, uiLength);
  }
}

CcWString::CcWString(const wchar_t* wcString, size_t uiLength)
{
  append(wcString, uiLength);
}

CcWString::CcWString(size_t uiLength, wchar_t wcInitValue)
{
  allocateBuffer(uiLength);
  m_uiLength = uiLength;
  while ((--uiLength) > 0) m_pBuffer[uiLength] = wcInitValue;
}

CcWString::~CcWString()
{
  deleteBuffer();
}

CcWString& CcWString::operator=(CcWString&& oToMove)
{
  if(this != &oToMove)
  {
    CCDELETE(m_pBuffer);
    m_pBuffer     = oToMove.m_pBuffer;
    m_uiReserved  = oToMove.m_uiReserved;
    m_uiLength    = oToMove.m_uiLength;
    oToMove.m_pBuffer = nullptr;
    oToMove.m_uiReserved = 0;
    oToMove.m_uiLength = 0;
  }
  return *this;
}

CcWString& CcWString::operator=(const CcWString& oToCopy)
{
  clear();
  m_uiReserved = oToCopy.m_uiReserved;
  CCNEWARRAY(m_pBuffer, wchar_t, oToCopy.m_uiReserved);

  append(oToCopy.getWcharString(), oToCopy.length());
  return *this;
}

bool CcWString::operator==(const CcWString& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcWString::operator!=(const CcWString& oToCompare) const
{
  return !operator==(oToCompare);
}

void CcWString::clear()
{
  allocateBuffer(0);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

CcWString& CcWString::remove(size_t uiPos, size_t uiLength)
{
  if (uiPos < m_uiLength)
  {
    if (uiLength > m_uiLength - uiPos)
      uiLength = m_uiLength - uiPos;
    size_t uiBeginLast = uiPos + uiLength;
    size_t uiLengthLast = m_uiLength - uiBeginLast;
    for (size_t i = 0; i < uiLengthLast; i++)
      m_pBuffer[uiPos + i] = m_pBuffer[uiBeginLast + i];
    m_uiLength -= uiLength;
    m_pBuffer[m_uiLength] = 0;
  }
  return *this;
}

CcWString& CcWString::append(const CcWString& sString)
{
  return append(sString.getWcharString(), sString.length());
}

CcWString& CcWString::append(wchar_t wcSingle)
{
  allocateBuffer(m_uiLength + 1);
  m_pBuffer[m_uiLength] = wcSingle;
  m_uiLength++;
  m_pBuffer[m_uiLength] = 0;
  return *this;
}

CcWString& CcWString::append(const wchar_t* wcString)
{
  size_t i = 0;
  while (wcString[i] != 0) i++;
  return append(wcString, i);
}

CcWString& CcWString::append(const wchar_t* wcString, size_t uiLength)
{
  allocateBuffer(m_uiLength + uiLength);
  for (size_t i = 0; i < uiLength; i++)
  {
    m_pBuffer[m_uiLength] = wcString[i];
    m_uiLength++;
  }
  m_pBuffer[m_uiLength] = 0;
  return *this;
}

CcWString& CcWString::appendNumber(uint8 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(int8 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(uint16 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(int16 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(uint32 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(int32 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(uint64 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(int64 number, uint8 uiBase)
{
  CcString sString;
  sString.appendNumber(number, uiBase);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(float number)
{
  CcString sString;
  sString.appendNumber(number);
  append(sString);
  return *this;
}

CcWString& CcWString::appendNumber(double number)
{
  CcString sString;
  sString.appendNumber(number);
  append(sString);
  return *this;
}

CcWString& CcWString::set(const CcWString& sString)
{
  clear();
  return append(sString);
}

CcWString& CcWString::set(wchar_t wcSingle)
{
  clear();
  return append(wcSingle);
}

CcWString& CcWString::set(const wchar_t* wcString)
{
  clear();
  return append(wcString);
}

CcWString& CcWString::set(const wchar_t* wcString, size_t uiLength)
{
  clear();
  return append(wcString, uiLength);
}

CcWString& CcWString::setNumber(uint8 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(int8 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(uint16 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(int16 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(uint32 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(int32 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(uint64 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(int64 number, uint8 uiBase)
{
  clear();
  return appendNumber(number, uiBase);
}

CcWString& CcWString::setNumber(float number)
{
  clear();
  return appendNumber(number);
}

CcWString& CcWString::setNumber(double number)
{
  clear();
  return appendNumber(number);
}

CcWString& CcWString::insert(size_t pos, const wchar_t* pcToInsert, size_t uiLength)
{
  allocateBuffer(m_uiLength + uiLength);
  size_t uiNewEnd = m_uiLength + uiLength;
  for (size_t uiCnt = 0; uiCnt <= m_uiLength - pos; uiCnt++)
  {
    m_pBuffer[uiNewEnd - uiCnt] = m_pBuffer[m_uiLength - uiCnt];
  }
  CcStatic::memcpy(m_pBuffer + pos, pcToInsert, uiLength);
  m_uiLength += uiLength;
  return *this;
}

CcWString& CcWString::insert(size_t pos, const CcWString& toInsert)
{
  return insert(pos, toInsert.getWcharString(), toInsert.length());
}

bool CcWString::isStringAtOffset(const CcWString& sToCompare, size_t uiOffset, ESensitivity eSensitivity) const
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
      CcWString sStringA((*this).substr(uiOffset, sToCompare.length()));
      CcWString sStringB(sToCompare);
      if (sStringA.toLower() != sStringB.toLower())
      {
        return false;
      }
    }
  }
  return true;
}

bool CcWString::startsWith(const CcWString& sToCompare, ESensitivity eSensitivity) const
{
  return isStringAtOffset(sToCompare, 0, eSensitivity);
}

bool CcWString::endsWith(const CcWString& sToCompare, ESensitivity eSensitivity) const
{
  bool bEndsWith = false;
  if (m_uiLength >= sToCompare.m_uiLength)
    bEndsWith = isStringAtOffset(sToCompare, m_uiLength - sToCompare.m_uiLength, eSensitivity);
  return bEndsWith;
}

CcWString &CcWString::erase(size_t pos, size_t len)
{
  if (len + pos > m_uiLength)
    len = m_uiLength - pos;
  remove(pos, len);
  return *this;
}

size_t CcWString::find(const CcWString& sToFind, size_t offset) const
{
  return find(sToFind.m_pBuffer, sToFind.m_uiLength, offset);
}

size_t CcWString::find(const wchar_t* pcString, size_t uiLength, size_t uiOffset) const
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

CcWString CcWString::substr(size_t pos, size_t len) const
{
  CcWString sRet;
  if (pos < m_uiLength)
  {
    if (len > m_uiLength ||
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

CcWString& CcWString::toLower()
{
  size_t uiLength = m_uiLength;
  while (uiLength--)
  {
#ifdef FULL_OS_AVAILABLE
    m_pBuffer[uiLength] = (wchar_t)::tolower(m_pBuffer[uiLength]);
#else
    for (size_t uiPos = 0; uiPos < uiLength; uiPos++)
    {
      if (m_pBuffer[uiPos] >= 'A' && m_pBuffer[uiPos] <= 'Z')
        m_pBuffer[uiPos] = m_pBuffer[uiPos] + ('a' - 'A');
    }
#endif
  }
  return *this;
}

CcWString CcWString::getLower() const
{
  return CcWString(*this).toLower();
}

CcWString& CcWString::replace(const CcWString& needle, const CcWString& replace)
{
  size_t pos = 0;
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

CcWString& CcWString::fromString(const char* pcString, size_t uiLength)
{
  allocateBuffer(uiLength);
  for (size_t i = 0; i < uiLength; i++)
  {
    if (static_cast<uchar>(pcString[i]) < 0x80)
    {
      append(pcString[i]);
    }
    else if (static_cast<uchar>(pcString[i]) < 0xc3 && i + 1 < uiLength)
    {
      append(pcString[i+1]);
      i++;
    }
    else if (static_cast<uchar>(pcString[i]) < 0xe0 && i + 1 < uiLength)
    {
      uint8 uiTemp = static_cast<uchar>(pcString[i]) - 0xc0;
      wchar_t uiValue = static_cast<uchar>(pcString[i+1] & 0x3f) + (uiTemp << 6);
      uiValue |= (uiTemp >> 2) << 8;
      append(uiValue);
      i++;
    }
    else if (i + 2 < uiLength)
    {
      uint8 uiMasterTemp = static_cast<uchar>(pcString[i]) - 0xe0;
      uint8 uiAddition = static_cast<uchar>(pcString[i+1]) - 0x80;
      wchar_t uiValue = static_cast<uchar>(pcString[i+2] & 0x3f) + (uiAddition << 6);
      uiValue |= ((uiAddition >> 2) + (uiMasterTemp << 4)) << 8;
      append(uiValue);
      i += 2;
    }
  }
  return *this;
}

CcWString& CcWString::fromString(const CcString& oString)
{
  return fromString(oString.getCharString(), oString.length());
}

CcString CcWString::getString() const
{
  CcString sRet(m_pBuffer, m_uiLength);
  return sRet;
}

void CcWString::reserve(size_t uiLength)
{
  allocateBuffer(uiLength);
  m_uiLength = uiLength;
  m_pBuffer[uiLength] = 0;
}

void CcWString::reserve(size_t uiLength, const wchar_t cDefaultChar)
{
  size_t uiLastLength = m_uiLength;
  allocateBuffer(uiLength);
  m_uiLength = uiLength;
  if (m_uiLength > uiLastLength)
  {
    CcStatic::memset(m_pBuffer + uiLastLength, cDefaultChar, m_uiLength - uiLastLength);
  }
  m_pBuffer[m_uiLength] = 0;
}

void CcWString::allocateBuffer(size_t uiSize)
{
  if (uiSize == 0)
  {
    deleteBuffer();
    CCNEWARRAY(m_pBuffer, wchar_t, c_uiDefaultMultiplier);
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
      uiNewLen = c_uiDefaultMultiplier * (uiMultiplier + 1);
    }
    else
    {
      uiNewLen = c_uiDefaultMultiplier * uiMultiplier;
    }
    CCNEWARRAYTYPE(pBuffer, wchar_t, uiNewLen);
    size_t uiOldLen = m_uiLength;
    CcStatic::memcpy(pBuffer, m_pBuffer, sizeof(wchar_t)*m_uiLength);
    deleteBuffer();
    m_pBuffer = pBuffer;

    m_uiLength = uiOldLen;
    m_uiReserved = uiNewLen;
    m_pBuffer[m_uiLength] = 0;
  }
  else
  {
    // keep reserved size.
  }
}

void CcWString::deleteBuffer()
{
  CCDELETEARR(m_pBuffer);
  m_uiLength = 0;
  m_uiReserved = 0;
}
