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
 * @brief     Class CcButton
 */
#include "CcButton.h"
#include "CcKernel.h"
#include "CcWindow.h"
#include "CcPainter.h"
#include "CcText.h"

class CcButton::CPrivate
{
public:
  CcStyleButton oStyle;
  CcFont        oFont;
  bool          m_bIsHovered = false;
  bool          m_bIsActive = false;
  bool          m_bIsFocused = false;
  bool          m_bIsFocusable = false;
  CcColor       m_oHoverColor = CcStyle::ButtonHoverBackgroundColor;
  CcString      sText;
}; 

CcButton::CcButton( CcWidget* pParent) :
  CcWidget(pParent)
{
  CCNEW(m_pPrivate, CPrivate);
  setSubSystemHandle(static_cast<void*>(m_pPrivate));
  CcWidget::setStyle(CcStyleButton::oDefaultWidgetStyle);
}

CcButton::~CcButton() 
{
  CCDELETE(m_pPrivate);
}

void CcButton::setCustomPainting(bool bEnable)
{
  CcWidget::setCustomPainting(bEnable);
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
      // @todo TBD
      //bRet = m_pPrivate->oButton.setPixelArea(oArea);
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
  CCUNUSED(bDoFlush);
  /*drawBorder(getBorderColor(), getBorderSize());
  CcPainter oPainter(this);
  if (m_pPrivate->m_bIsHovered)
    oPainter.setColor(getStyle()->HoverBackgroundColor);
  else
    oPainter.setColor(getBackgroundColor());
  CcRectangle oRectangle = getRectangle();
  oRectangle = CcPoint(0, 0);
  oPainter.drawRectangle(oRectangle, 0, true);
  
  CcSize oTextSize = m_pPrivate->m_pTextWidget->getSize();
  uint16 uiTempHeight = static_cast<uint16>(oTextSize.getHeight());
  uint16 uiTempWidth  = static_cast<uint16>(oTextSize.getWidth());
  uiTempWidth  = static_cast<uint16>(getWidth()) - uiTempWidth;
  uiTempHeight = static_cast<uint16>(getHeight()) - uiTempHeight;
  uiTempWidth  = uiTempWidth  / 2;
  uiTempHeight = uiTempHeight / 2;
  m_pPrivate->m_pTextWidget->setTextOffset(static_cast<uint16>(getPos().getX()) + uiTempWidth, static_cast<uint16>(getPos().getY()) + uiTempHeight );
  m_pPrivate->m_pTextWidget->setBackgroundColor(getBackgroundColor());
  drawBackground(getBackgroundColor());
  drawBorder(getBorderColor(), getBorderSize());
  m_pPrivate->m_pTextWidget->drawString();*/
}

void CcButton::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CcWidget::drawPixel(oPixel, uiNumber);
}

bool CcButton::isHovered() const
{
  return m_pPrivate->m_bIsHovered;
}

void CcButton::setHoverStyle(bool bActive, const CcColor &oForegroundColor, const CcColor &oBackgroundColor, const CcColor &oBorderColor, uint16 uiBorderSize)
{
  getStyle().oHoverStyle.oForegroundColor = oForegroundColor;
  getStyle().oHoverStyle.oBackgroundColor = oBackgroundColor;
  getStyle().oHoverStyle.oBorderColor = oBorderColor;
  getStyle().oHoverStyle.uBorderSize = uiBorderSize;
  getStyle().bHoverActive = bActive;
  CcStyleEvent oEvent(EEventType::StyleHoverColor);
  event(&oEvent);
}

CcStyleButton& CcButton::getStyle()
{
  return m_pPrivate->oStyle;
}

const CcStyleButton& CcButton::getStyle() const
{
  return m_pPrivate->oStyle;
}

void CcButton::onEvent(CcInputEvent* pEventData)
{
  switch (pEventData->getType())
  {
    case EEventType::StyleBackgroundColor:
    {
      break;
    }
    case EEventType::StyleForegroundColor:
    {
      break;
    }
    default:
      CcWidget::onEvent(pEventData);
      break;
  }
}

void CcButton::onMouseEvent(CcMouseEvent* pEventData)
{
  switch (pEventData->getType())
  {
    case EEventType::MouseLeftDown:
      onMouseClick(pEventData);
      break;
    case EEventType::MouseHover:
      onMouseHover(pEventData);
      break;
    case EEventType::MouseLeave:
      onMouseLeave(pEventData);
      break;
    case EEventType::MouseLeftDoubleClick:
      onMouseHover(pEventData);
      break;
    default:
      break;
  }
}

void CcButton::onKeyEvent(CcKeyEvent* pEventData)
{
  CCUNUSED(pEventData);
}

void CcButton::onWindowEvent(CcInputEvent *pEventData)
{
  CCUNUSED(pEventData);
}

void CcButton::onMouseHover(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
  m_pPrivate->m_bIsHovered = true;
  if (getStyle().bHoverActive)
  {
    setBackgroundColor(getStyle().oHoverStyle.oBackgroundColor);
    setForegroundColor(getStyle().oHoverStyle.oForegroundColor);
    draw();
  }
}

void CcButton::onMouseLeave(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
  m_pPrivate->m_bIsHovered = false;
  if (getStyle().bHoverActive)
  {
    setBackgroundColor(CcWidget::getStyle().oBackgroundColor);
    setForegroundColor(CcWidget::getStyle().oForegroundColor);
    draw();
  }
}

void CcButton::onMouseClick(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
}

void CcButton::onMouseDoubleClick(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
  draw();
}

void CcButton::onRectangleChanged()
{
  CcWidget::onRectangleChanged();
}

void CcButton::setText(const CcString& sString )
{
  m_pPrivate->sText = sString;
}

const CcString& CcButton::getText()
{
  return m_pPrivate->sText;
}
