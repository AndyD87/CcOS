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

#include "CcQt.h"
#include "CcWindow.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"
#include "Widgets/CcTitlebar.h"
#include "CcGuiSubsystem.h"
#include "CcGlobalStrings.h"

#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QResizeEvent>
#include <QMoveEvent>

class CcWindowMainWidget : public CcWidget
{
public:
  CcWindowMainWidget() : CcWidget(nullptr)
    {}
  virtual void onRectangleChanged();
};

void CcWindowMainWidget::onRectangleChanged()
{}

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindow::CPrivate : public QMainWindow
{
public:
  CPrivate(CcWindow* pParent) :
    QMainWindow(nullptr),
    pParent(pParent)
  {
    pMainWidget = new CcWindowMainWidget();
    pMainWidgetHandle = pMainWidget;
  }

  virtual ~CPrivate() override;

  void init()
  {

  }

  virtual void resizeEvent(QResizeEvent* event) override
  {
     QMainWindow::resizeEvent(event);
     // Your code here.
     m_oNormalRect.setSize(ToCcSize(event->size()));
     pParent->onRectangleChanged();
  }

  virtual void moveEvent(QMoveEvent* event) override
  {
     QMainWindow::moveEvent(event);
     // Your code here.
     m_oNormalRect.setPoint(ToCcPoint(event->pos()));
     pParent->onRectangleChanged();
  }

  virtual bool event(QEvent *event) override
  {
    bool bSuccess = false;
    EGuiEvent eEvent = EGuiEvent::NoEvent;
    switch(event->type())
    {
      case QEvent::ActivationChange:
        eEvent = EGuiEvent::WindowRestore;
        pParent->eventControl(&eEvent);
        bSuccess = true;
        break;
      default:
        break;
    }
    bSuccess = QMainWindow::event(event);
    return bSuccess;
  }

  CcWindow*               pParent;
  CcGuiSubsystem*         pGuiSubsystem = nullptr;
  CcWindowMainWidget*     pMainWidget   = nullptr;
  QApplication*           pApp          = nullptr;
  CcWidgetHandle          pMainWidgetHandle;

  CcWindowHandle      m_hThis;
  CcString            m_sWindowTitle;
  CcRectangle         m_oNormalRect;
  EWindowState        m_eState = EWindowState::Normal;
  EWindowState        m_eLastState = EWindowState::Normal;
  CcEventHandler      m_oCloseHandler;
  CcMouseEventHandler m_oMouseEventHandler;
  CcStyleWidget       m_oWindowStyle;
};

CcWindow::CPrivate::~CPrivate()
{
  CCDELETE(pMainWidget);
  CCDELETE(pApp);
}

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
  m_pPrivate->setWindowTitle(ToQString(m_pPrivate->m_sWindowTitle));
  m_pPrivate->resize(ToQSize(m_pPrivate->m_oNormalRect.getSize()));
  m_pPrivate->move(ToQPoint(m_pPrivate->m_oNormalRect.getPoint()));
  m_pPrivate->show();
  m_pPrivate->pApp->exec();
}

CcRectangle CcWindow::getInnerRect()
{
  CcRectangle oRect;
  if (m_pPrivate->pGuiSubsystem->hasFrame())
  {
    oRect = m_pPrivate->pGuiSubsystem->getInnerRect();
    oRect.setPoint(CcPoint(0, 0));
  }
  else
  {
    oRect = m_pPrivate->m_oNormalRect;
  }
  return oRect;
}

bool CcWindow::setPixelArea(const CcRectangle& oRectangle)
{
  CcRectangle oArea = oRectangle;
  if (m_pPrivate->pGuiSubsystem != nullptr)
  {
    return  m_pPrivate->pGuiSubsystem->setPixelArea(oArea);
  }
  return false;
}

void CcWindow::setWindowState(EWindowState eState)
{
  m_pPrivate->pGuiSubsystem->setWindowState(eState);
  m_pPrivate->m_eState = eState;
}

CcWidgetHandle& CcWindow::getHandle()
{
  return m_pPrivate->pMainWidgetHandle;
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
}

void CcWindow::drawPixel(const CcColor& oColor, uint64 uiNumber)
{
  if (m_pPrivate->pGuiSubsystem != nullptr)
    m_pPrivate->pGuiSubsystem->drawPixel(oColor, uiNumber);
}

bool CcWindow::initWindow()
{
  m_pPrivate->pGuiSubsystem = new CcGuiSubsystem(this);
  if (m_pPrivate->pGuiSubsystem != nullptr &&
      m_pPrivate->pGuiSubsystem->open())
  {
    return true;
  }
  return false;
}

void CcWindow::initWindowPrivate()
{
  static int CcWindow__argc = 1;
  static CcString CcWindow__argv = "/";
  static char* CcWindow__ppArgv = CcWindow__argv.getCharString();
  QApplication *pApp = new QApplication(CcWindow__argc, &CcWindow__ppArgv);
  CCNEW(m_pPrivate, CPrivate, this);
  m_pPrivate->pApp = pApp;
  m_pPrivate->init();
}

void CcWindow::onRectangleChanged()
{
}

void CcWindow::setTitle(const CcString& sTitle)
{
  m_pPrivate->m_sWindowTitle = sTitle;
  m_pPrivate->setWindowTitle(ToQString(sTitle));
}

void CcWindow::setSize(const CcSize& oSize)
{
  m_pPrivate->resize(ToQSize(oSize));
}

void CcWindow::setPos(const CcPoint& oPos)
{
  m_pPrivate->move(ToQPoint(oPos));
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
  //m_pPrivate->m_oMouseEventHandler.call(pFound.ptr(), &oMouseEvent);
}

CcWindowHandle& CcWindow::getWindow()
{
  return m_pPrivate->m_hThis;
}
