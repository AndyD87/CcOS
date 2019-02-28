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
 * @page      Windows
 * @subpage   CcWindowsGuiSubSystem
 *
 * @page      CcWindowsGuiSubSystem
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Class CcWindowsGuiSubSystem
 **/
#ifndef _WINDOWSWINDOW_H_
#define _WINDOWSWINDOW_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindowsGlobals.h"
#include "Devices/IDisplay.h"
#include "CcGuiSubSystem.h"
#include "CcWindowsTouch.h"
#include "CcWString.h"
#include "Types/CcColor.h"
#include "Types/CcBitmap.h"
#include "CcCWnd.h"

/**
 * @brief This class creates an Area on a Display wich can be used to draw within.
 *        Currently, this will be used to draw the generic CcWindow on Windows.
 * @todo  It
 */
class CcWindowsGuiSubSystem : public CcGuiSubSystem
{
public:
  CcWindowsGuiSubSystem(const CcWindowHandle& hWindow);
  virtual ~CcWindowsGuiSubSystem();

  void init();
  virtual CcStatus open() override;
  virtual CcStatus close() override;
  virtual void loop() override;
  virtual void drawPixel(const CcColor& oPixel, uint64 uiNumber = 1) override;
  virtual void draw()override;
  virtual void getMaxArea(CcRectangle& oArea) override;
  virtual void hide()override;
  virtual void show()override;
  virtual bool hasFrame() override
    { return true; }
  virtual CcRectangle getInnerRect() override;

  virtual bool setPixelArea(const CcRectangle& oArea) override;
  virtual void setWindowTitle(const CcString& sTitle) override;
  virtual bool setWindowState(EWindowState eState) override;
  virtual CcSubSysHandle getHandle() override;

  void onColorChanged();
  void updateSize() override;
  void updatePos() override;


  void TrackMouse();
  LRESULT executeMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private: //methods
  void drawBitmap(HWND hWnd);

private: //member
  HINSTANCE m_hInst;
  HWND      m_hWnd;
  CcBitmap  m_Bitmap;
  BITMAPINFO m_bmi;
  CcSize    m_oNextSize;
  bool m_bMouseTrackingOn = false;
  bool m_bWindowClosedCalled = false;

  uint32 m_uiMouseFlags = 0;

  int32 m_DrawXStart;
  int32 m_DrawYStart;
  int32 m_DrawXSize;
  int32 m_DrawYSize;
  int32 m_CursorX;
  int32 m_CursorY;
  CcWString m_WindowTitle;
  CcWString m_WindowId;
  RECT m_oLastWindowRect;
  CcRectangle   m_oInnerRect;
};

#endif /* _WINDOWSWINDOW_H_ */
