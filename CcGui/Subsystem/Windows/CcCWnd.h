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

/**
 * @brief Wrap CWnd class and convert signal to CcGui known format
 */
class CcCWnd : public CWnd, public CcWindowsGuiPixmap
{
  // Macro for MFC
  DECLARE_DYNAMIC(CcCWnd)

public:
  /**
   * @brief Create CWnd with target Widget to passthrough signals
   * @param pParentWidget
   */
  CcCWnd(CcWidget* pParentWidget);

  /**
   * @brief Create CWnd from an existing window handle
   * @param pParentWindow
   */
  CcCWnd(HWND pParentWindow);
  virtual ~CcCWnd();

  /**
   * @brief Enable drawping mechanismus
   */
  void enablePainting();

  //! @param pParentWidget change parent widget item.
  void setParentWidget(CcWidget* pParentWidget)
    { m_pParentWidget = pParentWidget; }

  /**
   * @brief Override on paint method
   */
  void OnPaint();

  /**
   * @brief Override on size for window method
   * @param nType: Type of resizing like relative absolut
   * @param cx: X value for resize
   * @param cy: Y value for resize
   */
  void OnSize(UINT nType, int cx, int cy);

  /**
   * @brief Override on move for window method
   * @param x: X value of movement
   * @param y: Y value of movement
   */
  void OnMove(int x, int y);

  /**
   * @brief Override on mouse hover signal
   * @param nFlags: Additional info of mouse hover event
   * @param point:  Position of hover event
   */
  void OnMouseHover(UINT nFlags, CPoint point);

  /**
   * @brief Override on mouse move signal
   * @param nFlags: Additional info of mouse move event
   * @param point:  Position of next move
   */
  void OnMouseMove(UINT nFlags, CPoint point);

  /**
   * @brief Override on mouse leave signale
   */
  void OnMouseLeave();
  
  /**
   * @brief Override on left mouse button down.
   * @param nFlags: Additional info of mouse move event
   * @param point:  Position of mouse event
   */
  void OnLButtonDown(UINT nFlags, CPoint point);

  /**
   * @brief Override on left mouse button up.
   * @param nFlags: Additional info of mouse move event
   * @param point:  Position of mouse event
   */
  void OnLButtonUp(UINT nFlags, CPoint point);

  /**
   * @brief Override on left mouse button double click.
   * @param nFlags: Additional info of mouse move event
   * @param point:  Position of mouse event
   */
  void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
  DECLARE_MESSAGE_MAP()

public:
  CcWidget* m_pParentWidget = nullptr;  //!< Target widget for events
  bool m_bIsHovered = false;            //!< Track mouse on window if true.
  bool m_bPaintingOn = false;           //!< If true painting is activated
};

#endif // H_CcCWnd_H_
