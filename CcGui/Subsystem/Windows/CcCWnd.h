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
 * @brief     Class CcCWnd
 **/
#ifndef H_CcCWnd_H_
#define H_CcCWnd_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindowsGlobals.h"
#include "CcWindowsGuiPixmap.h"
#include "CcBitmap.h"

class CcWidget;
class CcRectangle;

class CcCWnd : public CWnd, public CcWindowsGuiPixmap
{
  DECLARE_DYNAMIC(CcCWnd)

public:
  CcCWnd(CcWidget* pParentWidget);
  CcCWnd(HWND pParentWindow);
  virtual ~CcCWnd();

  void enablePainting();
  void setParentWidget(CcWidget* pParentWidget)
    { m_pParentWidget = pParentWidget; }

  void OnPaint();
  void OnSize(UINT nType, int cx, int cy);
  void OnMove(int x, int y);
  void OnMouseHover(UINT nFlags, CPoint point);
  void OnMouseMove(UINT nFlags, CPoint point);
  void OnMouseLeave();
  
  void OnLButtonDown(UINT nFlags, CPoint point);
  void OnLButtonUp(UINT nFlags, CPoint point);
  void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
  DECLARE_MESSAGE_MAP()

public:
  static uint m_uButtonIds;
  CcWidget* m_pParentWidget = nullptr;
  bool m_bIsHovered = false;

  bool m_bPaintingOn = false;
};

#endif // H_CcCWnd_H_
