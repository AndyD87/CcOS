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
#include "CcGuiSubSystem.h"

typedef class CcSharedPointer<CcGuiSubSystem> CcGuiSubSystemPointer;

class CcWindowsGuiMainWidget : public CcWidget
{
public:
  CcWindowsGuiMainWidget() : CcWidget(nullptr)
    { }

  virtual void onRectangleChanged()
  { }
};

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindowPrivate
{
public:
  CcGuiSubSystemPointer       m_oGuiSubSystem = nullptr;
  CcSharedPointer<CcTitlebar> m_oTitlebarWidget = nullptr;
  CcSharedPointer<CcWindowsGuiMainWidget>   m_oMainWidget = nullptr;
  CcWidgetHandle              m_hMainWidget = nullptr;
  CcWidgetHandle              m_pLastLeftButtonDown = nullptr;
  CcWidgetHandle              m_pLastRightButtonDown = nullptr;
  CcWidgetHandle              m_pLastMiddleButtonDown = nullptr;
  CcWidgetHandle              m_pLastHovered = nullptr;
};

CcWindowHandle CcWindow::Null(nullptr);

CcWindow::CcWindow() :
  m_hThis(this),
  m_oNormalRect(0, 0, 260, 320)
{
  initWindowPrivate();
}

CcWindow::CcWindow(uint16 sizeX, uint16 sizeY) :
  m_hThis(this),
  m_oNormalRect(0, 0, sizeX, sizeY)
{
  initWindowPrivate();
}

CcWindow::~CcWindow() 
{
  CCDELETE(m_pPrivate);
}

bool CcWindow::init()
{
  return initWindow();
}

void CcWindow::loop()
{
  m_pPrivate->m_oGuiSubSystem->loop();
}

CcRectangle CcWindow::getInnerRect()
{
  CcRectangle oRect;
  if (m_pPrivate->m_oGuiSubSystem->hasFrame())
  {
    oRect = m_pPrivate->m_oGuiSubSystem->getInnerRect();
    oRect.setPoint(CcPoint(0, 0));
  }
  else
  {
    oRect = m_oNormalRect;
  }
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
  {
    oRect.addHeight(-m_pPrivate->m_oTitlebarWidget->getHeight());
    oRect.addX(m_pPrivate->m_oTitlebarWidget->getHeight());
  }
  return oRect;
}

bool CcWindow::setPixelArea(const CcRectangle& oRectangle)
{
  CcRectangle oArea = oRectangle;
  if (m_pPrivate->m_oGuiSubSystem != nullptr)
  {
    return  m_pPrivate->m_oGuiSubSystem->setPixelArea(oArea);
  }
  return false;
}

void CcWindow::setWindowState(EWindowState eState)
{
  m_pPrivate->m_oGuiSubSystem->setWindowState(eState);
  m_eState = eState;
}

CcWidgetHandle& CcWindow::getHandle()
{
  return m_pPrivate->m_hMainWidget;
}

void CcWindow::draw()
{
  if (m_pPrivate->m_hMainWidget != nullptr)
  {
    m_pPrivate->m_hMainWidget->draw(false);
  }
}

void CcWindow::drawPixel(const CcColor& oColor, uint64 uiNumber)
{
  if (m_pPrivate->m_oGuiSubSystem != nullptr)
    m_pPrivate->m_oGuiSubSystem->drawPixel(oColor, uiNumber);
}

