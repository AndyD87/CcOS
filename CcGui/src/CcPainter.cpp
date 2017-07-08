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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
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
  int16 ySize;
  int16 xSize;
  uint16 uiStartX;
  uint16 uiStartY;
  int16  iMultiplier;
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
      for (uint16 k = 0; k < xSize; k++)
      {
        m_Widget->drawPixel(m_Color);
      }
    }
  }
  else
  {
    xSize++;
    ySize++;
    uint8 append = 0;
    float PixPerStep = (float)xSize / ySize;
    uint16 modPixPerStep = (uint16)PixPerStep;
    if (PixPerStep - (float)modPixPerStep > 0 ||
        PixPerStep == 0)
    {
      append = 1;
    }
    CcRectangle oRectangle;
    for (uint16 i = 0; i< ySize; i++)
    {
      oRectangle.setY(uiStartY + (int16)(i*iMultiplier));
      oRectangle.setX(uiStartX + (int16)(i*PixPerStep));
      oRectangle.setHeight(1);
      oRectangle.setWidth((uint16)PixPerStep + append);
      if (m_Widget->setPixelArea(oRectangle))
      {
        for (float k = 0; k < oRectangle.getWidth(); k += 1)
        {
          m_Widget->drawPixel(m_Color);
        }
      }
    }
  }
}

void CcPainter::drawCross(const CcPoint& position, const CcCross& cross)
{
  if (cross.m_bDiagonal)
  {
    if (cross.m_thick & 1)
    {
      uint16 uiCount = (cross.m_thick - 1) / 2;
      uint16 startX = position.getX();
      uint16 startY = position.getY();
      drawLine(CcPoint(startX, startY), CcPoint(startX + cross.m_width, startY + cross.m_height));
      drawLine(CcPoint(startX + cross.m_width, startY), CcPoint(startX, startY + cross.m_height));
      for (uint16 i = 1; i < uiCount+1; i++)
      {
        drawLine(CcPoint(startX, startY + i), CcPoint(startX + cross.m_width - i, startY + cross.m_height));
        drawLine(CcPoint(startX + i, startY), CcPoint(startX + cross.m_width, startY + cross.m_height - i));
        drawLine(CcPoint(startX + cross.m_width - i, startY), CcPoint(startX, startY + cross.m_height - i));
        drawLine(CcPoint(startX + cross.m_width, startY + i), CcPoint(startX + i, startY + cross.m_height));
      }
    }
    else
    {
      uint16 uiCount = (cross.m_thick - 1) / 2;
      uint16 startX = position.getX();
      uint16 startY = position.getY();
      drawLine(CcPoint(startX, startY), CcPoint(startX + cross.m_width, startY + cross.m_height));
      drawLine(CcPoint(startX + cross.m_width, startY), CcPoint(startX, startY + cross.m_height));
      bool bDrawFirst = true;
      for (uint16 i = 1; i < uiCount; i++)
      {
        drawLine(CcPoint(startX, startY + i), CcPoint(startX + cross.m_width - i, startY + cross.m_height));
        drawLine(CcPoint(startX + i, startY), CcPoint(startX + cross.m_width, startY + cross.m_height - i));
        if (bDrawFirst)
        {
          bDrawFirst = false;
        }
        {
          drawLine(CcPoint(startX + cross.m_width - i, startY), CcPoint(startX, startY + cross.m_height - i));
          drawLine(CcPoint(startX + cross.m_width, startY + i), CcPoint(startX + i, startY + cross.m_height));
        }
      }
    }
  }
  else
  {
    uint16 startX = position.getX() + ((cross.m_width - (cross.m_thick / 2)) / 2);
    uint16 startY = position.getY() + ((cross.m_height - (cross.m_thick / 2)) / 2);
    if (cross.m_thick & 1)
    {
      startX--;
      startY--;
    }
    uint16 y = position.getY();
    uint16 x = position.getX();
    for (uint16 i = 0; i < cross.m_thick; i++)
    {
      CcPoint oStart(x, startY + i);
      CcPoint oStop(x + cross.m_width, startY + i);
      drawLine(oStart, oStop);
      oStart.setPoint(startX + i, y);
      oStop.setPoint(startX + i, y + cross.m_width);
      drawLine(oStart, oStop);
    }
  }
}

void CcPainter::drawRectangle(const CcRectangle& oRectangle, uint16 uiBorderSize, bool bFilled)
{
  if (bFilled)
  {
    if (m_Widget->setPixelArea(oRectangle))
    {
      for (uint16 i = 0; i < oRectangle.getAreaSize(); i++)
      {
        m_Widget->drawPixel(m_Color);
      }
    }
  }
  else
  {
    uint16 uiHalfHeight = oRectangle.getHeight() / 2;
    uint16 uiHalfWidth = oRectangle.getWidth() / 2;
    if (uiHalfHeight  <= uiBorderSize ||
        uiHalfWidth   <= uiBorderSize)
    {
      if (m_Widget->setPixelArea(oRectangle))
      {
        for (uint16 i = 0; i < oRectangle.getAreaSize(); i++)
        {
          m_Widget->drawPixel(m_Color);
        }
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