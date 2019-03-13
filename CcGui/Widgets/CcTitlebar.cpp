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
 * @brief     Implemtation of class CcTitlebar
 */
#include "Widgets/CcTitlebar.h"
#include "Buttons/CcCloseButton.h"
#include "Buttons/CcMaximizeButton.h"
#include "Buttons/CcMinimizeButton.h"
#include "CcPainter.h"

CcTitlebar::CcTitlebar(const CcWidgetHandle& pParent):
  CcWidget(0, 0, pParent->getWidth(), CcStyle::TitlebarHeight,pParent)
{
  m_oTitlebarStyle.oBackgroundColor = CcColor(0xff, 0xff, 0);
  setStyle(&m_oTitlebarStyle);
  setMinimizeButton(true);
  setMaximizeButton(true);
  setCloseButton(true);
  setBorderSize(0);
  onRectangleChanged();
  registerOnEvent(EGuiEvent::MouseLeftDown, CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseLeftDown)));
  registerOnEvent(EGuiEvent::MouseLeftUp, CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseLeftUp)));
  registerOnEvent(EGuiEvent::MouseLeave, CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseLeave)));
  registerOnEvent(EGuiEvent::MouseMove, CcEventHandle(new CcEvent<CcTitlebar, CcMouseEvent>(this, &CcTitlebar::onMouseMove)));
  m_oOriginalRect = getWindowRect();
}

CcTitlebar::~CcTitlebar()
{
  setMinimizeButton(false);
  setMaximizeButton(false);
  setCloseButton(false);
}

void CcTitlebar::setMinimizeButton(bool bEnable)
{
  if (getMinimizeButton() && bEnable == false)
  {
    CCDELETE(m_oMinimizeButton);
  }
  else if (getMinimizeButton() == false && bEnable)
  {
    m_oMinimizeButton = new CcMinimizeButton(this); 
    CCMONITORNEW(m_oMinimizeButton);
  }
}

void CcTitlebar::setMaximizeButton(bool bEnable)
{
  if (getMaximizeButton() && bEnable == false)
  {
    CCDELETE(m_oMaximizeButton);
  }
  else if (getMaximizeButton() == false && bEnable)
  {
    m_oMaximizeButton = new CcMaximizeButton(this); 
    CCMONITORNEW(m_oMaximizeButton);
  }
}

void CcTitlebar::setCloseButton(bool bEnable)
{
  if (getCloseButton() && bEnable == false)
  {
    CCDELETE(m_oCloseButton);
  }
  else if (getCloseButton() == false && bEnable)
  {
    m_oCloseButton = new CcCloseButton(this);
    CCMONITORNEW(m_oCloseButton);
  }
}

void CcTitlebar::setHeight(int32 uiHeight)
{
  CcRectangle oNewRect(getWindowRect());
  m_uiHeight = uiHeight;
  oNewRect.setHeight(m_uiHeight);
  setWindowRect(oNewRect);
}

void CcTitlebar::draw(bool bDoFlush)
{
  CcWidget::draw();
  drawTopLine();
  if (bDoFlush)
  {
    flush();
  }
}

void CcTitlebar::drawTopLine()
{
  CcPainter oPainter(this);
  oPainter.setColor(CcColor(0, 0, 255));
  for (int32 i = 0; i < m_uiTopLineSize; i++)
  {
    oPainter.drawLine(CcPoint(0, i), CcPoint(getWidth(), i));
  }
}

void CcTitlebar::onRectangleChanged()
{
  CcWidget::onRectangleChanged();
  int32 uiDefaultButtonWidth = (int32)1.5*getHeight();
  int32 uiCurrentButtonPos = getWidth() - uiDefaultButtonWidth;
  int32 uiCurrentButtonHeight = getHeight() - m_uiTopLineSize;
  int32 uiButtonCounter = 1;
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

void CcTitlebar::onMouseLeftDown(CcMouseEvent* pMouseEvent)
{
  if (getWindow()->getState() == EWindowState::Normal)
  {
    m_bMouseDown = true;
    m_oMouseDownPoint.setPoint(pMouseEvent->x, pMouseEvent->y);
    //getWindow()->registerOnEvent()
  }
}

void CcTitlebar::onMouseLeftUp(CcMouseEvent* pMouseEvent)
{
  if (getWindow()->getState() == EWindowState::Normal)
  {
    CCUNUSED(pMouseEvent);
    m_bMouseDown = false;
    setWindowRect(m_oOriginalRect);
  }
}

void CcTitlebar::onMouseMove(CcMouseEvent* pMouseEvent)
{
  if (getWindow()->getState() == EWindowState::Normal)
  {
    if (m_bMouseDown)
    {
      CcPoint oMousePoint(pMouseEvent->x, pMouseEvent->y);
      CcPoint oNewPoint = getWindow()->getPos() + (oMousePoint - m_oMouseDownPoint);
      getWindow()->setPos(oNewPoint);
    }
  }
}

void CcTitlebar::onMouseLeave(CcMouseEvent* pMouseEvent)
{
  CCUNUSED(pMouseEvent);
  if (m_bMouseDown)
  {
    //onMouseLeftUp(MouseEvent);
  }
}
