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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcCWnd
 */

#include "CcCWnd.h"
#include "CcString.h"
#include "CcWidget.h"
#include "CcRectangle.h"
#include "CcWindowsGuiUtil.h"

IMPLEMENT_DYNAMIC(CcCWnd, CWnd)

BEGIN_MESSAGE_MAP(CcCWnd, CWnd)
  ON_WM_PAINT()
  ON_WM_MOUSEHOVER()
  ON_WM_MOUSEMOVE()
  ON_WM_MOUSELEAVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_SIZE()
  ON_WM_MOVE()
END_MESSAGE_MAP()


CcCWnd::CcCWnd(CcWidget* pParentWidget) :
  m_pParentWidget(pParentWidget)
{
  CWnd* pParentHandle = nullptr;
  if (pParentWidget->getParent() != nullptr)
  {
    pParentHandle = CWNDHANDLE(pParentWidget->getParent());
  }
  Create(CcWindowsGuiUtil::getWidgetClass(), L"", WS_CHILD | WS_VISIBLE | WS_MAXIMIZE, CcWindowsGuiUtil::getRect(pParentWidget->getRectangle()), pParentHandle, CcWindowsGuiUtil::getNextId(), NULL);
}

CcCWnd::CcCWnd(HWND pParentWindow)
{
  RECT oRect = {0};
  Create(CcWindowsGuiUtil::getWidgetClass(), L"", WS_CHILD | WS_VISIBLE | WS_MAXIMIZE, oRect, CWnd::FromHandle(pParentWindow), CcWindowsGuiUtil::getNextId(), NULL);
}

CcCWnd::~CcCWnd()
{
}

void CcCWnd::enablePainting()
{
  m_bPaintingOn = true;
  setSize(m_pParentWidget->getWidth(), m_pParentWidget->getHeight(), 3);
}

void CcCWnd::OnPaint()
{
  if (m_bPaintingOn && false)
  {
    PAINTSTRUCT   ps;
    uint8        *data = nullptr;
    CDC* pCdc = BeginPaint(&ps);
    HDC hdc = *pCdc;
    if (hdc != nullptr)
    {
      HDC hMemDC = CreateCompatibleDC(hdc);
      HBITMAP hBitmap = CreateDIBSection(hdc, &m_bmi, DIB_RGB_COLORS, (void **) &data, 0, 0);
      if (hBitmap != nullptr)
      {
        HGDIOBJ hOldBmp = SelectObject(hMemDC, hBitmap);

        for (uint16 y = (uint16) ps.rcPaint.top; y < ps.rcPaint.bottom; y++)
        {
          for (uint16 x = (uint16) ps.rcPaint.left; x < ps.rcPaint.right; x++)
          {
            m_Bitmap.copy((SBitmapRGB*) data, x, y);
          }
        }

        BitBlt(hdc, 0, 0, m_Bitmap.width, m_Bitmap.height, hMemDC, 0, 0, SRCCOPY);

        SelectObject(hMemDC, hOldBmp);
        DeleteDC(hMemDC);
        DeleteObject(hBitmap);
      }
      EndPaint(&ps);
    }
  }
  else
  {
    CWnd::OnPaint();
  }
}

void CcCWnd::OnSize(UINT nType, int cx, int cy)
{
  CWnd::OnSize(nType, cx, cy);
  if (m_bPaintingOn)
  {
    if (cx < 0) cx = -cx;
    if (cy < 0) cy = -cy;
    setSize(static_cast<uint16>(cx), static_cast<uint16>(cy), 3);
  }
  if(m_pParentWidget != nullptr)
    m_pParentWidget->setSize(CcSize(cx, cy));
}

void CcCWnd::OnMove(int x, int y)
{
  if (m_pParentWidget != nullptr)
    m_pParentWidget->setPos(CcPoint(x, y));
}

void CcCWnd::OnMouseHover(UINT nFlags, CPoint point)
{
  CWnd::OnMouseHover(nFlags, point);
  CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseHover, point);
  if (m_pParentWidget != nullptr)
  {
    m_pParentWidget->event(EEventType::MouseHover, &oEvent);
  }
}

void  CcCWnd::OnMouseMove(UINT nFlags, CPoint point)
{
  CWnd::OnMouseMove(nFlags, point);
  CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseMove, point);
  if (m_pParentWidget != nullptr)
  {
    m_pParentWidget->event(EEventType::MouseMove, &oEvent);
  }
  if (m_bIsHovered == false)
  {
    m_bIsHovered = true;
    CcWindowsGuiUtil::trackMouse(GetSafeHwnd());
  }
}

void  CcCWnd::OnMouseLeave()
{
  CWnd::OnMouseLeave();
  m_bIsHovered = false;
  if (m_pParentWidget != nullptr)
  {
    m_pParentWidget->event(EEventType::MouseLeave, nullptr);
  }
}

void CcCWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
  CWnd::OnLButtonDown(nFlags, point);
  m_bIsHovered = false;
  CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseLeftDown, point);
  if (m_pParentWidget != nullptr)
  {
    m_pParentWidget->event(EEventType::MouseLeftDown, &oEvent);
  }
}

void CcCWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
  CWnd::OnLButtonDown(nFlags, point);
  m_bIsHovered = false;
  CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseLeftUp, point);
  if (m_pParentWidget != nullptr)
  {
    m_pParentWidget->event(EEventType::MouseLeftUp, &oEvent);
  }
}

void CcCWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  CWnd::OnLButtonDown(nFlags, point);
  m_bIsHovered = false;
  CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseLeftDoubleClick, point);
  if (m_pParentWidget != nullptr)
  {
    m_pParentWidget->event(EEventType::MouseLeftDown, &oEvent);
  }
}
