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
#include "CcGuiSubsystem.h"

typedef class CcSharedPointer<CcGuiSubsystem> CcGuiSubsystemPointer;

class CcWindowsGuiMainWidget : public CcWidget
{
public:
  CcWindowsGuiMainWidget() : CcWidget(nullptr)
    {}
  virtual void onRectangleChanged()
    {}
};

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindow::CPrivate
{
public:
  CcGuiSubsystemPointer       m_oGuiSubSystem = nullptr;
  CcSharedPointer<CcTitlebar> m_oTitlebarWidget = nullptr;
  CcSharedPointer<CcWindowsGuiMainWidget>   m_oMainWidget = nullptr;
  CcWidgetHandle              m_hMainWidget = nullptr;
  CcWidgetHandle              m_pLastLeftButtonDown = nullptr;
  CcWidgetHandle              m_pLastRightButtonDown = nullptr;
  CcWidgetHandle              m_pLastMiddleButtonDown = nullptr;
  CcWidgetHandle              m_pLastHovered = nullptr;

  CcWindowHandle      m_hThis;
  CcString            m_sWindowTitle;
  CcRectangle         m_oNormalRect;
  EWindowState        m_eState = EWindowState::Normal;
  EWindowState        m_eLastState = EWindowState::Normal;
  CcEventHandler      m_oCloseHandler;
  CcMouseEventHandler m_oMouseEventHandler;
  CcStyleWidget       m_oWindowStyle;
};

CcWindowHandle CcWindow::Null(nullptr);

CcWindow::CcWindow()
{
  initWindowPrivate();
  m_pPrivate->m_hThis = this;
  m_pPrivate->m_oNormalRect.set(0, 0, 260, 320);
}

CcWindow::CcWindow(uint16 sizeX, uint16 sizeY)
{
  initWindowPrivate();
  m_pPrivate->m_hThis = this;
  m_pPrivate->m_oNormalRect.set(0, 0, sizeX, sizeY);
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
    oRect = m_pPrivate->m_oNormalRect;
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
  m_pPrivate->m_eState = eState;
}

CcWidgetHandle& CcWindow::getHandle()
{
  return m_pPrivate->m_hMainWidget;
}

CcEventHandler& CcWindow::getCloseHandler()
{
  return m_pPrivate->m_oCloseHandler;
}

CcMouseEventHandler& CcWindow::getMouseEventHandler()
{
  return m_pPrivate->m_oMouseEventHandler;
}

const CcString& CcWindow::getTitle()
{
  return m_pPrivate->m_sWindowTitle;
}

const CcSize& CcWindow::getSize() const
{
  return m_pPrivate->m_oNormalRect.getSize();
}

const CcPoint& CcWindow::getPos() const
{
  return m_pPrivate->m_oNormalRect.getPoint();
}

int32 CcWindow::getHeight() const
{
  return m_pPrivate->m_oNormalRect.getHeight();
}

int32 CcWindow::getWidth() const
{
  return m_pPrivate->m_oNormalRect.getWidth();
}

EWindowState CcWindow::getState()
{
  return m_pPrivate->m_eState;
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
  m_pPrivate->m_oGuiSubSystem = new CcGuiSubsystem(this);
  if (m_pPrivate->m_oGuiSubSystem != nullptr &&
      m_pPrivate->m_oGuiSubSystem->open())
  {
    if (!m_pPrivate->m_oGuiSubSystem->hasFrame())
    {
      CCNEW(m_pPrivate->m_oTitlebarWidget, CcTitlebar, nullptr);
    }

    CCNEW(m_pPrivate->m_oMainWidget, CcWindowsGuiMainWidget);
    m_pPrivate->m_oMainWidget->setSubSystemHandle(m_pPrivate->m_oGuiSubSystem->getHandle());
    m_pPrivate->m_oMainWidget->setBackgroundColor(CcColor(0, 255, 255));
    m_pPrivate->m_hMainWidget = m_pPrivate->m_oMainWidget.handle().cast<CcWidget>();

    m_pPrivate->m_oGuiSubSystem->setWindowTitle(m_pPrivate->m_sWindowTitle);
    m_pPrivate->m_oGuiSubSystem->getInputEventHandler() += NewCcEvent(CcWindow, CcInputEvent, CcWindow::eventInput, this);
    m_pPrivate->m_oGuiSubSystem->getControlEventHandler() += NewCcEvent(CcWindow, EGuiEvent, CcWindow::eventControl, this);
    return true;
  }
  return false;
}


void CcWindow::initWindowPrivate()
{
  CCDELETE(m_pPrivate);
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->m_oWindowStyle.oBackgroundColor = CcColor(0, 0, 0);
  m_pPrivate->m_oWindowStyle.oForegroundColor = CcColor(0, 0, 0);
  m_pPrivate->m_oWindowStyle.oBorderColor = CcColor(0xff, 0xff, 0xff);
  m_pPrivate->m_oWindowStyle.uBorderSize = 0;
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
  m_pPrivate->m_sWindowTitle = sTitle;
  if (m_pPrivate->m_oGuiSubSystem != nullptr)
  {
    m_pPrivate->m_oGuiSubSystem->setWindowTitle(sTitle);
  }
}

void CcWindow::setSize(const CcSize& oSize)
{
  m_pPrivate->m_oNormalRect.setSize(oSize);
  onRectangleChanged();
}

void CcWindow::setPos(const CcPoint& oPos)
{
  m_pPrivate->m_oNormalRect.setPoint(oPos);
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
      m_pPrivate->m_eState = m_pPrivate->m_eLastState;
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
    default:
      break;
  }
}

void CcWindow::parseMouseEvent(CcMouseEvent& oMouseEvent)
{
  CcPoint pPoint(oMouseEvent.x, oMouseEvent.y);
  CcWidgetHandle& pFound = m_pPrivate->m_hMainWidget->getHitTest(pPoint);
  m_pPrivate->m_oMouseEventHandler.call(pFound.ptr(), &oMouseEvent);
}

CcWindowHandle& CcWindow::getWindow()
{
  return m_pPrivate->m_hThis;
}
