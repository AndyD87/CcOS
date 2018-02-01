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
 * @brief     Class CcWindow
 */

#include "CcWindow.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"
#include "Widgets/CcTitlebar.h"

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindowPrivate
{
public:
  CcDisplayAreaPointer        m_oDisplayArea = nullptr;
  CcHandle<CcDisplay>         m_oDisplay;
  CcSharedPointer<CcTitlebar> m_oTitlebarWidget = nullptr;
  CcWidgetHandle              m_pLastLeftButtonDown = nullptr;
  CcWidgetHandle              m_pLastRightButtonDown = nullptr;
  CcWidgetHandle              m_pLastMiddleButtonDown = nullptr;
  CcWidgetHandle              m_pLastHovered = nullptr;
};

CcWindow::CcWindow(void) :
  CcWidget(0, 0, 260, 320 - CcStyle::TitlebarHeight, nullptr)
{
  initWindowPrivate();
  m_pPrivate->m_oDisplay = CcKernel::getDevice(EDeviceType::Display).cast<CcDisplay>();
}

CcWindow::CcWindow(const CcHandle<CcDisplay>& oDisplay) :
  CcWidget(0, 0, 200, 300 - CcStyle::TitlebarHeight, nullptr)
{
  initWindowPrivate();
  m_pPrivate->m_oDisplay = oDisplay;
}

CcWindow::CcWindow(uint16 sizeX, uint16 sizeY, const CcHandle<CcDisplay>& oDisplay) :
  CcWidget(0, 0, sizeX, sizeY - CcStyle::TitlebarHeight, nullptr)
{
  initWindowPrivate();
  m_pPrivate->m_oDisplay = oDisplay;
}

CcWindow::~CcWindow() 
{
  CCDELETE(m_pPrivate);
}

bool CcWindow::init()
{
  return initWindow();
}

void CcWindow::loop(void)
{
  m_pPrivate->m_oDisplayArea->loop();
}

void CcWindow::close(void)
{
  m_pPrivate->m_oDisplayArea->close();
  m_oCloseHandler.call(nullptr);
}

bool CcWindow::setPixelArea(const CcRectangle& oRectangle)
{
  CcRectangle oArea = oRectangle;
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
    oArea += CcPoint(0, m_pPrivate->m_oTitlebarWidget->getHeight());
  if (m_pPrivate->m_oDisplayArea != nullptr)
  {
    return  m_pPrivate->m_oDisplayArea->setPixelArea(oArea);
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
      if (m_pPrivate->m_oTitlebarWidget != nullptr)
        m_oNormalRect += CcSize(0, m_pPrivate->m_oTitlebarWidget->getHeight());
      CcRectangle oMaxArea;
      if (m_pPrivate->m_oDisplayArea != nullptr)
        m_pPrivate->m_oDisplayArea->getMaxArea(oMaxArea);
      setWindowRect(oMaxArea);
      break;
    }
    case EWindowState::Normal:
      setWindowRect(m_oNormalRect);
      break;
    case EWindowState::Minimized:
      m_eLastState = m_eState;
      if (m_pPrivate->m_oDisplayArea != nullptr)
        m_pPrivate->m_oDisplayArea->hide();
      break;
    case EWindowState::Tray:
      if (m_pPrivate->m_oDisplayArea != nullptr)
         m_pPrivate->m_oDisplayArea->hide();
      break;
  }
  m_eState = eState;
}

void CcWindow::draw( void )
{
  if (m_pPrivate->m_oDisplayArea != nullptr)
  {
    CcRectangle oInnerRect = getInnerRect();
    uint32 uiTemp = oInnerRect.getAreaSize();
    if (m_pPrivate->m_oDisplayArea->setPixelArea(oInnerRect))
    {
      for (uint32 i = 0; i < uiTemp; i++)
        drawPixel(getBackgroundColor());
    }
    drawAllChilds();
    m_pPrivate->m_oDisplayArea->draw();
  }
}

void CcWindow::drawPixel(const CcColor& oColor)
{
  if (m_pPrivate->m_oDisplayArea != nullptr)
    m_pPrivate->m_oDisplayArea->drawPixel(oColor);
}