bool CcWindow::initWindow()
{
  m_pPrivate->m_oGuiSubSystem = CcGuiSubSystem::create(this);
  if (m_pPrivate->m_oGuiSubSystem != nullptr &&
      m_pPrivate->m_oGuiSubSystem->open())
  {
    if (!m_pPrivate->m_oGuiSubSystem->hasFrame())
    {
      m_pPrivate->m_oTitlebarWidget = new CcTitlebar(nullptr);
      CCMONITORNEW(m_pPrivate->m_oTitlebarWidget.getPtr());
    }

    m_pPrivate->m_oMainWidget = new CcWindowsGuiMainWidget();
    m_pPrivate->m_oMainWidget->setSubSystemHandle(m_pPrivate->m_oGuiSubSystem->getHandle());
    m_pPrivate->m_oMainWidget->setBackgroundColor(CcColor(0, 255, 255));
    m_pPrivate->m_hMainWidget = m_pPrivate->m_oMainWidget.handle().cast<CcWidget>();
    CCMONITORNEW(m_pPrivate->m_oMainWidget.getPtr());

    m_pPrivate->m_oGuiSubSystem->setWindowTitle(m_sWindowTitle);
    m_pPrivate->m_oGuiSubSystem->getInputEventHandler() += NewCcEvent(CcWindow, CcInputEvent, CcWindow::eventInput, this);
    m_pPrivate->m_oGuiSubSystem->getControlEventHandler() += NewCcEvent(CcWindow, EGuiEvent, CcWindow::eventControl, this);
    return true;
  }
  return false;
}


void CcWindow::initWindowPrivate()
{
  CCDELETE(m_pPrivate);
  m_pPrivate = new CcWindowPrivate();
  CCMONITORNEW(m_pPrivate);
  m_oWindowStyle.oBackgroundColor = CcColor(0, 0, 0);
  m_oWindowStyle.oForegroundColor = CcColor(0, 0, 0);
  m_oWindowStyle.oBorderColor = CcColor(0xff, 0xff, 0xff);
  m_oWindowStyle.uBorderSize = 0;
}

void CcWindow::onRectangleChanged()
{
  if (m_pPrivate->m_oGuiSubSystem != nullptr)
  {
    m_pPrivate->m_oGuiSubSystem->updateSize();
    m_pPrivate->m_oGuiSubSystem->updatePos();
  }
  if (m_pPrivate->m_oTitlebarWidget != nullptr)
  {
    CcSize oRect = m_pPrivate->m_oTitlebarWidget->getSize();
    oRect.setWidth(getWidth());
    m_pPrivate->m_oTitlebarWidget->setSize(oRect);
  }
  if (m_pPrivate->m_oMainWidget != nullptr)
  {
    CcRectangle oMainWidgetRect = getInnerRect();
    m_pPrivate->m_oMainWidget->setWindowRect(oMainWidgetRect);
  }
}

void CcWindow::setTitle(const CcString& sTitle)
{
  m_sWindowTitle = sTitle;
  if (m_pPrivate->m_oGuiSubSystem != nullptr)
  {
    m_pPrivate->m_oGuiSubSystem->setWindowTitle(sTitle);
  }
}

void CcWindow::setSize(const CcSize& oSize)
{
  m_oNormalRect.setSize(oSize);
  onRectangleChanged();
}

void CcWindow::setPos(const CcPoint& oPos)
{
  m_oNormalRect.setPoint(oPos);
  onRectangleChanged();
}

void CcWindow::eventControl(EGuiEvent* eCommand)
{
  switch (*eCommand)
  {
    case EGuiEvent::WindowClose:
      setWindowState(EWindowState::Close);
      break;
    case EGuiEvent::WindowRestore:
      m_eState = m_eLastState;
      break;
    case EGuiEvent::WindowMaximimized:
      setWindowState(EWindowState::Maximimized);
      break;
    case EGuiEvent::WindowMinimized:
      setWindowState(EWindowState::Minimized);
      break;
    case EGuiEvent::WindowNormal:
      setWindowState(EWindowState::Normal);
      break;
    case EGuiEvent::WindowTray:
      setWindowState(EWindowState::Tray);
      break;
    case EGuiEvent::WindowSize:
      setWindowState(EWindowState::Tray);
      break;
    default:
      break;
  }
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
  CcWidgetHandle& pFound = m_pPrivate->m_hMainWidget->getHitTest(pPoint);
  m_oMouseEventHandler.call(pFound.ptr(), &oMouseEvent);
}

CcWindowHandle& CcWindow::getWindow()
{
  return m_hThis;
}
