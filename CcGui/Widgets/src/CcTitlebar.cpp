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
 * @brief     Implemtation of class CcTitlebar
 */
#include "Widgets/CcTitlebar.h"
#include "Buttons/CcCloseButton.h"
#include "Buttons/CcMaximizeButton.h"
#include "Buttons/CcMinimizeButton.h"
#include "CcPainter.h"

CcTitlebar::CcTitlebar(CcWidget* pParent):
  CcWidget(0, -CcStyle::TitlebarHeight, pParent->getWidth(), CcStyle::TitlebarHeight,pParent)
{
  setMinimizeButton(true);
  setMaximizeButton(true);
  setCloseButton(true);
  onRectangleChanged();
  getWindow()->getMouseLeftDownHandler() += CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseLeftDown));
  getWindow()->getMouseLeftUpHandler() += CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseLeftUp));
  m_oOriginalRect = getWindowRect();
}

CcTitlebar::~CcTitlebar( void )
{
  setMinimizeButton(false);
  setMaximizeButton(false);
  setCloseButton(false);
}

void CcTitlebar::setMinimizeButton(bool bEnable)
{
  if (getMinimizeButton() && bEnable == false)
  {
    CCMONITORDELETE(m_oMinimizeButton); delete m_oMinimizeButton;
    m_oMinimizeButton = nullptr;
  }
  else if (getMinimizeButton() == false && bEnable)
  {
    m_oMinimizeButton = new CcMinimizeButton(this); CCMONITORNEW(m_oMinimizeButton);
  }
}

void CcTitlebar::setMaximizeButton(bool bEnable)
{
  if (getMaximizeButton() && bEnable == false)
  {
    CCMONITORDELETE(m_oMaximizeButton); delete m_oMaximizeButton;
    m_oMaximizeButton = nullptr;
  }
  else if (getMaximizeButton() == false && bEnable)
  {
    m_oMaximizeButton = new CcMaximizeButton(this); CCMONITORNEW(m_oMaximizeButton);
  }
}

void CcTitlebar::setCloseButton(bool bEnable)
{
  if (getCloseButton() && bEnable == false)
  {
    CCMONITORDELETE(m_oCloseButton); delete m_oCloseButton;
    m_oCloseButton = nullptr;
  }
  else if (getCloseButton() == false && bEnable)
  {
    CCMONITORNEW(m_oCloseButton); m_oCloseButton = new CcCloseButton(this);
  }
}

void CcTitlebar::setHeight(uint16 uiHeight)
{
  CcRectangle oNewRect(getWindowRect());
  m_uiHeight = uiHeight;
  oNewRect.setHeight(m_uiHeight);
  setWindowRect(oNewRect);
}

void CcTitlebar::draw()
{
  drawBackground();
  drawTopLine();
  CcWidget::draw();
}

void CcTitlebar::drawTopLine()
{
  CcPainter oPainter(this);
  oPainter.setColor(CcColor(0, 0, 255));
  for (uint16 i = 0; i < m_uiTopLineSize; i++)
  {
    oPainter.drawLine(CcPoint(0, i), CcPoint(getWidth(), i));
  }
}

void CcTitlebar::onRectangleChanged()
{
  uint16 uiDefaultButtonWidth = (uint16)1.5*getHeight();
  uint16 uiCurrentButtonPos = getWidth() - uiDefaultButtonWidth;
  uint16 uiCurrentButtonHeight = getHeight() - m_uiTopLineSize;
  uint16 uiButtonCounter = 1;
  if (m_oCloseButton != nullptr)
  {
    m_oCloseButton->setPos(CcPoint(uiCurrentButtonPos, m_uiTopLineSize));
    m_oCloseButton->setSize(CcSize(uiDefaultButtonWidth, uiCurrentButtonHeight));
    uiButtonCounter++;
    uiCurrentButtonPos -= uiDefaultButtonWidth;
  }
  if (m_oMaximizeButton != nullptr)
  {
    m_oMaximizeButton->setPos(CcPoint(uiCurrentButtonPos, m_uiTopLineSize));
    m_oMaximizeButton->setSize(CcSize(uiDefaultButtonWidth, uiCurrentButtonHeight));
    uiButtonCounter++;
    uiCurrentButtonPos -= uiDefaultButtonWidth;
  }
  if (m_oMinimizeButton != nullptr)
  {
    m_oMinimizeButton->setPos(CcPoint(uiCurrentButtonPos, m_uiTopLineSize));
    m_oMinimizeButton->setSize(CcSize(uiDefaultButtonWidth, uiCurrentButtonHeight));
    uiButtonCounter++;
    uiCurrentButtonPos -= uiDefaultButtonWidth;
  }
}

void CcTitlebar::onMouseLeftDown(CcMouseEvent* MouseEvent)
{
  m_bMouseDown = true;
  m_oMouseDownPoint.setPoint(MouseEvent->x, MouseEvent->y);
  getWindow()->setMouseMoveHandler() += CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseMove));
}

void CcTitlebar::onMouseLeftUp(CcMouseEvent* MouseEvent)
{
  CCUNUSED(MouseEvent);
  m_bMouseDown = false;
  setWindowRect(m_oOriginalRect);
  getWindow()->setMouseMoveHandler().removeObject(this);
}

void CcTitlebar::onMouseMove(CcMouseEvent* MouseEvent)
{
  if (m_bMouseDown)
  {
    CcPoint oMousePoint(MouseEvent->x, MouseEvent->y);
    CcPoint oNewPoint = getWindow()->getPos() + ( oMousePoint - m_oMouseDownPoint);
    getWindow()->setPos(oNewPoint);
  }
}
