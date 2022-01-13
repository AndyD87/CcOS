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
 * @brief     Class CcBitmap
 */
#pragma once

#include "CcBase.h"
#include "CcColor.h"
#include "CcStatic.h"

/**
 * @brief Define a RGB structure
 */
class CBitmapRGB
{
public:
  uint8 R = 0;  //!< Red value
  uint8 G = 0;  //!< Green value
  uint8 B = 0;  //!< Blue value
};

/**
 * @brief Define a RGB structure with additional ocupacy valuey
 */
class CBitmapARGB : public CBitmapRGB
{
public:
  uint8 A = 0;  //!< ocupacy value
};

/**
 * @brief Raw bitmap with all pixels
 */
class CcKernelSHARED CcBitmap
{
public:
  /**
   * @brief Destructor, cleanup bitmap
   */
  ~CcBitmap()
  {
    CCDELETEARR(bitmap);
  }

  /**
   * @brief Reset internal bimap size.
   *
   * @param uiWidth:  New width of bitmap
   * @param uiHeight: New height of bitmap
   */
  void setSize(int32 uiWidth, int32 uiHeight)
  {
    width = uiWidth;
    height = uiHeight;
    setPixCount(uiWidth * uiHeight);
  }

  /**
   * @brief Format all pixels to one color
   * @param oColor
   */
  void format(const CcColor& oColor)
  {
    for (int32 uiIndex = 0; uiIndex < pixCount; uiIndex++)
    {
      bitmap[uiIndex].R = oColor.getR();
      bitmap[uiIndex].G = oColor.getG();
      bitmap[uiIndex].B = oColor.getB();
    }
  }

  //! @return Number of pixel this bitmap has
  int64 getPixCount()
  { return pixCount; }

  /**
   * @brief Get pixel at specific location.
   * @param iX: X coordinate of target pixel
   * @param iY: Y coordinate of target pixel
   * @return Reference to the pixel
   */
  CBitmapARGB& getPixel(int32 iX, int32 iY)
  {
    int32 uiPos = (iY * width) + iX;
    return bitmap[uiPos];
  }

  /**
   * @brief Copy pixel at same coordinates from this bitmap to target RGB bitmap
   * @param pTarget:  Target Bitmap
   * @param iX:       X coordinate of pixel to copy
   * @param iY:       Y coordinate of pixel to copy
   */
  void copy(CBitmapRGB* pTarget, int32 iX, int32 iY)
  {
    int iPos = (iY * width) + iX;
    if (iPos < pixCount)
    {
      pTarget[iPos].R = bitmap[iPos].R;
      pTarget[iPos].G = bitmap[iPos].G;
      pTarget[iPos].B = bitmap[iPos].B;
    }
  }

  /**
   * @brief Copy pixel at same coordinates from this bitmap to target ARGB bitmap
   * @param pTarget:  Target Bitmap
   * @param iX:       X coordinate of pixel to copy
   * @param iY:       Y coordinate of pixel to copy
   */
  void copy(CBitmapARGB* pTarget, int32 iX, int32 iY)
  {
    pTarget[(iY * width) + iX] = bitmap[(iY * width) + iX];
  }

private:
  /**
   * @brief Set number of pixels and adjust internal buffer.
   * @param uiPixCount: Number of new pixels
   */
  void setPixCount(int64 uiPixCount)
  {
    CCDELETE(bitmap);
    pixCount = uiPixCount;
    CCNEWARRAY(bitmap,CBitmapARGB,static_cast<size_t>(uiPixCount));
    CcStatic::memset(bitmap, 0, static_cast<size_t>(uiPixCount));
  }

private:
  int32 width;
  int32 height;
  int64 pixCount;
  CBitmapARGB *bitmap = nullptr;
};
