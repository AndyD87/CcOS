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
 * @brief     Class CcWindowsGuiPixmap
 **/
#ifndef H_CcWindowsGuiPixmap_H_
#define H_CcWindowsGuiPixmap_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcBitmap.h"
#include "CcRectangle.h"

class CcWindowsGuiPixmap
{
public:
  CcWindowsGuiPixmap();
  ~CcWindowsGuiPixmap();
  void setSize(int32 uiWidth, int32 uiHeight, uint8 uiBytesPerPixel);
  bool setPixelArea(const CcRectangle& oArea);
  void drawPixel(const CcColor& oPixel, uint64 uiNumber);
public:
  BITMAPINFO m_bmi;

  int32 m_DrawXStart;
  int32 m_DrawYStart;
  int32 m_DrawXSize;
  int32 m_DrawYSize;
  int32 m_CursorX;
  int32 m_CursorY;

  CcBitmap  m_Bitmap;
};

#endif // H_CcWindowsGuiPixmap_H_
