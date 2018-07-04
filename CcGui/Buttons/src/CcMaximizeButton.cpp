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
 * @brief     Implemtation of class CcMaximizeButton
 */
#include "Buttons/CcMaximizeButton.h"
#include "CcPainter.h"

CcMaximizeButton::CcMaximizeButton(const CcWidgetHandle& pParent) :
  CcButton(pParent)
{
  setCustomPainting(true);
}

CcMaximizeButton::~CcMaximizeButton( void )
{
}

void CcMaximizeButton::draw(bool bDoFlush)
{
  CcButton::draw(bDoFlush);
  if (getWindowState() == EWindowState::Maximimized)
  {
    CcPainter oPainter(this);
    CcPoint oBegin;
    CcPoint oEnd;
    oPainter.setColor(getForegroundColor());
    CcRectangle oRectangle;
    oRectangle.setHeight(7);
    oRectangle.setWidth(8);
    oRectangle.setX(13);
    oRectangle.setY(5);
    oPainter.drawRectangle(oRectangle, 1, false);
    oRectangle.setX(8);
    oRectangle.setY(10);
    oPainter.drawRectangle(oRectangle, 1, false);
    oBegin.setX(13);
    oBegin.setY(4);
    oEnd.setX(20);
    oEnd.setY(4);
    oPainter.drawLine(oBegin, oEnd);
    oBegin.setX(8);
    oBegin.setY(9);
    oEnd.setX(15);
    oEnd.setY(9);
    oPainter.drawLine(oBegin, oEnd);
  }
  else
  {
    CcPainter oPainter(this);
    oPainter.setColor(getForegroundColor());
    CcRectangle oRectangle;
    oRectangle.setHeight(10);
    oRectangle.setWidth(12);
    oRectangle.setX((getWidth() / 2) - 6);
    oRectangle.setY((getHeight() / 2) - 5);
    oPainter.drawRectangle(oRectangle, 1, false);
    CcPoint oBegin;
    oBegin.setX((getWidth() / 2) - 6);
    oBegin.setY((getHeight() / 2) - 6);
    CcPoint oEnd;
    oEnd.setX((getWidth() / 2) + 5);
    oEnd.setY((getHeight() / 2) - 6);
    oPainter.drawLine(oBegin, oEnd);
  }
  if (bDoFlush)
    flush();
}

void CcMaximizeButton::onMouseClick(CcMouseEvent* pEvent)
{
  CCUNUSED(pEvent);
  if (getWindowState() == EWindowState::Normal)
    setWindowState(EWindowState::Maximimized);
  else if (getWindowState() == EWindowState::Maximimized)
    setWindowState(EWindowState::Normal);
}
