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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcCrc32
 */
#include "Hash/CcCrc32.h"
#include "CcByteArray.h"
#include "CcString.h"


uint32 CcCrc32::s_puiLookUp[256] = {0};
bool CcCrc32::s_bLookupDone = false;

CcCrc32::CcCrc32()
{
  if (s_bLookupDone == false)
    computeLookup();
}

CcCrc32& CcCrc32::append(const CcByteArray& oByteArray)
{
  return append(oByteArray.getArray(), oByteArray.size());
}

CcCrc32& CcCrc32::append(const char *pData, size_t uiSize)
{
  for (size_t i = 0; i < uiSize; i++)
  {
    m_uiCrcValue = (m_uiCrcValue >> 8) ^ (size_t) s_puiLookUp[(uint8) (m_uiCrcValue & 0xFF) ^ (uint8) pData[i]];
  }
  return *this;
}


void CcCrc32::computeLookup()
{
  for (uint16 i = 0; i <= 0xFF; i++)
  {
    uint32_t crc = i;
    for (uint8 j = 0; j < 8; j++)
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320L);
    s_puiLookUp[i] = crc;
  }
  s_bLookupDone = true;
}