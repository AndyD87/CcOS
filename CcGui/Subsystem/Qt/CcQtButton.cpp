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
#include <QEvent>
#include <QMouseEvent>

class CcButton::CPrivate : public QPushButton, public CcObject
{
public:
  CPrivate(CcButton* pButton, QWidget* pParent):
    QPushButton(pParent),
    pButton(pButton),
    oFont(CcGuiGlobals::Defaults::FontSize)
  {
  }

  virtual ~CPrivate() override;

  virtual void mousePressEvent(QMouseEvent* pMouseEvent)
  {
    EEventType    eType = EEventType::MouseEvent;
    CcMouseEvent oMouseEvent;
    switch (pMouseEvent->button())
    {
      case Qt::MouseButton::LeftButton:
        eType = EEventType::MouseLeftDown;
        oMouseEvent.setLeft(true);
        break;
      case Qt::MouseButton::RightButton:
        eType = EEventType::MouseRightDown;
        oMouseEvent.setRight(true);
        break;
      case Qt::MouseButton::MiddleButton:
        eType = EEventType::MouseMiddleDown;
        oMouseEvent.setMiddle(true);
        break;
    }
    pButton->event(eType, &oMouseEvent);
  }

  virtual void mouseReleaseEvent(QMouseEvent* pMouseEvent)
  {
    EEventType    eType = EEventType::MouseEvent;
    CcMouseEvent oMouseEvent;
    switch (pMouseEvent->button())
    {
      case Qt::MouseButton::LeftButton:
        eType = EEventType::MouseLeftUp;
        oMouseEvent.setLeft(false);
        break;
      case Qt::MouseButton::RightButton:
        eType = EEventType::MouseRightUp;
        oMouseEvent.setRight(false);
        break;
      case Qt::MouseButton::MiddleButton:
        eType = EEventType::MouseMiddleUp;
        oMouseEvent.setMiddle(false);
        break;
    }
    pButton->event(eType, &oMouseEvent);
  }

  virtual bool event(QEvent* pEvent)
  {
    bool bHandled = false;
    switch (pEvent->type())
    {
      case QEvent::HoverEnter:
        CCFALLTHROUGH;
      case QEvent::HoverMove:
        pButton->event(EEventType::MouseHover, nullptr);
        bHandled = true;
        break;
      case QEvent::HoverLeave:
        pButton->event(EEventType::MouseLeave, nullptr);
        bHandled = true;
        break;
    }
    if (!bHandled)
    {
      bHandled = QPushButton::event(pEvent);
    }
    return bHandled;
  }

  
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
  setBorderColor(CcGuiGlobals::Defaults::ButtonBorderColor);
  setBorderSize(CcGuiGlobals::Defaults::ButtonBorderSize);
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

void CcButton::onEvent(EEventType eEvent, void *pMouseEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pMouseEvent);
}

void CcButton::onMouseEvent(EEventType eEvent, CcMouseEvent* pMouseEvent)
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
    default:
      break;
  }
}

void CcButton::onKeyEvent(EEventType eEvent, CcKeyEvent* pKeyEvent)
{
  CCUNUSED(eEvent);
  CCUNUSED(pKeyEvent);
}

void CcButton::onWindowEvent(EEventType eWindowEvent)
{
  CCUNUSED(eWindowEvent);
}

void CcButton::onMouseHover(CcMouseEvent* pInputEvent)
{
  m_pPrivate->m_bIsHovered = true;
  setBackgroundColor(CcColor(0,0,0));
  draw();
  CCUNUSED(pInputEvent);
}

void CcButton::onMouseLeave(CcMouseEvent* pInputEvent)
{
  m_pPrivate->m_bIsHovered = false;
  setBackgroundColor(CcColor(0xff, 0xff, 0xff));
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
  m_pPrivate->setText(ToQString(sString));
}

const CcString& CcButton::getText()
{
  return m_pPrivate->sText;
}
