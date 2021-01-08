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
 * @brief     Windows implementation of Class CcButton
 */

#include "CcButton.h"
#include "CcKernel.h"
#include "CcWindow.h"
#include "CcPainter.h"
#include "CcGuiSubsystem.h"
#include "CcWindowsGuiUtil.h"
#include "CcWindowsGuiPixmap.h"
#include "Style/CcStyleButton.h"
#include "Style/CcStyle.h"
#include <afxbutton.h>

class CcCButton : public CMFCButton, public CcWindowsGuiPixmap
{
  DECLARE_DYNAMIC(CcCButton)

public:
  CcCButton(CcWidget* pParentWidget) :
    m_pParentWidget(pParentWidget)
  {
  }

  virtual ~CcCButton()
  {
  }

  void SetFlatStyle(CMFCButton::FlatStyle eFlatStyle = CMFCButton::BUTTONSTYLE_FLAT)
  {
    m_nFlatStyle = eFlatStyle;
  }

  void OnPaint()
  {
    if (m_bPaintingOn)
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
              if(m_bmi.bmiHeader.biBitCount == 24)
                m_Bitmap.copy((SBitmapRGB*) data, x, y);
              else
                m_Bitmap.copy((SBitmapARGB*)data, x, y);
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
      CMFCButton::OnPaint();
    }
  }

  void enablePainting()
  {
    m_bPaintingOn = true;
    setSize(m_pParentWidget->getWidth(), m_pParentWidget->getHeight(), 4);
  }

  void OnSize(UINT nType, int cx, int cy)
  {
    CMFCButton::OnSize(nType, cx, cy);
    if (m_bPaintingOn)
    {
      if (cx < 0) cx = -cx;
      if (cy < 0) cy = -cy;
      setSize(static_cast<uint16>(cx), static_cast<uint16>(cy), 4);
    }
    if (m_pParentWidget != nullptr)
      m_pParentWidget->setSize(CcSize(cx, cy));
  }

  void OnMove(int x, int y)
  {
    if (m_pParentWidget != nullptr)
      m_pParentWidget->setPos(CcPoint(x, y));
  }

  void OnMouseHover(UINT nFlags, CPoint point)
  {
    CMFCButton::OnMouseHover(nFlags, point);
    CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseHover, point);
    if (m_pParentWidget != nullptr)
    {
      m_pParentWidget->event(EEventType::MouseHover, &oEvent);
    }
  }

  void  OnMouseMove(UINT nFlags, CPoint point)
  {
    CMFCButton::OnMouseMove(nFlags, point);
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

  void  OnMouseLeave()
  {
    CMFCButton::OnMouseLeave();
    m_bIsHovered = false;
    if (m_pParentWidget != nullptr)
    {
      m_pParentWidget->event(EEventType::MouseLeave, nullptr);
    }
  }

  void OnLButtonDown(UINT nFlags, CPoint point)
  {
    CMFCButton::OnLButtonDown(nFlags, point);
    m_bIsHovered = false;
    CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseLeftDown, point);
    if (m_pParentWidget != nullptr)
    {
      m_pParentWidget->event(EEventType::MouseLeftDown, &oEvent);
    }
  }

  void OnLButtonUp(UINT nFlags, CPoint point)
  {
    CMFCButton::OnLButtonDown(nFlags, point);
    m_bIsHovered = false;
    CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseLeftUp, point);
    if (m_pParentWidget != nullptr)
    {
      m_pParentWidget->event(EEventType::MouseLeftUp, &oEvent);
    }
  }

  void OnLButtonDblClk(UINT nFlags, CPoint point)
  {
    CMFCButton::OnLButtonDown(nFlags, point);
    m_bIsHovered = false;
    CcMouseEvent oEvent = CcWindowsGuiUtil::fromCPoint(EEventType::MouseLeftDoubleClick, point);
    if (m_pParentWidget != nullptr)
    {
      m_pParentWidget->event(EEventType::MouseLeftDown, &oEvent);
    }
  }


protected:
  DECLARE_MESSAGE_MAP()

public:
  static uint m_uButtonIds;
  CcWidget* m_pParentWidget;
  bool m_bPaintingOn = false;
  bool m_bIsHovered = false;
};

IMPLEMENT_DYNAMIC(CcCButton, CMFCButton)

BEGIN_MESSAGE_MAP(CcCButton, CMFCButton)
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

class CcButton::CPrivate
{
public:
  CPrivate(CcWidget* pParentWidget) :
    oButton(pParentWidget)
  {
  }
  CcCButton     oButton;
  CFont         oStaticFont;
  CcStyleButton oStyle;
  CcString      sLabel;
};

