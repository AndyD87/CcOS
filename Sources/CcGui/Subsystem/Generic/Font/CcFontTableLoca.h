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
#pragma once

#include "CcBase.h"
#include "CcStatic.h"

#pragma pack(push, 4)

/**
 * @brief Table with locale information. Currently not in use
 */
class CcFontTableLoca
{
public:
  //! Get offsets of 16 or 32 bit formats.
  union
  {
    uint16 ui16[1];
    uint32 ui32[1];
  } Offsets;

  /**
   * @brief Get glyph ofsset of specific sign.
   * @param uiSign:             Sign to query
   * @param iIndexToLocFormat:  Local format to check 32bit or 16bit offset
   * @return Offset to target glyph.
   */
  uint32 getGlyphOffset(uint16 uiSign, int32 iIndexToLocFormat);
};
#pragma pack(pop)
