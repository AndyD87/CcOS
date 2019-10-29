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

class CcText::CPrivate : public CcObject, public QLabel
{
public:
  CPrivate(uint16 uiFontSize, CcText* pText, QWidget* pParent):
    QLabel(pParent),
    oFont(uiFontSize),
    pText(pText)
  {}

  ~CPrivate()
  {}

  void setGeometryConvert(void*)
  {
    this->setGeometry(ToQRect(pText->getRectangle()));
  }

  CcFont    oFont;
  CcString  sString;   //!< String for Display
  CcColor   cFontColor;
  CcText*   pText;
};

CcText::CcText(CcWidget* rParent, uint16 uiFontSize) :
  CcWidget(rParent)
{
  QWidget* pParent = nullptr;
  if (rParent)
    pParent = ToQWidget(rParent->getSubSysHandle());
  CCNEW(m_pPrivate, CPrivate, uiFontSize, this, pParent);
  setSubSystemHandle(static_cast<void*>(m_pPrivate));
  setFontColor(0, 0, 0);
  setTextOffset(0,0);
}

CcText::~CcText() 
{
  CCDELETE(m_pPrivate);
}

void CcText::writeChar(char cValue)
{
  char* fontBuf = m_pPrivate->oFont.getPixles(cValue);
  char cToCompare;
  char cFont;
  for(uint16 y=0; y < m_pPrivate->oFont.getFontSizeY(); y++)
  {
    cToCompare = 0x01;
    cFont = fontBuf[y];
    for(uint16 x=0; x < m_pPrivate->oFont.getFontSizeX(); x++)
    {
      if(cFont & cToCompare)
        getWindow()->drawPixel(m_pPrivate->cFontColor, m_pPrivate->oFont.getFontSizeX());
      else
        getWindow()->drawPixel(getBackgroundColor());
      cToCompare = cToCompare << 1;
    }
  }
}

void CcText::drawString()
{
  m_pPrivate->update();
}

void CcText::setFontColor(uchar R, uchar G, uchar B)
{
  QPalette oPalette = m_pPrivate->palette();
  oPalette.setColor(m_pPrivate->foregroundRole(), QColor(R,G,B));
  m_pPrivate->setPalette(oPalette);

  m_pPrivate->cFontColor.setColor(R, G, B);
}

void CcText::setTextOffset(uint16 x, uint16 y )
{
  // @todo
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
