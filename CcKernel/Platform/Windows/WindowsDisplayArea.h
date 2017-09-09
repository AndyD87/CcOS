/**
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
 * @page      Windows
 * @subpage   WindowsDisplayArea
 *
 * @page      WindowsDisplayArea
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsDisplayArea
 **/
#ifndef WINDOWSWINDOW_H_
#define WINDOWSWINDOW_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include "Devices/CcDisplay.h"
#include "Devices/CcDisplayArea.h"
#include "WindowsTouch.h"
#include "CcWString.h"
#include "Types/CcColor.h"
#include "Types/CcBitmap.h"

class WindowsDisplayArea : public CcDisplayArea
{
public:
  WindowsDisplayArea(uint16 nr, const CcRectangle& oArea);
  virtual ~WindowsDisplayArea();

  void init(void);
  void loop() override;
  void close() override;
  void drawPixel(const CcColor& oPixel) override;
  bool setPixelArea(const CcRectangle& oArea) override;
  void draw()override;

  void onColorChanged(void);
  void updateSize() override;
  void updatePos() override;

  void getMaxArea(CcRectangle& oArea) override;
  void setWindowTitle(const CcString& sTitle) override;
  void hide()override;
  void show()override;

  void TrackMouse();
  LRESULT executeMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private: //methods
  void drawBitmap(HWND hWnd);

private: //member
  HINSTANCE m_hInst;
  HWND      m_hWnd;
  CcBitmap  m_Bitmap;
  BITMAPINFO m_bmi;
  bool m_bMouseTrackingOn = false;
  bool m_bWindowClosedCalled = false;

  uint16 m_uiMouseFlags = 0;

  int16 m_DrawXStart;
  int16 m_DrawYStart;
  uint16 m_DrawXSize;
  uint16 m_DrawYSize;
  uint16 m_CursorX;
  uint16 m_CursorY;
  CcWString m_WindowTitle;
  CcWString m_WindowId;
};

#endif /* WINDOWSWINDOW_H_ */
