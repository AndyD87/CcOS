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
 * @brief     Implementation of class CcPainter
 */
#include "CcPainter.h"

CcPainter::CcPainter(CcWidget* Window) :
  m_Widget(Window)
{
  setColor(0,0,0);
}

CcPainter::~CcPainter() 
{
  // todo Auto-generated destructor stub
}

void CcPainter::setColor(uint8 R, uint8 G, uint8 B)
{
  m_Color.setColor(R, G, B);
}

void CcPainter::setColor(const CcColor& Color)
{
  m_Color = Color;
}

void CcPainter::drawLine(const CcPoint& oStart, const CcPoint& oStop)
{
  int32 ySize;
  int32 xSize;
  int32 uiStartX;
  int32 uiStartY;
  int32  iMultiplier;
  if(oStart.getX() <= oStop.getX())
  {
    uiStartX = oStart.getX();
    uiStartY = oStart.getY();
    if(oStart.getY() <= oStop.getY())
    {
      iMultiplier = 1;
      ySize = oStop.getY() - oStart.getY();
      xSize = oStop.getX() - oStart.getX();
    }
    else
    {
      iMultiplier = -1;
      ySize = oStart.getY() - oStop.getY();
      xSize = oStart.getX() - oStop.getX();
    }
  }
  else
  {
    uiStartX = oStop.getX();
    uiStartY = oStop.getY();
    if(oStart.getY() <= oStop.getY())
    {
      iMultiplier = -1;
      ySize = oStop.getY() - oStart.getY();
      xSize = oStart.getX() - oStop.getX();
    }
    else
    {
      iMultiplier = 1;
      ySize = oStop.getY() - oStart.getY();
      xSize = oStart.getX() - oStop.getX();
    }
  }
  if (ySize == 0)
  {
    CcRectangle oRectangle;
    oRectangle.setPoint(uiStartX, uiStartY);
    oRectangle.setHeight(1);
    oRectangle.setWidth(xSize);
    if (m_Widget->setPixelArea(oRectangle))
    {
      m_Widget->drawPixel(m_Color, xSize);
    }
  }
  else
  {
    xSize++;
    ySize++;
    uint8 append = 0;
    float PixPerStep = (float)xSize / ySize;
    int32 modPixPerStep = (int32)PixPerStep;
    if (PixPerStep - (float)modPixPerStep > 0 ||
        PixPerStep == 0)
    {
      append = 1;
    }
    CcRectangle oRectangle;
    for (int32 i = 0; i< ySize; i++)
    {
      oRectangle.setY(uiStartY + (int32)(i*iMultiplier));
      oRectangle.setX(uiStartX + (int32)(i*PixPerStep));
      oRectangle.setHeight(1);
      oRectangle.setWidth((int32)PixPerStep + append);
      if (m_Widget->setPixelArea(oRectangle))
      {
        m_Widget->drawPixel(m_Color, oRectangle.getWidth());
      }
    }
  }
}

void CcPainter::drawCross(const CcPoint& position, const CcCross& oCross)
{
  if (oCross.isDiagonal())
  {
    if (oCross.getThickness() & 1)
    {
      int32 uiCount = (oCross.getThickness() - 1) / 2;
      int32 startX = position.getX();
      int32 startY = position.getY();
      drawLine(CcPoint(startX, startY), CcPoint(startX + oCross.getWidth(), startY + oCross.getHeight()));
      drawLine(CcPoint(startX + oCross.getWidth(), startY), CcPoint(startX, startY + oCross.getHeight()));
      for (int32 i = 1; i < uiCount+1; i++)
      {
        drawLine(CcPoint(startX, startY + i), CcPoint(startX + oCross.getWidth() - i, startY + oCross.getHeight()));
        drawLine(CcPoint(startX + i, startY), CcPoint(startX + oCross.getWidth(), startY + oCross.getHeight() - i));
        drawLine(CcPoint(startX + oCross.getWidth() - i, startY), CcPoint(startX, startY + oCross.getHeight() - i));
        drawLine(CcPoint(startX + oCross.getWidth(), startY + i), CcPoint(startX + i, startY + oCross.getHeight()));
      }
    }
    else
    {
      int32 uiCount = (oCross.getThickness() - 1) / 2;
      int32 startX = position.getX();
      int32 startY = position.getY();
      drawLine(CcPoint(startX, startY), CcPoint(startX + oCross.getWidth(), startY + oCross.getHeight()));
      drawLine(CcPoint(startX + oCross.getWidth(), startY), CcPoint(startX, startY + oCross.getHeight()));
      bool bDrawFirst = true;
      for (int32 i = 1; i < uiCount; i++)
      {
        drawLine(CcPoint(startX, startY + i), CcPoint(startX + oCross.getWidth() - i, startY + oCross.getHeight()));
        drawLine(CcPoint(startX + i, startY), CcPoint(startX + oCross.getWidth(), startY + oCross.getHeight() - i));
        if (bDrawFirst)
        {
          bDrawFirst = false;
        }
        {
          drawLine(CcPoint(startX + oCross.getWidth() - i, startY), CcPoint(startX, startY + oCross.getHeight() - i));
          drawLine(CcPoint(startX + oCross.getWidth(), startY + i), CcPoint(startX + i, startY + oCross.getHeight()));
        }
      }
    }
  }
  else
  {
    int32 startX = position.getX() + ((oCross.getWidth() - (oCross.getThickness() / 2)) / 2);
    int32 startY = position.getY() + ((oCross.getHeight() - (oCross.getThickness() / 2)) / 2);
    if (oCross.getThickness() & 1)
    {
      startX--;
      startY--;
    }
    int32 y = position.getY();
    int32 x = position.getX();
    for (int32 i = 0; i < oCross.getThickness(); i++)
    {
      CcPoint oStart(x, startY + i);
      CcPoint oStop(x + oCross.getWidth(), startY + i);
      drawLine(oStart, oStop);
      oStart.setPoint(startX + i, y);
      oStop.setPoint(startX + i, y + oCross.getWidth());
      drawLine(oStart, oStop);
    }
  }
}

void CcPainter::drawRectangle(const CcRectangle& oRectangle, int32 uiBorderSize, bool bFilled)
{
  if (bFilled)
  {
    if (m_Widget->setPixelArea(oRectangle))
    {
      m_Widget->drawPixel(m_Color, oRectangle.getAreaSize());
    }
  }
  else if(uiBorderSize > 0)
  {
    int32 uiHalfHeight = oRectangle.getHeight() / 2;
    int32 uiHalfWidth = oRectangle.getWidth() / 2;
    if (uiHalfHeight  <= uiBorderSize ||
        uiHalfWidth   <= uiBorderSize)
    {
      if (m_Widget->setPixelArea(oRectangle))
      {
        m_Widget->drawPixel(m_Color, oRectangle.getAreaSize());
      }
    }
    else
    {
      //write top border
      drawLine(oRectangle.getTopLeftCorner(), oRectangle.getTopRightCorner());
      //write right border
      drawLine(oRectangle.getTopLeftCorner(), oRectangle.getBottomLeftCorner());
      //write left border
      drawLine(oRectangle.getBottomRightCorner(), oRectangle.getBottomLeftCorner());
      //write bottom border
      drawLine(oRectangle.getBottomRightCorner(), oRectangle.getTopRightCorner());
    }
  }
}
