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
 * @brief     Class CcFontTableLoca
 */

#include "CcFontTableLoca.h"

uint32 CcFontTableLoca::getGlyphOffset(uint16 uiSign, int32 iIndexToLocFormat)
{
  uint32 uiGlyphOffset = 0;
/*bool readValues(CcFontFile& oFile, qint64 iSeek, uint16 uiIndex, int16 iIndexToLocFormat)*/
  if(iIndexToLocFormat == 1)
  {
    uint32* pData = CCVOIDPTRCAST(uint32*,CCVOIDPTRCAST(char*, this) + (uiSign*sizeof(uint32)));
    uiGlyphOffset = CcStatic::swapUint32(*pData);
  }
  else
  {
    uint16* pData = CCVOIDPTRCAST(uint16*,CCVOIDPTRCAST(char*, this) + (uiSign*sizeof(uint16)));
    uiGlyphOffset = static_cast<uint32>(CcStatic::swapUint16((*pData))) * 2;
  }
  return uiGlyphOffset;
}
