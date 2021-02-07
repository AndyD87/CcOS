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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Resource file from Bitstream Vera Sans Font for importing in Generic implementation.
 *            This file was mainly generated form CcFontConverter.
 *            (https://coolcow.de/projects/Tools/CcFontConverter.git)
 */
// FontConverter start
#include <stdlib.h>
#pragma once

#pragma pack(push, 1)
/**
 * @brief Structure for describing an filled rectangular.
 *        Containing data structure of row and columns is depending on it's width.
 *        For size of a row, SFontRectangle_GetArrayWidth can be used to get it width of row in bytes.
 */
typedef struct
{
  unsigned char uiWidth;  //!< Width of rectangle
  unsigned char uiHeight;  //!< Height of rectangle
  unsigned char pData[1];  //!< Pix map with size SFontRectangle_GetArrayWidth(this) * uiHeight.
} SFontRectangle;
#pragma pack(pop)

/**
 * @brief Get width of row in bytes
 * @param pRectangle: Get width of row in bytes from this rectangle
 * @return number of real width in bytes.
 */
inline unsigned char SFontRectangle_GetArrayWidth(SFontRectangle* pRectangle)
{
  unsigned char uiValue=0;
  uiValue = pRectangle->uiWidth / 8;
  if((pRectangle->uiWidth % 8) != 0)
  {
    uiValue++;
  }
  return uiValue;
}

//! BitstreamVeraSans 7 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_7_Size;

//! BitstreamVeraSans 8 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_8_Size;

//! BitstreamVeraSans 12 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_12_Size;

//! BitstreamVeraSans 16 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_16_Size;

//! BitstreamVeraSans 24 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_24_Size;

//! BitstreamVeraSans 48 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_48_Size;

//! BitstreamVeraSans 64 Size Character table
extern size_t CcFontRectangleBitstreamVeraSans_BitstreamVeraSans_64_Size;
