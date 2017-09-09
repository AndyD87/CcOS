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
 * @brief     Implemtation of class CcMinimizeButton
 */
#include "Buttons/CcMinimizeButton.h"
#include "CcPainter.h"

CcMinimizeButton::CcMinimizeButton(CcWidgetHandle pParent):
  CcButton(pParent)
{
  setBorderSize(0);
}

CcMinimizeButton::~CcMinimizeButton( void )
{
}

void CcMinimizeButton::draw(void)
{
  CcButton::draw();
  CcPainter oPainter(this);
  oPainter.setColor(getForegroundColor());
  CcPoint oBegin;
  oBegin.setX((getWidth() / 2) - 6);
  oBegin.setY((getHeight() / 2) + 6);
  CcPoint oEnd;
  oEnd.setX((getWidth() / 2) + 6);
  oEnd.setY((getHeight() / 2) + 6);
  oPainter.drawLine(oBegin, oEnd);
  CcPoint update(0, 1);
  oBegin += update;
  oEnd += update;
  oPainter.drawLine(oBegin, oEnd);
}

void CcMinimizeButton::onMouseClick(const CcPoint& oPosition)
{
  CCUNUSED(oPosition);
  getWindow()->setState(EWindowState::Minimized);
}