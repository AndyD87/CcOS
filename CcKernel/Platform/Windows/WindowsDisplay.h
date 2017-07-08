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
 * @subpage   WindowsDisplay
 *
 * @page      WindowsDisplay
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsDisplay
 **/
#ifndef WindowsDisplay_H_
#define WindowsDisplay_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include "CcVector.h"
#include "WindowsDisplayArea.h"

class CcWindow;

typedef struct{
  HWND hWindow;
  WindowsDisplayArea* Window;
} sWindowHandle;

class WindowsDisplay : public CcDisplay
{
public:
  WindowsDisplay(void);
  virtual ~WindowsDisplay()
    {}

  static WindowsDisplayArea* getWindow(HWND hWnd);
  static void registerWindow(HWND hWnd, WindowsDisplayArea* Window);
  static void deleteWindow(HWND hWnd, WindowsDisplayArea* Window);
  static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  bool open(EOpenFlags flags = EOpenFlags::NoFlag) override;
  bool close(void) override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  bool cancel()override {return false;}
  void setBackgroundLed(bool bState) override;
  CcDisplayArea* getDisplayArea(const CcRectangle& oArea) override;

  static bool hasOpenedWindows() 
    {return m_WidgetList.size() > 0;}

private: //member
  static CcVector<sWindowHandle> m_WidgetList;
  static uint16 s_WindowCnt;
};

#endif /* WindowsDisplay_H_ */
