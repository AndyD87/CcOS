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
 * @subpage   CcWindowsGuiUtil
 *
 * @page      CcWindowsGuiUtil
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsGuiUtil
 **/
#ifndef _CcWindowsGuiUtil_H_
#define _CcWindowsGuiUtil_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindowsGlobals.h"
#include "CcRectangle.h"
#include "CcColor.h"
#include "CcWString.h"
#include "CcInputEvent.h"

#define TOLPCWSTR(oCcString) CcWString(oCcString).getLPCWSTR()
#define CWNDHANDLE()         getSubSysHandle().cast<CWnd>().ptr()

class CcWindowsGuiUtil
{
public:
  inline static RECT getRect(const CcRectangle& oRectangle)
    { RECT oRet = { oRectangle.getX(), oRectangle.getY(), oRectangle.getX() + oRectangle.getWidth(), oRectangle.getY() + oRectangle.getHeight() }; return oRet; }
  inline static COLORREF getRGB(const CcColor& oColor)
    { return RGB(oColor.getR(), oColor.getG(), oColor.getB()); }
  inline static uint getNextId()
    {return s_uiIdInc++; }
  inline static LPCWSTR getWidgetClass()
    { return L"Widget"; }
  static void trackMouse(HWND hWnd);
  static CcMouseEvent fromCPoint(EMouseEventType eType, const CPoint& rPoint);
private:
  static uint s_uiIdInc;
};

#endif /* _CcWindowsGuiUtil_H_ */