CcButton::CcButton(CcWidget* rParent) :
  CcWidget(rParent)
{
  CCNEW(m_pPrivate, CPrivate, this);
  CMFCButton* pParent = static_cast<CMFCButton*>(getParent()->getSubSysHandle());
  if (m_pPrivate->oButton.Create(L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CcWindowsGuiUtil::getRect(getRectangle()), pParent, CcWindowsGuiUtil::getNextId()))
  {
    setSubSystemHandle(&m_pPrivate->oButton);
    m_pPrivate->oButton.SetFlatStyle();
    m_pPrivate->oStaticFont.CreateFontW(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI");
    m_pPrivate->oButton.SetFont(&m_pPrivate->oStaticFont, false);
    m_pPrivate->oButton.SetTextColor(CcWindowsGuiUtil::getRGB(getForegroundColor()));
    m_pPrivate->oButton.SetFaceColor(CcWindowsGuiUtil::getRGB(getBackgroundColor()));
  }
}

CcButton::~CcButton()
{
  CCDELETE(m_pPrivate);
}

bool CcButton::setPixelArea(const CcRectangle& oArea)
{
  bool bRet = false;
  if ((getRectangle().getWidth() > oArea.getX() &&
      getRectangle().getHeight() > oArea.getY()))
  {
    CcPoint oBottomRight = oArea.getBottomRightCorner();
    if (
      (getRectangle().getWidth() > oBottomRight.getX() &&
        getRectangle().getHeight() > oBottomRight.getY()))
    {
      bRet = m_pPrivate->oButton.setPixelArea(oArea);
    }
    else
      CCDEBUG("Out of area");
  }
  else
    CCDEBUG("Out of area");
  return bRet;
}

void CcButton::draw(bool bDoFlush)
{
  if (isHovered())
  {
    drawBackground(getStyle().oHoverStyle.oBackgroundColor);
    drawBorder(getStyle().oHoverStyle.oBorderColor, getStyle().oHoverStyle.uBorderSize);
  }
  else
  {
    drawBackground(CcWidget::getStyle().oBackgroundColor);
    drawBorder(CcWidget::getStyle().oBorderColor, CcWidget::getStyle().uBorderSize);
  }
  if (bDoFlush)
  {
    m_pPrivate->oButton.RedrawWindow();
    m_pPrivate->oButton.Invalidate();
    m_pPrivate->oButton.UpdateWindow();
  }
}

void CcButton::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  m_pPrivate->oButton.drawPixel(oPixel, uiNumber);
}

bool CcButton::isHovered() const
{
  return m_pPrivate->oButton.m_bIsHovered;
}

void CcButton::setHoverStyle(bool bActive, const CcColor &oForegroundColor, const CcColor &oBackgroundColor, const CcColor &oBorderColor, uint16 uiBorderSize)
{
  getStyle().oHoverStyle.oForegroundColor = oForegroundColor;
  getStyle().oHoverStyle.oBackgroundColor = oBackgroundColor;
  getStyle().oHoverStyle.oBorderColor = oBorderColor;
  getStyle().oHoverStyle.uBorderSize = uiBorderSize;
  getStyle().bHoverActive = bActive;
  CcStyle::EType eType = EEventType::StyleHoverColor;
  event(EEventType::WidgetStyleChanged, &eType);
}

CcStyleButton& CcButton::getStyle()
{
  return m_pPrivate->oStyle;
}

const CcStyleButton& CcButton::getStyle() const
{
  return m_pPrivate->oStyle;
}

void CcButton::onMouseEvent(CcMouseEvent* pEventData)
{
  switch (eEvent)
  {
    case EEventType::MouseLeftDown:
      onMouseClick(pMouseEvent);
      break;
    case EEventType::MouseHover:
      onMouseHover(pMouseEvent);
      break;
    case EEventType::MouseLeave:
      onMouseLeave(pMouseEvent);
      break;
    case EEventType::MouseLeftDoubleClick:
      onMouseHover(pMouseEvent);
      break;
  }
}

void CcButton::onKeyEvent(CcKeyEvent* pEventData)
{
  CCUNUSED(eEvent);
  CCUNUSED(pKeyEvent);
}

void CcButton::onWindowEvent(EEventType eWindowEvent)
{
  CCUNUSED(eWindowEvent);
}

void CcButton::setCustomPainting(bool bEnable)
{
  if (bEnable)
  {
    m_pPrivate->oButton.enablePainting();
  }
}

void CcButton::onMouseHover(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
  draw();
}

void CcButton::onMouseLeave(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
  draw();
}

void CcButton::onMouseClick(CcMouseEvent* pEvent)
{
  CCUNUSED(pEvent);
}

void CcButton::onMouseDoubleClick(CcMouseEvent* pEvent)
{
  CCUNUSED(pEvent);
}

void CcButton::onRectangleChanged()
{
  CMFCButton* pHandle = static_cast<CMFCButton*>(getSubSysHandle());
  if (pHandle != nullptr)
  {
    const CWnd* pParentWindow = &CWnd::wndTop;
    pHandle->SetWindowPos(pParentWindow,
                          getRectangle().getX(),
                          getRectangle().getY(),
                          getRectangle().getWidth(),
                          getRectangle().getHeight(),
                          SWP_NOZORDER);
  }
}

void CcButton::onEvent(EEventType eType, void* pEventData)
{
  switch (eType)
  {
    case EEventType::WidgetStyleChanged:
    {
      CcStyle::EType* pEventType = static_cast<CcStyle::EType*>(pEventData);
      switch (*pEventType)
      {
        case EEventType::StyleBackgroundColor:
        {
          CMFCButton* pHandle = static_cast<CMFCButton*>(getSubSysHandle());
          if (pHandle != nullptr)
          {
            pHandle->SetFaceColor(CcWindowsGuiUtil::getRGB(getBackgroundColor()));
          }
          break;
        }
        case EEventType::StyleForegroundColor:
        {
          CMFCButton* pHandle = static_cast<CMFCButton*>(getSubSysHandle());
          if (pHandle != nullptr)
          {
            pHandle->SetTextColor(CcWindowsGuiUtil::getRGB(getForegroundColor()));
          }
          break;
        }
      }
      break;
    }
    default:
      break;
  }
}

void CcButton::setText(const CcString& sString )
{
  m_pPrivate->sLabel = sString;
  static_cast<CMFCButton*>(getSubSysHandle())->SetWindowTextW(TOLPCWSTR(m_pPrivate->sLabel));
}

const CcString& CcButton::getText()
{
  return m_pPrivate->sLabel;
}
