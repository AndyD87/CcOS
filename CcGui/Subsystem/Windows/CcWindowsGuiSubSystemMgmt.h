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
 * @subpage   CcWindowsGuiSubSystemMgmt
 *
 * @page      CcWindowsGuiSubSystemMgmt
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsGuiSubSystemMgmt
 **/
#ifndef _CcWindowsGuiSubSystemMgmt_H_
#define _CcWindowsGuiSubSystemMgmt_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindowsGlobals.h"
#include "CcVector.h"
#include "CcWindowsGuiSubSystem.h"

class CcWindow;

typedef struct{
  HWND hWindow;
  CcWindowsGuiSubSystem* Window;
} sWindowHandle;

class CcWindowsGuiSubSystemMgmt
{
public:
  static CcWindowsGuiSubSystem* getWindow(HWND hWnd);
  static void registerWindow(HWND hWnd, CcWindowsGuiSubSystem* Window);
  static void deleteWindow(HWND hWnd, CcWindowsGuiSubSystem* Window);
  static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  static bool hasOpenedWindows() 
    {return m_WidgetList.size() > 0;}

private: //member
  static CcVector<sWindowHandle> m_WidgetList;
  static uint16 s_WindowCnt;
};

#endif /* _CcWindowsGuiSubSystemMgmt_H_ */
