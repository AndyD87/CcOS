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
    CCNEW(pStyle, CcStyleButton);
  }

  virtual ~CPrivate() override;

  virtual void mousePressEvent(QMouseEvent* pMouseEvent) override
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
      default:
        break;
    }
    pButton->event(eType, &oMouseEvent);
  }

  virtual void mouseReleaseEvent(QMouseEvent* pMouseEvent) override
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
      default:
        break;
    }
    pButton->event(eType, &oMouseEvent);
  }

  virtual void mouseDoubleClickEvent(QMouseEvent* pMouseEvent) override
  {
    EEventType    eType = EEventType::MouseEvent;
    CcMouseEvent oMouseEvent;
    switch (pMouseEvent->button())
    {
      case Qt::MouseButton::LeftButton:
        eType = EEventType::MouseLeftDoubleClick;
        oMouseEvent.setLeft(true);
        break;
      case Qt::MouseButton::RightButton:
        eType = EEventType::MouseRightDoubleClick;
        oMouseEvent.setRight(true);
        break;
      case Qt::MouseButton::MiddleButton:
        eType = EEventType::MouseMiddleDoubleClick;
        oMouseEvent.setMiddle(true);
        break;
      default:
        break;
    }
    pButton->event(eType, &oMouseEvent);
  }

  virtual void enterEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    EEventType    eType = EEventType::MouseHover;
    CcMouseEvent oMouseEvent;
    oMouseEvent.eType = EEventType::MouseHover;
    pButton->event(eType, nullptr);
  }

  virtual void leaveEvent(QEvent* pEvent)
  {
    CCUNUSED(pEvent);
    EEventType    eType = EEventType::MouseLeave;
    CcMouseEvent oMouseEvent;
    oMouseEvent.eType = EEventType::MouseLeave;
    pButton->event(eType, nullptr);
  }

  virtual bool event(QEvent* pEvent) override
  {
    bool bHandled = false;
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
  CcStyleButton* pStyle;
  CcFont    oFont;
  bool      m_bIsHovered = false;
  bool      m_bIsActive = false;
  bool      m_bIsFocused = false;
  bool      m_bIsFocusable = false;
  CcColor   m_oHoverColor = CcStyle::ButtonHoverBackgroundColor;
  CcString  sText;
}; 

CcButton::CPrivate::~CPrivate()
{
  CCDELETE(pStyle);
}

CcButton::CcButton( CcWidget* rParent) :
  CcWidget(rParent)
{
  QWidget* pParent = nullptr;
  if (rParent)
    pParent = ToQWidget(rParent->getSubSysHandle());
  CCNEW(m_pPrivate, CPrivate, this, pParent);
  setSubSystemHandle(static_cast<void*>(m_pPrivate));
  setBorderSize(CcStyle::ButtonBorderSize);
  setBorderColor(CcStyle::ButtonBorderColor);
  setForegroundColor(CcStyle::ButtonForegroundColor);
  setBackgroundColor(CcStyle::ButtonBackgroundColor);
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
  m_pPrivate->setAutoFillBackground(true);
  m_pPrivate->update();
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
  getStyle()->oHoverForegroundColor = oForegroundColor;
  getStyle()->oHoverBackgroundColor = oBackgroundColor;
  getStyle()->oHoverBorderColor = oBorderColor;
  getStyle()->uiHoverBorderSize = uiBorderSize;
  getStyle()->bHoverActive = bActive;
  CcStyle::EType eType = CcStyle::EType::HoverColor;
  event(EEventType::WidgetStyleChanged, &eType);
}

CcStyleButton* CcButton::getStyle()
{
  return m_pPrivate->pStyle;
}

const CcStyleButton* CcButton::getStyle() const
{
  return m_pPrivate->pStyle;
}

void CcButton::onEvent(EEventType eEvent, void *pEvent)
{
  switch (eEvent)
  {
    case EEventType::WidgetStyleChanged:
    {
      CcStyle::EType* pType = static_cast<CcStyle::EType*>(pEvent);
      switch (*pType)
      {
        case CcStyle::EType::BackgroundColor:
        {
          QPalette oPalette = m_pPrivate->palette();
          oPalette.setColor(QPalette::Button, ToQColor(CcWidget::getStyle()->oBackgroundColor));
          m_pPrivate->setPalette(oPalette);
          draw();
          break;
        }
        case CcStyle::EType::ForegroundColor:
        {
          QPalette oPalette = m_pPrivate->palette();
          oPalette.setColor(QPalette::ButtonText, ToQColor(CcWidget::getStyle()->oForegroundColor));
          m_pPrivate->setPalette(oPalette);
          draw();
          break;
        }
        default:
          break;
      }
      break;
    }
    default:
      CcWidget::onEvent(eEvent, pEvent);
      break;
  }
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
  CCUNUSED(pInputEvent);
  m_pPrivate->m_bIsHovered = true;
  if (getStyle()->bHoverActive)
  {
    QPalette oPalette = m_pPrivate->palette();
    oPalette.setColor(QPalette::Button, ToQColor(getStyle()->oHoverBackgroundColor));
    oPalette.setColor(QPalette::ButtonText, ToQColor(getStyle()->oHoverForegroundColor));
    m_pPrivate->setPalette(oPalette);
    draw();
  }
}

void CcButton::onMouseLeave(CcMouseEvent* pInputEvent)
{
  CCUNUSED(pInputEvent);
  m_pPrivate->m_bIsHovered = false;
  if (getStyle()->bHoverActive)
  {
    QPalette oPalette = m_pPrivate->palette();
    oPalette.setColor(QPalette::Button, ToQColor(CcWidget::getStyle()->oBackgroundColor));
    oPalette.setColor(QPalette::ButtonText, ToQColor(CcWidget::getStyle()->oForegroundColor));
    m_pPrivate->setPalette(oPalette);
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
  IEvent* pEvent = CcEvent<CcButton::CPrivate, void>::create(m_pPrivate, &CcButton::CPrivate::setGeometryConvert);
  getWindow()->appendAction(CcEventAction(pEvent, nullptr));
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
