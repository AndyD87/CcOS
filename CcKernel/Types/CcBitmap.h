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
 * @page      Types
 * @subpage   CcBitmap
 *
 * @page      CcBitmap
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcBitmap
 */
#ifndef H_CcBitmap_H_
#define H_CcBitmap_H_


#include "CcBase.h"
#include "CcBase.h"
#include "CcColor.h"
#include "CcStatic.h"

typedef struct
{
  uint8 B;
  uint8 G;
  uint8 R;
} SBitmapRGB;

typedef struct
{
  uint8 B;
  uint8 G;
  uint8 R;
  uint8 A;
} SBitmapARGB;

class CcKernelSHARED CcBitmap
{
public:
  ~CcBitmap()
  {
    CCDELETEARR(bitmap);
  }

  void setSize(int32 uiWidth, int32 uiHeight)
  {
    width = uiWidth;
    height = uiHeight;
    setPixCount(uiWidth * uiHeight);
  }

  void format(const CcColor& oColor)
  {
    for (int32 uiIndex = 0; uiIndex < pixCount; uiIndex++)
    {
      bitmap[uiIndex].R = oColor.getR();
      bitmap[uiIndex].G = oColor.getG();
      bitmap[uiIndex].B = oColor.getB();
    }
  }

  void setPixCount(int32 uiPixCount)
  {
    CCDELETE(bitmap);
    pixCount = uiPixCount;
    CCNEWARRAY(bitmap,SBitmapARGB,uiPixCount);
    CcStatic::memset(bitmap, 0, sizeof(SBitmapRGB)*uiPixCount);
  }

  int32 getPixCount()
    {return pixCount;}

  SBitmapARGB& getPixel(int32 x, int32 y)
  {
    int32 uiPos = (y * width) + x;
    return bitmap[uiPos];
  }

  void copy(SBitmapRGB* target, int32 x, int32 y)
  {
    int iPos = (y * width) + x;
    if (iPos < pixCount)
    {
      target[iPos].R = bitmap[iPos].R;
      target[iPos].G = bitmap[iPos].G;
      target[iPos].B = bitmap[iPos].B;
    }
  }

  void copy(SBitmapARGB* target, int32 x, int32 y)
  {
    target[(y * width) + x] = bitmap[(y * width) + x];
  }

  int32 width;
  int32 height;
  int32 pixCount;
  SBitmapARGB *bitmap = nullptr;
};

#endif // H_CcBitmap_H_
