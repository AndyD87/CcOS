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
 * @page      CcBitmap
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcBitmap
 */
#ifndef CcBitmap_H
#define CcBitmap_H

#include "CcBase.h"
#include "CcKernelBase.h"


typedef struct
{
  uint8 B;
  uint8 G;
  uint8 R;
} SBitmapRGB;

class CcKernelSHARED CcBitmap
{
public:
  ~CcBitmap()
  {
    if (bitmap != NULL)
    {
      CCMONITORDELETE(bitmap); delete bitmap;
    }
  }

  void setSize(uint16 uiWidth, uint16 uiHeight)
  {
    width = uiWidth;
    height = uiHeight;
    setPixCount(uiWidth * uiHeight);
  }

  void setPixCount(uint32 uiPixCount)
  {
    if (bitmap != NULL)
    {
      CCMONITORDELETE(bitmap); delete bitmap;
    }
    pixCount = uiPixCount;
    bitmap = new SBitmapRGB[uiPixCount]; CCMONITORNEW(bitmap);
  }

  SBitmapRGB& getPixel(uint16 x, uint16 y)
  {
    uint16 uiPos = (y * width) + x;
    return bitmap[uiPos];
  }

  void copy(SBitmapRGB* target, uint16 x, uint16 y)
  {
    target[(y * width) + x] = bitmap[(y * width) + x];
  }

  uint16 width;
  uint16 height;
  uint32 pixCount;
  SBitmapRGB *bitmap = NULL;
};

#endif // CcBitmap_H