bool CcWindow::initWindow()
{
  setBackgroundColor(CcStyle::WindowBackgroundColor);
  if (m_pPrivate->m_oDisplay != nullptr)
  {
    m_pPrivate->m_oTitlebarWidget = new CcTitlebar(this);
    CCMONITORNEW(m_pPrivate->m_oTitlebarWidget.getPtr());
    setSize(getSize());
    m_pPrivate->m_oDisplayArea = m_pPrivate->m_oDisplay->getDisplayArea(getWindowRect());
    m_pPrivate->m_oDisplayArea->setDisplay(m_pPrivate->m_oDisplay);
    m_pPrivate->m_oDisplayArea->setWindowTitle(m_sWindowTitle);
    m_pPrivate->m_oDisplayArea->getInputEventHandler() += CcEventHandle(new CcEvent<CcWindow, CcInputEvent>(this, &CcWindow::eventInput));
    m_pPrivate->m_oDisplayArea->getControlEventHandler() += CcEventHandle(new CcEvent<CcWindow, EDisplayCommands>(this, &CcWindow::eventControl));
    return true;
  }
  return false;
}

void CcWindow::initWindowPrivate()
{
  CCDELETE(m_pPrivate);
  m_pPrivate = new CcWindowPrivate();
  CCMONITORNEW(m_pPrivate);
}

void CcWindow::onRectangleChanged()
{
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
  {
    m_pPrivate->m_oTitlebarWidget->setSize(CcSize(getWidth(), m_pPrivate->m_oTitlebarWidget->getHeight()));
  }
}

void CcWindow::setSize(const CcSize& oNewSize)
{
  CcSize oSize(oNewSize);
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
  {
    oSize -= CcSize(0, m_pPrivate->m_oTitlebarWidget->getHeight());
  }
  CcWidget::setSize(oSize);
  if (m_pPrivate->m_oDisplayArea != nullptr)
    m_pPrivate->m_oDisplayArea->setSize(oNewSize);
  onRectangleChanged();
  draw();
}

void CcWindow::setPos(const CcPoint& oPoint)
{
  CcWidget::setPos(oPoint);
  if (m_pPrivate->m_oDisplayArea != nullptr)
    m_pPrivate->m_oDisplayArea->setPos(oPoint);
}

void CcWindow::setWindowRect(const CcRectangle& oRect)
{
  CcRectangle oNewRect(oRect);
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
  {
    oNewRect -= CcSize(0, m_pPrivate->m_oTitlebarWidget->getHeight());
  }
  CcWidget::setWindowRect(oNewRect);
  if (m_pPrivate->m_oDisplayArea != nullptr)
    m_pPrivate->m_oDisplayArea->setArea(oRect);
  draw();
}

void CcWindow::setTitle(const CcString& sTitle)
{
  m_sWindowTitle = sTitle;
  if (m_pPrivate->m_oDisplayArea != nullptr)
  {
    m_pPrivate->m_oDisplayArea->setWindowTitle(sTitle);
  }
}

void CcWindow::eventControl(EDisplayCommands* eCommand)
{
  switch (*eCommand)
  {
    case EDisplayCommands::Close:
      close();
      break;
    case EDisplayCommands::Restore:
      m_eState = m_eLastState;
      break;
    case EDisplayCommands::Maximimized:
    case EDisplayCommands::Minimized:
    case EDisplayCommands::Normal:
    case EDisplayCommands::Tray:
    default:
      break;
  }
}

CcWidgetHandle CcWindow::getHitTest(const CcPoint& oPointToFind)
{
  CcWidgetHandle pRet = this;
  CcPoint pPoint(oPointToFind);
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
  {
    pPoint -= CcPoint(0, m_pPrivate->m_oTitlebarWidget->getHeight());
  }
  pRet = CcWidget::getHitTest(pPoint);
  return pRet;
}

void CcWindow::eventInput(CcInputEvent* pInputEvent)
{
  switch (pInputEvent->getType())
  {
    case EInputEventType::Mouse:
    {
      parseMouseEvent(pInputEvent->getMouseEvent());
      break;
    }
    case EInputEventType::Touch:
      break;
    case EInputEventType::Keyboard:
    case EInputEventType::Joystick:
    case EInputEventType::Undefined:
    default:
      break;
  }
}

void CcWindow::parseMouseEvent(CcMouseEvent& oMouseEvent)
{
  CcPoint pPoint(oMouseEvent.x, oMouseEvent.y);
  CcWidgetHandle pFound = getHitTest(pPoint);
  m_oMouseEventHandler.call(pFound.ptr(), &oMouseEvent);
}

