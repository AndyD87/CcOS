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
#include "CcFont.h"
#include "CcGuiGlobals.h"
#include "CcGuiSubsystem.h"

#include "CcQt.h"

INCLUDE_WARNING_LEVEL_START
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
INCLUDE_WARNING_LEVEL_STOP

class CcButton::CPrivate : public CcObject, public QPushButton
{
public:
  CPrivate(CcButton* pButton, QWidget* pParent):
    QPushButton(pParent),
    pButton(pButton),
    oFont()
  {
  }

  virtual ~CPrivate() override;

  virtual void enterEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    CcMouseEvent oMouseEvent;
    oMouseEvent.setType(EEventType::MouseHover);
    pButton->event(&oMouseEvent);
  }

  virtual void leaveEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    CcMouseEvent oMouseEvent;
    oMouseEvent.setType(EEventType::MouseLeave);
    pButton->event(&oMouseEvent);
  }

  virtual bool event(QEvent* pEvent) override
  {
    bool bHandled = false;
    CCNEWARRAYTYPE(pData, char, CCMAX(sizeof(CcMouseEvent), sizeof(CcKeyEvent)));
    CcStatic::memset(pData, 0, CCMAX(sizeof(CcMouseEvent), sizeof(CcKeyEvent)));
    if(CcGuiSubsystem::convertMouseEvent(pEvent, *CCVOIDPTRCAST(CcMouseEvent*, pData)))
    {

    }
    else if(CcGuiSubsystem::convertKeyEvent(pEvent, *CCVOIDPTRCAST(CcKeyEvent*, pData)))
    {

    }
    else
    {
      switch(pEvent->type())
      {
        case QEvent::Type::Resize:
          pButton->setSize(ToCcSize(size()));
          bHandled = true;
          break;
      }

      if (!bHandled)
      {
        bHandled = QPushButton::event(pEvent);
      }
    }
    CCDELETEARR(pData);
    return bHandled;
  }
  void setGeometryConvert(void*)
  {
    this->setGeometry(ToQRect(pButton->getRectangle()));
  }

  CcButton*     pButton;
  CcStyleButton oStyle;
  CcFont        oFont;
  bool          m_bIsHovered = false;
  bool          m_bIsActive = false;
  bool          m_bIsFocused = false;
  bool          m_bIsFocusable = false;
  CcColor       m_oHoverColor = CcStyle::ButtonHoverBackgroundColor;
  CcString      sText;
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
  QPushButton* pButton = static_cast<QPushButton*>(m_pPrivate);
  setSubSystemHandle(pButton);
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
  bool bHandled = false;
  switch (pEventData->getType())
  {
    case EEventType::StyleBackgroundColor:
    {
      bHandled = true;
      break;
    }
    case EEventType::StyleForegroundColor:
    {
      bHandled = true;
      break;
    }
  }
  // Forward to default handler
  if(!bHandled)
    CcWidget::onEvent(pEventData);
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
  CcEvent oEvent = NewCcEventType(CcButton::CPrivate, void, m_pPrivate, CcButton::CPrivate::setGeometryConvert);
  CcEventAction oAction(oEvent, nullptr);
  oAction.lock();
  getWindow()->appendAction(&oAction);
  oAction.lock();
  oAction.unlock();
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
