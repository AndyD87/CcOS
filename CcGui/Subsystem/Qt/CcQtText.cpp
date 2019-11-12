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

#include "CcQt.h"
#include <QLabel>

class CcText::CPrivate : public QLabel, public CcObject
{
public:
  CPrivate(CcText* pText, QWidget* pParent):
    QLabel(pParent),
    pText(pText)
  {
  }

  virtual ~CPrivate() override;

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

void CcText::drawString()
{
  m_pPrivate->update();
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

void CcText::onRectangleChanged()
{
  IEvent* pEvent = CcEvent<CcText::CPrivate, void>::create(m_pPrivate, &CcText::CPrivate::setGeometryConvert);
  getWindow()->appendAction(CcEventAction(pEvent, nullptr));
}

void CcText::setText( const CcString& sString )
{
  m_pPrivate->setText(ToQString(sString));
  m_pPrivate->adjustSize();
  getRectangle() = ToCcRectangle(m_pPrivate->geometry());
  m_pPrivate->sString = sString;
}
