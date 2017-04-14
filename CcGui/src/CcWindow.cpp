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
 * @file      CcWindow
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindow
 */

#include "CcWindow.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"
#include "Widgets/CcTitlebar.h"

CcWindow::CcWindow(void) :
  CcWidget(0, 0, 260, 320 - CcStyle::TitlebarHeight, nullptr)
{
  m_oDisplay = CcKernel::getDevice(EDeviceType::Display).cast<CcDisplay>();
}

CcWindow::CcWindow(const CcHandle<CcDisplay>& oDisplay) :
  CcWidget(0, 0, 200, 300 - CcStyle::TitlebarHeight, nullptr),
  m_oDisplay(oDisplay)
{
}

CcWindow::CcWindow(uint16 sizeX, uint16 sizeY, const CcHandle<CcDisplay>& oDisplay) :
  CcWidget(0, 0, sizeX, sizeY - CcStyle::TitlebarHeight, nullptr),
  m_oDisplay(oDisplay)
{
}

CcWindow::~CcWindow() 
{
  if (m_oDisplayArea != nullptr)
  {
    CCMONITORDELETE(m_oDisplayArea); delete m_oDisplayArea;
  }
}

bool CcWindow::init()
{
  return initWindow();
}

void CcWindow::close(void)
{
  m_oCloseHandler.call(nullptr);
}

bool CcWindow::setPixelArea(const CcRectangle& oRectangle)
{
  CcRectangle oArea = oRectangle;
  if (m_oTitlebarWidget != nullptr)
    oArea += CcPoint(0, m_oTitlebarWidget->getHeight());
  if (m_oDisplayArea != nullptr)
  {
    return m_oDisplayArea->setPixelArea(oArea);
  }
  return false;
}

void CcWindow::setState(EWindowState eState)
{
  switch (eState)
  {
    case EWindowState::Maximimized:
    {
      m_oNormalRect = getWindowRect();
      if (m_oTitlebarWidget != nullptr)
        m_oNormalRect += CcSize(0, m_oTitlebarWidget->getHeight());
      CcRectangle oMaxArea;
      if (m_oDisplayArea != nullptr)
        m_oDisplayArea->getMaxArea(oMaxArea);
      setWindowRect(oMaxArea);
      break;
    }
    case EWindowState::Normal:
      setWindowRect(m_oNormalRect);
      break;
    case EWindowState::Minimized:
      m_eLastState = m_eState;
      if (m_oDisplayArea != nullptr)
        m_oDisplayArea->hide();
      break;
    case EWindowState::Tray:
      if (m_oDisplayArea != nullptr)
        m_oDisplayArea->hide();
      break;
  }
  m_eState = eState;
}

void CcWindow::draw( void )
{
  if (m_oDisplayArea != nullptr)
  {
    CcRectangle oInnerRect = getInnerRect();
    uint32 uiTemp = oInnerRect.getAreaSize();
    if (m_oDisplayArea->setPixelArea(oInnerRect))
    {
      for (uint32 i = 0; i < uiTemp; i++)
        drawPixel(getBackgroundColor());
    }
    drawAllChilds();
    m_oDisplayArea->draw();
  }
}

void CcWindow::drawPixel(const CcColor& oColor)
{
  if (m_oDisplayArea != nullptr)
    m_oDisplayArea->drawPixel(oColor);
}

bool CcWindow::initWindow()
{
  setBackgroundColor(CcStyle::WindowBackgroundColor);
  if (m_oDisplay != nullptr)
  {
    m_oTitlebarWidget = new CcTitlebar(this); CCMONITORNEW(m_oTitlebarWidget);
    setSize(getSize());
    m_oDisplayArea = m_oDisplay->getDisplayArea(getWindowRect());
    m_oDisplayArea->setDisplay(m_oDisplay);
    m_oDisplayArea->setWindowTitle(m_sWindowTitle);
    m_oDisplayArea->getInputEventHandler() += CcEventHandle(new CcEvent<CcWindow, CcInputEvent>(this, &CcWindow::eventInput));
    m_oDisplayArea->getControlEventHandler() += CcEventHandle(new CcEvent<CcWindow, EDisplayCommands>(this, &CcWindow::eventControl));
    return true;
  }
  return false;
}

