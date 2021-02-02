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
 * @brief     Class CcWindowsGuiPixmap
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcBitmap.h"
#include "CcRectangle.h"

/**
 * @brief Define pixmap for drawing on windows
 */
class CcWindowsGuiPixmap
{
public:
  CcWindowsGuiPixmap();
  ~CcWindowsGuiPixmap();
  /**
   * @brief Adjust size for basic bitmap
   * @param uiWidth:          Widht of bitmap
   * @param uiHeight:         Height of bitmap
   * @param uiBytesPerPixel:  Number of Pytes per pixel.
   *                          It is depending on color mode
   */
  void setSize(int32 uiWidth, int32 uiHeight, uint8 uiBytesPerPixel);
  /**
   * @brief Resize pixmap with data from Rectanble
   * @param oArea:  Area as rectangle
   * @return True if values were set successfully
   */
  bool setPixelArea(const CcRectangle& oArea);

  /**
   * @brief Draw a single pixel to bitmap on current cursor
   * @param oPixel:    Pixel color to write
   * @param uiNumber:  Repeated number of pixels to draw
   */
  void drawPixel(const CcColor& oPixel, uint64 uiNumber);
public:
  BITMAPINFO m_bmi;   //!< Bitmap handle form windows

  int32 m_DrawXStart; //!< X Offset of drawing windows for cursor
  int32 m_DrawYStart; //!< Y Offset of drawing windows for cursor
  int32 m_DrawXSize;  //!< Number of pixels available in X direction
  int32 m_DrawYSize;  //!< Number of pixels available in Y direction
  int32 m_CursorX;    //!< current X location of Curser in drawing window
  int32 m_CursorY;    //!< current Y location of Curser in drawing window

  CcBitmap  m_Bitmap; //!< Common bitmap as storage
};
