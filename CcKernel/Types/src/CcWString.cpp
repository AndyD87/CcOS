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
#include <sstream>
#include <iomanip>
#include <cstdarg>
#include <limits>
#include "CcStringUtil.h"

const size_t c_uiDefaultMultiplier = 16;

CcWString::CcWString(void )
{
  reserve(c_uiDefaultMultiplier);
}

CcWString::CcWString( const CcWString& oToCopy )
{
  operator=(oToCopy);
}

CcWString::CcWString( CcWString&& oToMove )
{
  operator=(std::move(oToMove));
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
  reserve(uiLength);
  m_uiLength = uiLength;
  while ((--uiLength) > 0) m_pBuffer[uiLength] = wcInitValue;
}

CcWString::~CcWString(void )
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
  m_pBuffer = new wchar_t[oToCopy.m_uiReserved];
  CCMONITORNEW(m_pBuffer);

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
  reserve(0);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

void CcWString::clearAndReserve(size_t uiLength)
{
  reserve(uiLength);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

void CcWString::resize(size_t uiLength)
{
  reserve(uiLength);
  m_uiLength = uiLength;
  m_pBuffer[m_uiLength] = 0;
}

CcWString& CcWString::append(const CcWString& sString)
{
  return append(sString.getWcharString(), sString.length());
}

CcWString& CcWString::append(wchar_t wcSingle)
{
  reserve(m_uiLength + 1);
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
  reserve(m_uiLength + uiLength);
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
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << (uint)number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring((uint)number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(int8 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << (int)number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring((int)number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(uint16 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring(number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(int16 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring(number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(uint32 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring(number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(int32 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring(number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(uint64 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring(number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(int64 number, uint8 uiBase)
{
  std::wstringstream stream;
  std::wstring sTemp;
  switch (uiBase)
  {
  case 16:
    stream << std::hex << number;
    sTemp = stream.str();
    break;
  default:
    sTemp = std::to_wstring(number);
    break;
  }
  append(sTemp.c_str(), sTemp.length());
  return *this;
}

CcWString& CcWString::appendNumber(float number)
{
  std::wostringstream os;
  os << std::setprecision(std::numeric_limits<unsigned>::digits10 + 1) << number;
  append(os.str().c_str());
  if (number == 0)
  {
    append(L".0");
  }
  return *this;
}

CcWString& CcWString::appendNumber(double number)
{
  std::wostringstream os;
  os << std::setprecision(std::numeric_limits<unsigned>::digits10 + 1) << number;
  append(os.str().c_str());
  if (number == 0)
  {
    append(L".0");
  }
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

CcWString& CcWString::fromString(const char* pcString, size_t uiLength)
{
  reserve(uiLength);
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

void CcWString::reserve(size_t uiSize)
{
  if (uiSize + 1 > m_uiReserved)
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
    wchar_t* pBuffer = new wchar_t[uiNewLen];
    CCMONITORNEW(pBuffer);
    m_uiReserved = uiNewLen;
    CcStatic::memcpy(pBuffer, m_pBuffer, sizeof(wchar_t)*m_uiLength);
    pBuffer[m_uiLength] = 0;
    deleteBuffer();
    m_pBuffer = pBuffer;
  }
  else
  {
    // keep reserved size.
  }
}

void CcWString::deleteBuffer()
{
  CCDELETE(m_pBuffer);
}
