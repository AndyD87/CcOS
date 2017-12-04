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
 * @subpage   CcWindowsDisplay
 *
 * @page      CcWindowsDisplay
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsDisplay
 **/
#ifndef CcWindowsDisplay_H_
#define CcWindowsDisplay_H_

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "CcVector.h"
#include "CcWindowsDisplayArea.h"

class CcWindow;

typedef struct{
  HWND hWindow;
  CcWindowsDisplayArea* Window;
} sWindowHandle;

class CcWindowsDisplay : public CcDisplay
{
public:
  CcWindowsDisplay(void);
  virtual ~CcWindowsDisplay()
    {}

  static CcWindowsDisplayArea* getWindow(HWND hWnd);
  static void registerWindow(HWND hWnd, CcWindowsDisplayArea* Window);
  static void deleteWindow(HWND hWnd, CcWindowsDisplayArea* Window);
  static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  CcStatus open(EOpenFlags flags = EOpenFlags::NoFlag) override;
  CcStatus close(void) override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  CcStatus cancel()override {return false;}
  void setBackgroundLed(bool bState) override;
  CcDisplayArea* getDisplayArea(const CcRectangle& oArea) override;

  static bool hasOpenedWindows() 
    {return m_WidgetList.size() > 0;}

private: //member
  static CcVector<sWindowHandle> m_WidgetList;
  static uint16 s_WindowCnt;
};

#endif /* CcWindowsDisplay_H_ */
