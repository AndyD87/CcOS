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
 * @brief     Implementations of class CcText with qt features
 */

#include "CcText.h"
#include "CcWindow.h"
#include "CcEventAction.h"
#include "CcRectangle.h"

#include "CcQt.h"
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>

class CcText::CPrivate : public QLabel, public CcObject
{
public:
  CPrivate(CcText* pText, QWidget* pParent):
    QLabel(pParent),
    pText(pText)
  {
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
    pText->event(eType, &oMouseEvent);
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
    pText->event(eType, &oMouseEvent);
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
    pText->event(eType, &oMouseEvent);
  }

  virtual void enterEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    EEventType    eType = EEventType::MouseHover;
    CcMouseEvent oMouseEvent;
    oMouseEvent.eType = EEventType::MouseHover;
    pText->event(eType, nullptr);
  }

  virtual void leaveEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    EEventType    eType = EEventType::MouseLeave;
    CcMouseEvent oMouseEvent;
    oMouseEvent.eType = EEventType::MouseLeave;
    pText->event(eType, nullptr);
  }

  virtual bool event(QEvent* pEvent) override
  {
    bool bHandled = false;
    switch(pEvent->type())
    {
      case QEvent::Type::Resize:
        pText->setSize(ToCcSize(size()));
        break;
      default:
        bHandled = false;
    }

    if (!bHandled)
    {
      bHandled = QLabel::event(pEvent);
    }
    return bHandled;
  }

  void setGeometryConvert(void*)
  {
    this->setGeometry(ToQRect(pText->getRectangle()));
  }

  CcText*   pText;
  CcString  sString;   //!< String for Display
};

CcText::CPrivate::~CPrivate()
{}

CcText::CcText(CcWidget* rParent) :
  CcWidget(rParent)
{
  QWidget* pParent = nullptr;
  if (rParent)
    pParent = ToQWidget(rParent->getSubSysHandle());
  CCNEW(m_pPrivate, CPrivate, this, pParent);
  setSubSystemHandle(static_cast<void*>(m_pPrivate));
  setFontColor(0, 0, 0);
  setTextOffset(0,0);
}

CcText::~CcText() 
{
  CCDELETE(m_pPrivate);
}

void CcText::setFontColor(uchar R, uchar G, uchar B)
{
  setForegroundColor(CcColor(R,G,B));
}

void CcText::setTextOffset(uint16 x, uint16 y )
{
  // @todo
  CCUNUSED(x);
  CCUNUSED(y);
}

const CcString& CcText::getText()
{
  return m_pPrivate->sString;
}

void CcText::setText( const CcString& sString )
{
  m_pPrivate->setText(ToQString(sString));
  m_pPrivate->adjustSize();
  setRectangle(ToCcRectangle(m_pPrivate->geometry()));
  m_pPrivate->sString = sString;
}

void CcText::event(EEventType eEvent, void* pEventData)
{
  bool bHandled = false;
  if (eEvent == EEventType::WidgetStyleChanged)
  {
    CcStyle::EType eStyleEvent = *static_cast<CcStyle::EType*>(pEventData);
    if (getSubSysHandle())
    {
      switch (eStyleEvent)
      {
        case CcStyle::EType::BackgroundImage:
        {
          bHandled = true;
          QPixmap oPixmap;
          oPixmap.load(CcWidget::getStyle().sBackgroundImage.getCharString());
          QSize oSize = ToQSize( getSize());
          QPixmap oRatioPixmap = oPixmap.scaled(oSize, Qt::AspectRatioMode::KeepAspectRatio);
          ToQLabel(getSubSysHandle())->setPixmap(oRatioPixmap);
          ToQLabel(getSubSysHandle())->setAlignment(Qt::AlignCenter);
        }
        default:
          break;
      }
    }
  }
  if(!bHandled)
  {
    CcWidget::event(eEvent, pEventData);
  }
}
