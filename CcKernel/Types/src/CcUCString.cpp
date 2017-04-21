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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcUCString
 */
#include "CcUCString.h"
#include <cstring>

const size_t c_uiDefaultMultiplier = 16;

CcUCString::CcUCString( void )
{
  reserve(c_uiDefaultMultiplier);
}

CcUCString::CcUCString( const CcUCString& oToCopy )
{
  operator=(oToCopy);
}

CcUCString::CcUCString( CcUCString&& oToMove )
{
  operator=(std::move(oToMove));
}

CcUCString::CcUCString(const CcString& sString)
{
  fromString(sString);
}

CcUCString::CcUCString(const wchar_t* wcString, size_t uiLength)
{
  append(wcString, uiLength);
}

CcUCString::CcUCString(size_t uiLength, wchar_t wcInitValue)
{
  reserveByLength(uiLength);
  m_uiLength = uiLength;
  while ((--uiLength) > 0) m_pBuffer[uiLength] = wcInitValue;
}

CcUCString::~CcUCString( void )
{
  if (m_pBuffer != nullptr)
  {
    delete m_pBuffer;
    m_pBuffer = nullptr;
  }
}

CcUCString& CcUCString::operator=(const CcUCString& oToCopy)
{
  if (m_pBuffer != nullptr)
    delete m_pBuffer;

  m_uiReserved = oToCopy.m_uiReserved;
  m_pBuffer = new wchar_t[oToCopy.m_uiReserved];

  append(oToCopy.getCharString(), oToCopy.length());
  return *this;
}

CcUCString& CcUCString::operator=(CcUCString&& oToMove)
{
  if(this != &oToMove)
  {
    if (m_pBuffer != nullptr)
      delete m_pBuffer;
    m_pBuffer     = oToMove.m_pBuffer;
    m_uiReserved  = oToMove.m_uiReserved;
    m_uiLength    = oToMove.m_uiLength;
    oToMove.m_pBuffer = nullptr;
    oToMove.m_uiReserved = 0;
    oToMove.m_uiLength = 0;
  }
  return *this;
}

bool CcUCString::operator==(const CcUCString& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcUCString::operator!=(const CcUCString& oToCompare) const
{
  return !operator==(oToCompare);
}

void CcUCString::clear()
{
  reserve(c_uiDefaultMultiplier);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

void CcUCString::clearAndReserve(size_t uiLength)
{
  reserveByLength(uiLength);
  m_uiLength = 0;
  m_pBuffer[m_uiLength] = 0;
}

void CcUCString::resize(size_t uiLength)
{
  reserveByLength(uiLength);
  m_uiLength = uiLength;
  m_pBuffer[m_uiLength] = 0;
}

CcUCString& CcUCString::append(wchar_t wcSingle)
{
  reserveByLength(m_uiLength + 1);
  m_pBuffer[m_uiLength] = wcSingle;
  m_uiLength++;
  m_pBuffer[m_uiLength] = 0;
  return *this;
}

CcUCString& CcUCString::append(const wchar_t* wcString, size_t uiLength)
{
  reserveByLength(m_uiLength + uiLength);
  for (size_t i = 0; i < uiLength; i++)
  {
    m_pBuffer[m_uiLength] = wcString[i];
    m_uiLength++;
  }
  m_pBuffer[m_uiLength] = 0;
  return *this;
}

CcUCString& CcUCString::fromString(const CcString& oString)
{
  reserveByLength(oString.length());
  for (size_t i = 0; i < oString.length(); i++)
  {
    if (static_cast<uchar>(oString.at(i)) < 0x80)
    {
      append(oString.at(i));
    }
    else if (static_cast<uchar>(oString.at(i)) < 0xc3 && i + 1 < oString.length())
    {
      append(oString.at(i + 1));
      i++;
    }
    else if (static_cast<uchar>(oString.at(i)) < 0xe0 && i + 1 < oString.length())
    {
      uint8 uiTemp = static_cast<uchar>(oString.at(i)) - 0xc0;
      wchar_t uiValue = static_cast<uchar>(oString.at(i + 1) & 0x3f) + (uiTemp << 6);
      uiValue |= (uiTemp >> 2) << 8;
      append(uiValue);
      i++;
    }
    else if (i + 2 < oString.length())
    {
      uint8 uiMasterTemp = static_cast<uchar>(oString.at(i)) - 0xe0;
      uint8 uiAddition = static_cast<uchar>(oString.at(i + 1)) - 0x80;
      wchar_t uiValue = static_cast<uchar>(oString.at(i + 2) & 0x3f) + (uiAddition << 6);
      uiValue |= ((uiAddition >> 2) + (uiMasterTemp << 4)) << 8;
      append(uiValue);
      i += 2;
    }
  }
  return *this;
}

CcString CcUCString::getString() const
{
  CcString sRet(m_pBuffer, m_uiLength);
  return sRet;
}

void CcUCString::reserve(size_t uiSize)
{
  if (m_uiReserved == 0)
  {
    m_pBuffer = new wchar_t[uiSize];
    m_uiLength = 0;
    m_uiReserved = uiSize;
  }
  else if (uiSize > m_uiReserved)
  {
    wchar_t* pBuffer = new wchar_t[uiSize];
    m_uiReserved = uiSize;
    memcpy(pBuffer, m_pBuffer, sizeof(wchar_t)*m_uiLength);
    pBuffer[m_uiLength] = 0;
    if (m_pBuffer != nullptr)
    {
      delete m_pBuffer;
    }
    m_pBuffer = pBuffer;
  }
  else
  {
    // keep reserved size.
  }
}

void CcUCString::reserveByLength(size_t uiLength)
{
  if (uiLength + 1 > m_uiReserved)
  {
    size_t uiNewLen = uiLength + 1;
    size_t uiMultiplier = uiNewLen / c_uiDefaultMultiplier;
    if ((uiNewLen % c_uiDefaultMultiplier) > 0)
    {
      uiMultiplier++;
    }
    reserve(uiMultiplier * c_uiDefaultMultiplier);
  }
}

