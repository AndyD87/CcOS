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
 * @brief     Class CcWindowsGuiUtil
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindowsGlobals.h"
#include "CcRectangle.h"
#include "CcColor.h"
#include "CcWString.h"
#include "CcInputEvent.h"

//! Get a temporary wchar array from string
#define TOLPCWSTR(oCcString) CcWString(oCcString).getLPCWSTR()

//! Get Window handle from subsystem
#define CWNDHANDLE(POINTER)         static_cast<CWnd*>(POINTER->getSubSysHandle())

/**
 * @brief Helper methods for Windows GUI elements
 */
class CcWindowsGuiUtil
{
public:
  /**
   * @brief Convert CcOS Rectangle to Windows Rectangle
   * @param oRectangle: Rectangle to convert
   * @return Converted Rectangle
   */
  inline static RECT getRect(const CcRectangle& oRectangle)
  { RECT oRet = { oRectangle.getX(), oRectangle.getY(), oRectangle.getX() + oRectangle.getWidth(), oRectangle.getY() + oRectangle.getHeight() }; return oRet; }

  /**
   * @brief Get color ref in windows format from CcOS Color
   * @param oColor: Color to convert
   * @return Windows color value
   */
  inline static COLORREF getRGB(const CcColor& oColor)
  { return RGB(oColor.getR(), oColor.getG(), oColor.getB()); }

  /**
   * @brief Incrementing index for unique identifier
   * @return Next unique id.
   */
  inline static uint getNextId()
  {return s_uiIdInc++; }

  /**
   * @brief Get name of Widget class
   * @return Wide char string for widget class name
   */
  inline static LPCWSTR getWidgetClass()
  { return L"Widget"; }

  /**
   * @brief Enable mouse tracking for out of Window movements
   * @param hWnd: Target window to enable for.
   */
  static void trackMouse(HWND hWnd);

  /**
   * @brief Create mousevent with type and point information.
   * @param eType:  Mouse event type
   * @param rPoint: Coordinates to set for Event
   * @return Generated mouse event
   */
  static CcMouseEvent fromCPoint(EEventType eType, const CPoint& rPoint);
private:
  static uint s_uiIdInc;
};
