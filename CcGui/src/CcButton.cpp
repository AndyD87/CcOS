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
 * @brief     Class CcButton
 */
#include "CcButton.h"
#include "CcKernel.h"
#include "CcWindow.h"
#include "CcPainter.h"

CcButton::CcButton(CcWidget* parent) :
  CcWidget(parent)
{
  getWindow()->getMouseHoverHandler() += CcEventHandle(new CcEvent<CcButton, CcMouseEvent>(this, &CcButton::onMouseHover));
  getWindow()->getMouseLeaveHanlder() += CcEventHandle(new CcEvent<CcButton, CcMouseEvent>(this, &CcButton::onMouseLeave));
  getWindow()->getMouseClickHanlder() += CcEventHandle(new CcEvent<CcButton, CcMouseEvent>(this, &CcButton::onMouseClick));
  getWindow()->getMouseDoubleClickHanlder() += CcEventHandle(new CcEvent<CcButton, CcMouseEvent>(this, &CcButton::onMouseDoubleClick));
}

CcButton::~CcButton() 
{
}

void CcButton::draw()
{
  drawBorder();
  CcPainter oPainter(this);
  if (m_bIsHovered)
    oPainter.setColor(getHoverColor());
  else
    oPainter.setColor(getBackgroundColor());
  CcRectangle oRectangle = getWindowRect();
  oRectangle = CcPoint(0, 0);
  oPainter.drawRectangle(oRectangle, 0, true);
}

void CcButton::onMouseHover(CcMouseEvent* pInputEvent)
{
  m_bIsHovered = true;
  CcPoint oPoint(pInputEvent->x, pInputEvent->y);
  onMouseHover(oPoint);
  draw();
}

void CcButton::onMouseLeave(CcMouseEvent* pInputEvent)
{
  m_bIsHovered = false;
  CCUNUSED(pInputEvent);
  onMouseLeave();
  draw();
}

void CcButton::onMouseClick(CcMouseEvent* pInputEvent)
{
  CcPoint oPoint(pInputEvent->x, pInputEvent->y);
  onMouseClick(oPoint);
}

void CcButton::onMouseDoubleClick(CcMouseEvent* pInputEvent)
{
  CcPoint oPoint(pInputEvent->x, pInputEvent->y);
  onMouseDoubleClick(oPoint);
  draw();
}
