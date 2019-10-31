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
 * @brief     Class CcButton
 */
#include "CcButton.h"
#include "CcKernel.h"
#include "CcWindow.h"
#include "CcPainter.h"
#include "CcFont.h"
#include "CcGuiGlobals.h"

#include "CcQt.h"
#include <QPushButton>

class CcButton::CPrivate : public QPushButton, public CcObject
{
public:
  CPrivate(CcButton* pButton, QWidget* pParent):
    QPushButton(pParent),
    pButton(pButton),
    oFont(CcGuiGlobals::Defaults::FontSize)
  {}

  virtual ~CPrivate() override;

  void setGeometryConvert(void*)
  {
    this->setGeometry(ToQRect(pButton->getRectangle()));
  }

  CcButton* pButton;
  CcFont    oFont;
  bool      m_bIsHovered = false;
  bool      m_bIsActive = false;
  bool      m_bIsFocused = false;
  bool      m_bIsFocusable = false;
  CcColor   m_oHoverColor = CcStyle::ButtonHoverBackgroundColor;
  CcString  sText;
}; 

CcButton::CPrivate::~CPrivate()
{}

CcButton::CcButton( CcWidget* rParent) :
  CcWidget(rParent)
{
  QWidget* pParent = nullptr;
  if (rParent)
    pParent = ToQWidget(rParent->getSubSysHandle());
  CCNEW(m_pPrivate, CPrivate, this, pParent);
  setSubSystemHandle(static_cast<void*>(m_pPrivate));
  setBorderColor(CcColor(
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_R,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_G,
    CCPUSHBUTTON_DEFAULT_BORDERCOLOR_B));
  setBorderSize(CCPUSHBUTTON_DEFAULT_BORDERSIZE);
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
  drawBorder(getBorderColor(), getBorderSize());
  CcPainter oPainter(this);
  if (m_pPrivate->m_bIsHovered)
    oPainter.setColor(getStyle()->HoverBackgroundColor);
  else
    oPainter.setColor(getBackgroundColor());
  CcRectangle oRectangle = getRectangle();
  oRectangle = CcPoint(0, 0);
  oPainter.drawRectangle(oRectangle, 0, true);
}

void CcButton::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CcWidget::drawPixel(oPixel, uiNumber);
}

bool CcButton::isHovered() const
{
  return m_pPrivate->m_bIsHovered;
}

void CcButton::onEvent(EGuiEvent eEvent, void *pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcButton::onMouseEvent(EGuiEvent eEvent, CcMouseEvent* pMouseEvent)
{
  switch (eEvent)
  {
    case EGuiEvent::MouseLeftDown:
      onMouseClick(pMouseEvent);
      break;
    case EGuiEvent::MouseHover:
      onMouseHover(pMouseEvent);
      break;
    case EGuiEvent::MouseLeave:
      onMouseLeave(pMouseEvent);
      break;
    case EGuiEvent::MouseLeftDoubleClick:
      onMouseHover(pMouseEvent);
      break;
    default:
      break;
  }
}

void CcButton::onKeyEvent(EGuiEvent eEvent, CcKeyEvent* pKeyEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pKeyEvent);
}

void CcButton::onWindowEvent(EGuiEvent eWindowEvent)
{
  CCUNUSED(eWindowEvent);
}

void CcButton::onMouseHover(CcMouseEvent* pInputEvent)
{
  m_pPrivate->m_bIsHovered = true;
  draw();
  CCUNUSED(pInputEvent);
}

void CcButton::onMouseLeave(CcMouseEvent* pInputEvent)
{
  m_pPrivate->m_bIsHovered = false;
  draw();
  CCUNUSED(pInputEvent);
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
  IEvent* pEvent = CcEvent<CcButton::CPrivate, void>::create(m_pPrivate, &CcButton::CPrivate::setGeometryConvert);
  getWindow()->appendAction(CcEventAction(pEvent, nullptr));
}

void CcButton::onBackgroundChanged()
{
  // @todo TBD
}

void CcButton::onForegroundChanged()
{
  // @todo TBD
}

void CcButton::setText(const CcString& sString )
{
  m_pPrivate->sText = sString;
}

const CcString& CcButton::getString()
{
  return m_pPrivate->sText;
}
