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
 */

#include "CcWindowsGuiPixmap.h"
CcWindowsGuiPixmap::CcWindowsGuiPixmap()
{
}

CcWindowsGuiPixmap::~CcWindowsGuiPixmap()
{
}

void CcWindowsGuiPixmap::setSize(int32 uiWidth, int32 uiHeight, uint8 uiBytesPerPixel)
{
  ZeroMemory(&m_bmi, sizeof(m_bmi));
  m_Bitmap.setSize(uiWidth, uiHeight);
  m_Bitmap.format(CcColor(100, 100, 100));
  m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  m_bmi.bmiHeader.biWidth = uiWidth;
  m_bmi.bmiHeader.biHeight = -uiHeight; // set negative, because bitmaps coordinates are beginning at bottom left corner
  m_bmi.bmiHeader.biPlanes = 1;
  m_bmi.bmiHeader.biBitCount = uiBytesPerPixel*8;
  m_bmi.bmiHeader.biCompression = BI_RGB;
  m_bmi.bmiHeader.biSizeImage = m_Bitmap.getPixCount() * uiBytesPerPixel;
}

void CcWindowsGuiPixmap::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  int32 iPointX = m_CursorX + m_DrawXStart;
  int32 iPointY = m_CursorY + m_DrawYStart;
  uint64 uiCount = 0;
  while (uiCount < uiNumber)
  {
    uiCount++;
    if( iPointX >= 0 &&
        iPointY >= 0 &&
        m_CursorX < m_Bitmap.width &&
        m_CursorY < m_Bitmap.height)
    {
      int32 uiTemp = (iPointY) * m_Bitmap.width;
      uiTemp += iPointX;
      if (uiTemp < m_Bitmap.pixCount)
      {
        m_Bitmap.bitmap[uiTemp].R = oPixel.getR();
        m_Bitmap.bitmap[uiTemp].G = oPixel.getG();
        m_Bitmap.bitmap[uiTemp].B = oPixel.getB();
      }
    }
    if (m_CursorX < m_DrawXSize - 1)
    {
      m_CursorX++;
    }
    else
    {
      m_CursorX = 0;
      if (m_CursorY < m_DrawYSize - 1)
      {
        m_CursorY++;
      }
      else
      {
        // End of Rectangle reached, draw window.
        m_CursorY = 0;
      }
    }
    iPointX = m_CursorX + m_DrawXStart;
    iPointY = m_CursorY + m_DrawYStart;
  }
}

bool CcWindowsGuiPixmap::setPixelArea(const CcRectangle& oArea)
{
  bool bRet = false;
  m_DrawXStart = oArea.getX();
  m_DrawYStart = oArea.getY();
  m_DrawXSize = oArea.getWidth();
  m_DrawYSize = oArea.getHeight();
  m_CursorX = 0;
  m_CursorY = 0;
  bRet = true;
  return bRet;
}
