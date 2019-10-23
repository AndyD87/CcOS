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

#include <QWindow>
#include <QGuiApplication>

class CcWindowMainWidget : public CcWidget
{
public:
  CcWindowMainWidget() : CcWidget(nullptr)
    { }

  virtual void onRectangleChanged()
  { }
};

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindow::CPrivate
{
public:
  CPrivate()
  {
    m_pMainWidget = new CcWindowMainWidget();
    m_pMainWidgetHandle = m_pMainWidget;
  }
  CcGuiSubsystem*         m_oGuiSubsystem;
  CcWindowMainWidget*     m_pMainWidget;
  CcWidgetHandle          m_pMainWidgetHandle;
  QWindow*                m_pWindow           = nullptr;
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
  int argc = 0;
  QGuiApplication app(argc, nullptr);
  m_pPrivate->m_pWindow = new QWindow();
  m_pPrivate->m_pWindow->show();
  app.exec();
}

CcRectangle CcWindow::getInnerRect()
{
  CcRectangle oRect;
  if (m_pPrivate->m_oGuiSubsystem->hasFrame())
  {
    oRect = m_pPrivate->m_oGuiSubsystem->getInnerRect();
    oRect.setPoint(CcPoint(0, 0));
  }
  else
  {
    oRect = m_oNormalRect;
  }
  return oRect;
}

bool CcWindow::setPixelArea(const CcRectangle& oRectangle)
{
  CcRectangle oArea = oRectangle;
  if (m_pPrivate->m_oGuiSubsystem != nullptr)
  {
    return  m_pPrivate->m_oGuiSubsystem->setPixelArea(oArea);
  }
  return false;
}

void CcWindow::setWindowState(EWindowState eState)
{
  m_pPrivate->m_oGuiSubsystem->setWindowState(eState);
  m_eState = eState;
}

CcWidgetHandle& CcWindow::getHandle()
{
  return m_pPrivate->m_pMainWidgetHandle;
}

void CcWindow::draw()
{
}

void CcWindow::drawPixel(const CcColor& oColor, uint64 uiNumber)
{
  if (m_pPrivate->m_oGuiSubsystem != nullptr)
    m_pPrivate->m_oGuiSubsystem->drawPixel(oColor, uiNumber);
}

bool CcWindow::initWindow()
{
  m_pPrivate->m_oGuiSubsystem = new CcGuiSubsystem(this);
  if (m_pPrivate->m_oGuiSubsystem != nullptr &&
      m_pPrivate->m_oGuiSubsystem->open())
  {
    return true;
  }
  return false;
}


void CcWindow::initWindowPrivate()
{
  CCDELETE(m_pPrivate);
  CCNEW(m_pPrivate, CPrivate);

}

void CcWindow::onRectangleChanged()
{
}

void CcWindow::setTitle(const CcString& sTitle)
{
}

void CcWindow::setSize(const CcSize& oSize)
{
  onRectangleChanged();
}

void CcWindow::setPos(const CcPoint& oPos)
{
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
    default:
      break;
  }
}

void CcWindow::parseMouseEvent(CcMouseEvent& oMouseEvent)
{
  //m_oMouseEventHandler.call(pFound.ptr(), &oMouseEvent);
}

CcWindowHandle& CcWindow::getWindow()
{
  return m_hThis;
}
