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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcText
 */

#include "CcText.h"
#include "Devices/CcTimer.h"
#include "CcWindow.h"

CcText::CcText(CcWidget* parent, uint16 fontSize) :
  CcWidget(parent),
  CcFont(fontSize)
{
  setFontColor(0xff, 0xff, 0xff);
  setTextOffset(0,0);
}

CcText::~CcText() {
}

void CcText::writeChar(char cValue)
{
  char* fontBuf = getPixles(cValue);
  char cToCompare;
  char cFont;
  for(uint16 y=0; y < m_FontSizeY; y++)
  {
    cToCompare = 0x01;
    cFont = fontBuf[y];
    for(uint16 x=0; x < m_FontSizeX; x++)
    {
      if(cFont & cToCompare)
        getWindow()->drawPixel(m_cFontColor);
      else
        getWindow()->drawPixel(getBackgroundColor());
      cToCompare = cToCompare << 1;
    }
  }
}

void CcText::drawString( void )
{
  uint16 xVal = m_uiOffsetX;
  uint16 yVal = m_uiOffsetY;
  CcRectangle oRectangle;
  for(uint16 i=0; i<m_sString.length(); i++)
  {
    oRectangle.setHeight(m_FontSizeY);
    oRectangle.setWidth(m_FontSizeX);
    oRectangle.setX(xVal);
    oRectangle.setY(yVal);
    if (setPixelArea(oRectangle))
    {
      switch (m_sString.at(i))
      {
        case '\n':
          xVal = m_uiOffsetX;
          yVal += m_FontSizeY;
          break;
        case '\r':
          xVal = m_uiOffsetX;
          break;
        default:
          writeChar(m_sString.at(i));
          xVal += m_FontSizeX;
      }
    }
  }
}

void CcText::setFontColor(uchar R, uchar G, uchar B)
{
  m_cFontColor.setColor(R, G, B);
}


void CcText::setTextOffset(uint16 x, uint16 y )
{
  m_uiOffsetX = x;
  m_uiOffsetY = y;
}

const CcString& CcText::getString(void)
{
  return m_sString;
}

void CcText::setString( const CcString& sString )
{
  m_sString = sString;
  calcTextSize();
}

void CcText::calcTextSize(void)
{
  m_TextSizeX=0;
  m_TextSizeY=1;
  uint16 tempX=0;
  for(uint16 i=0; i<m_sString.length(); i++)
  {
    switch(m_sString.at(i))
    {
      case '\n':
        m_TextSizeY++;
        //no break \n includes \r
      case '\r':
        if(tempX > m_TextSizeX)
          m_TextSizeX=tempX;
        tempX=0;
        break;
      default:
        tempX++;
    }
  }
  if(tempX > m_TextSizeX)
    m_TextSizeX=tempX;
  m_TextSizeX *= m_FontSizeX;
  m_TextSizeY *= m_FontSizeY;
}

void CcText::getTextSize(uint16* x, uint16* y)
{
  *x = m_TextSizeX;
  *y = m_TextSizeY;
}
