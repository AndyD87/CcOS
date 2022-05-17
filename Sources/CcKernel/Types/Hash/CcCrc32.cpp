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
 * @brief     Class CcCrc32
 */
#include "Hash/CcCrc32.h"
#include "CcByteArray.h"
#include "CcString.h"


uint32 CcCrc32::s_puiLookUp[256] = {0};
bool CcCrc32::s_bLookupDone = false;

CcCrc32::CcCrc32() :
  m_oCrcValue(static_cast<size_t>(4))
{
  castUint32() = ~(static_cast<uint32>(0));
  if (s_bLookupDone == false)
    computeLookup();
}

CcCrc32& CcCrc32::generate(const void* pData, size_t uiSize)
{
  return append(pData, uiSize);
}

CcCrc32& CcCrc32::append(const void *pData, size_t uiSize)
{
  for (size_t i = 0; i < uiSize; i++)
  {
    castUint32() = (castUint32() >> 8) ^ (size_t) s_puiLookUp[(uint8) (castUint32() & 0xFF) ^ (uint8) static_cast<const char*>(pData)[i]];
  }
  return *this;
}

CcCrc32& CcCrc32::finalize(const void* pData, size_t uiSize)
{
  return append(pData, uiSize);
}

void CcCrc32::computeLookup()
{
  for (uint16 i = 0; i <= 0xFF; i++)
  {
    uint32 crc = i;
    for (uint8 j = 0; j < 8; j++)
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320L);
    s_puiLookUp[i] = crc;
  }
  s_bLookupDone = true;
}