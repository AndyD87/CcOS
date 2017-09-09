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
 * @brief     Implemtation of class CcCloseButton
 */
#include "Buttons/CcCloseButton.h"
#include "CcPainter.h"

CcCloseButton::CcCloseButton(CcWidgetHandle pParent):
  CcButton(pParent)
{
  setBorderSize(0);
  setHoverColor(CcColor(0xc0, 0, 0));
}

CcCloseButton::~CcCloseButton( void )
{
  
}

void CcCloseButton::draw(void)
{
  CcButton::draw();
  CcCross oCross;
  oCross.setValues(10, 10, 3);
  oCross.setDiagonal(true);
  CcPoint oPoint;
  oPoint.setX((getWidth() - oCross.m_width) / 2);
  oPoint.setY((getHeight() - oCross.m_height) / 2);
  CcPainter oPainter((CcWidgetHandle)this);
  oPainter.setColor(getForegroundColor());
  oPainter.drawCross(oPoint, oCross);
}

void CcCloseButton::onMouseClick(const CcPoint& oPosition)
{
  CCUNUSED(oPosition);
  getWindow()->close();
}