void CcWindow::onRectangleChanged()
{
  if (m_oTitlebarWidget != nullptr)
  {
    m_oTitlebarWidget->setSize(CcSize(getWidth(), m_oTitlebarWidget->getHeight()));
  }
}

void CcWindow::setSize(const CcSize& oNewSize)
{
  CcSize oSize(oNewSize);
  if (m_oTitlebarWidget != nullptr)
  {
    oSize -= CcSize(0, m_oTitlebarWidget->getHeight());
  }
  CcWidget::setSize(oSize);
  if (m_oDisplayArea != nullptr)
    m_oDisplayArea->setSize(oNewSize);
  onRectangleChanged();
  draw();
}

void CcWindow::setPos(const CcPoint& oPoint)
{
  CcWidget::setPos(oPoint);
  if (m_oDisplayArea != nullptr)
    m_oDisplayArea->setPos(oPoint);
}

void CcWindow::setWindowRect(const CcRectangle& oRect)
{
  CcRectangle oNewRect(oRect);
  if (m_oTitlebarWidget != nullptr)
  {
    oNewRect -= CcSize(0, m_oTitlebarWidget->getHeight());
  }
  CcWidget::setWindowRect(oNewRect);
  if (m_oDisplayArea != nullptr)
    m_oDisplayArea->setArea(oRect);
  draw();
}

void CcWindow::setTitle(const CcString& sTitle)
{
  m_sWindowTitle = sTitle;
  if (m_oDisplayArea != nullptr)
  {
    m_oDisplayArea->setWindowTitle(sTitle);
  }
}

void CcWindow::eventControl(EDisplayCommands* eCommand)
{
  switch (*eCommand)
  {
    case EDisplayCommands::Close:
      m_oCloseHandler.call(nullptr);
      break;
    case EDisplayCommands::Restore:
      m_eState = m_eLastState;
      break;
  }
}

CcWidget* CcWindow::getHitTest(const CcPoint& oPointToFind)
{
  CcWidget* pRet = this;
  CcPoint pPoint(oPointToFind);
  if (m_oTitlebarWidget != nullptr)
  {
    pPoint -= CcPoint(0, m_oTitlebarWidget->getHeight());
  }
  pRet = CcWidget::getHitTest(pPoint);
  return pRet;
}

void CcWindow::eventInput(CcInputEvent* pInputEvent)
{
  switch (pInputEvent->getType())
  {
    case EInputEventType::Keyboard:
      break;
    case EInputEventType::Mouse:
    {
      parseMouseEvent(pInputEvent->getMouseEvent());
      break;
    }
    case EInputEventType::Touch:
      break;
  }
}

void CcWindow::parseMouseEvent(CcMouseEvent& oMouseEvent)
{
  switch (oMouseEvent.eType)
  {
    case EMouseEventType::LeftDown:
    {
      CcPoint pPoint(oMouseEvent.x, oMouseEvent.y);
      CcWidget* pFound = getHitTest(pPoint);
      m_pLastLeftButtonDown = pFound;
      m_oMouseLeftDownEvents.call(pFound, &oMouseEvent);
      break;
    }
    case EMouseEventType::LeftUp:
    {
      CcPoint pPoint(oMouseEvent.x, oMouseEvent.y);
      CcWidget* pFound = getHitTest(pPoint);
      if (m_pLastLeftButtonDown != nullptr)
      {
        if (m_pLastLeftButtonDown == pFound)
        {
          m_oMouseClickEvents.call(pFound, &oMouseEvent);
          m_pLastLeftButtonDown = nullptr;
        }
      }
      m_oMouseLeftUpEvents.call(pFound, &oMouseEvent);
    }
    case EMouseEventType::Move:
    {
      CcPoint pPoint(oMouseEvent.x, oMouseEvent.y);
      CcWidget* pFound = getHitTest(pPoint);
      if (m_pLastHovered != nullptr &&
          m_pLastHovered != pFound)
      {
        m_oMouseLeaveEvents.call(m_pLastHovered, &oMouseEvent);
      }
      m_oMouseHoverEvents.call(pFound, &oMouseEvent);
      m_oMouseMoveEvents.call(&oMouseEvent);
      m_pLastHovered = pFound;
      break;
    }
    case EMouseEventType::Leave:
    {
      if (m_pLastHovered != nullptr)
      {
        m_oMouseLeaveEvents.call(m_pLastHovered, &oMouseEvent);
      }
      break;
    }
  }
}

