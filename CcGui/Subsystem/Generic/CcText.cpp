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
 * @brief     Implementations of class CcText
 */

#include "CcText.h"
#include "CcWindow.h"

class CcText::CPrivate
{
public:
  CPrivate():
    oFont()
  {}

  CcFont    oFont;
  CcString  sString;   //!< String for Display
  CcColor   cFontColor;
  uint16    uiOffsetX;   //!< Position-Offest Y-Value Text is shown
  uint16    uiOffsetY;   //!< Position-Offest X-Value Text is shown
  uint16    TextSizeX;   //!< Calculated width in Pixles of showing Text
  uint16    TextSizeY;   //!< Calculated height in Pixles of showing Text
};

CcText::CcText(CcWidget* rParent) :
  CcWidget(rParent)
{
  CCNEW(m_pPrivate, CPrivate);
  setFontColor(0xff, 0xff, 0xff);
  setTextOffset(0,0);
}

CcText::~CcText() 
{
  CCDELETE(m_pPrivate);
}
/*
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
  uint16 xVal = m_pPrivate->uiOffsetX;
  uint16 yVal = m_pPrivate->uiOffsetY;
  CcRectangle oRectangle;
  for(uint16 i=0; i<m_pPrivate->sString.length(); i++)
  {
    oRectangle.setHeight(m_pPrivate->oFont.getFontSizeY());
    oRectangle.setWidth(m_pPrivate->oFont.getFontSizeX());
    oRectangle.setX(xVal);
    oRectangle.setY(yVal);
    if (setPixelArea(oRectangle))
    {
      switch (m_pPrivate->sString.at(i))
      {
        case '\n':
          xVal = m_pPrivate->uiOffsetX;
          yVal += m_pPrivate->oFont.getFontSizeY();
          break;
        case '\r':
          xVal = m_pPrivate->uiOffsetX;
          break;
        default:
          writeChar(m_pPrivate->sString.at(i));
          xVal += m_pPrivate->oFont.getFontSizeX();
      }
    }
  }
}
*/

void CcText::setFontColor(uchar R, uchar G, uchar B)
{
  setForegroundColor(CcColor(R,G,B));
}

void CcText::setTextOffset(uint16 x, uint16 y )
{
  m_pPrivate->uiOffsetX = x;
  m_pPrivate->uiOffsetY = y;
}

const CcString& CcText::getText()
{
  return m_pPrivate->sString;
}

void CcText::setText( const CcString& sString )
{
  m_pPrivate->sString = sString;
}

/*
void CcText::calcTextSize()
{
  m_pPrivate->TextSizeX=0;
  m_pPrivate->TextSizeY=1;
  uint16 tempX=0;
  for(uint16 i=0; i<m_pPrivate->sString.length(); i++)
  {
    switch(m_pPrivate->sString.at(i))
    {
      case '\n':
        m_pPrivate->TextSizeY++;
        // fall through
      case '\r':
        if(tempX > m_pPrivate->TextSizeX)
          m_pPrivate->TextSizeX=tempX;
        tempX=0;
        break;
      default:
        tempX++;
    }
  }
  if(tempX > m_pPrivate->TextSizeX)
    m_pPrivate->TextSizeX=tempX;
  m_pPrivate->TextSizeX *= m_pPrivate->oFont.getFontSizeX();
  m_pPrivate->TextSizeY *= m_pPrivate->oFont.getFontSizeY();
}

void CcText::getTextSize(uint16* x, uint16* y)
{
  *x = m_pPrivate->TextSizeX;
  *y = m_pPrivate->TextSizeY;
}
*/

void CcText::event(CcInputEvent* pEventData)
{
  bool bHandled = false;
  if(!bHandled)
  {
    CcWidget::event(pEventData);
  }
}
