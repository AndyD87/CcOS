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
 * @brief     Implementations of class CcText with qt features
 */

#include "CcText.h"
#include "CcWindow.h"
#include "CcEventAction.h"
#include "CcRectangle.h"

#include "CcQt.h"
#include "CcGuiSubsystem.h"

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

  virtual void keyPressEvent(QKeyEvent* pKeyEvent) override
  {
    CcKeyEvent oKeyEvent(EEventType::KeyDown);
    oKeyEvent.uiKey = static_cast<uint32>(pKeyEvent->key());
    pText->event(&oKeyEvent);
  }

  virtual void keyReleaseEvent(QKeyEvent* pKeyEvent) override
  {
    CcKeyEvent oKeyEvent(EEventType::KeyUp);
    oKeyEvent.uiKey = static_cast<uint32>(pKeyEvent->key());
    pText->event(&oKeyEvent);
  }

  virtual void enterEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    CcMouseEvent oMouseEvent;
    oMouseEvent.setType(EEventType::MouseHover);
    pText->event(&oMouseEvent);
  }

  virtual void leaveEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    CcMouseEvent oMouseEvent;
    oMouseEvent.setType(EEventType::MouseLeave);
    pText->event(&oMouseEvent);
  }

  virtual bool event(QEvent* pEvent) override
  {
    bool bHandled = false;
    CCNEWARRAYTYPE(pData, char, CCMAX(sizeof(CcMouseEvent), sizeof(CcKeyEvent)));
    CcStatic::memset(pData, 0, CCMAX(sizeof(CcMouseEvent), sizeof(CcKeyEvent)));
    if(CcGuiSubsystem::convertMouseEvent(pEvent, *CCVOIDPTRCAST(CcMouseEvent*, pData)))
    {
      pText->event(CCVOIDPTRCAST(CcInputEvent*, pData));
    }
    else if(CcGuiSubsystem::convertKeyEvent(pEvent, *CCVOIDPTRCAST(CcKeyEvent*, pData)))
    {
      pText->event(CCVOIDPTRCAST(CcInputEvent*, pData));
    }
    else
    {
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
    }
    CCDELETEARR(pData);
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

void CcText::event(CcInputEvent* pEventData)
{
  bool bHandled = false;
  switch (pEventData->getType())
  {
    case EEventType::StyleBackgroundImage:
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
  if(!bHandled)
  {
    CcWidget::event(pEventData);
  }
}
