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
  CcWindowMainWidget(CcWindow* pWindow, QWidget* pParent) :
    CcWidget(nullptr),
    pWindow(pWindow)
  {
    setSubSystemHandle(static_cast<void*>(pParent));
  }

  virtual CcWindow* getWindow() override;

  CcWindow* pWindow;
};

CcWindow* CcWindowMainWidget::getWindow()
{
  return pWindow;
}

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindow::CPrivate : public QMainWindow
{
public:
  CPrivate(CcWindow* pWindow) :
    QMainWindow(nullptr),
    pParent(pWindow),
    oQWidget(nullptr)
  {
    CCNEW(pMainWidget, CcWindowMainWidget, pWindow, this);
    setCentralWidget(&oQWidget);
  }

  virtual ~CPrivate() override;

  virtual void resizeEvent(QResizeEvent* event) override
  {
     QMainWindow::resizeEvent(event);
     // Your code here.
     oNormalRect.setSize(ToCcSize(event->size()));
     pParent->onRectangleChanged();
  }

  virtual void moveEvent(QMoveEvent* event) override
  {
     QMainWindow::moveEvent(event);
     // Your code here.
     oNormalRect.setPoint(ToCcPoint(event->pos()));
     pParent->onRectangleChanged();
  }

  virtual bool event(QEvent *event) override
  {
    bool bSuccess = false;
    EEventType eEvent = EEventType::NoEvent;
    switch(event->type())
    {
      case QEvent::ActivationChange:
        eEvent = EEventType::WindowRestore;
        pParent->eventControl(&eEvent);
        bSuccess = true;
        break;
      case QEvent::Close:
        eEvent = EEventType::WindowRestore;
        eState = EWindowState::Close;
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

  CcEventActionList   oEvents;
  CcString            sWindowTitle;
  CcRectangle         oNormalRect;
  EWindowState        eState = EWindowState::Normal;
  EWindowState        eLastState = EWindowState::Normal;
  CcEventHandler      oCloseHandler;
  CcMouseEventHandler oMouseEventHandler;
  CcStyleWidget       oWindowStyle;

  QApplication*       pApp = nullptr;
  QWidget             oQWidget;
};

CcWindow::CPrivate::~CPrivate()
{
  CCDELETE(pMainWidget);
}

CcWindow* CcWindow::Null(nullptr);

CcWindow::CcWindow()
{
  initWindowPrivate();
  m_pPrivate->oNormalRect.set(0, 0, 260, 320);
}

CcWindow::CcWindow(uint16 sizeX, uint16 sizeY)
{
  initWindowPrivate();
  m_pPrivate->oNormalRect.set(0, 0, sizeX, sizeY);
}

CcWindow::~CcWindow() 
{
  QApplication* pApp = m_pPrivate->pApp;
  CCDELETE(m_pPrivate);
  CCDELETE(pApp);
}

bool CcWindow::init()
{
  return initWindow();
}

void CcWindow::loop()
{
  m_pPrivate->setWindowTitle(ToQString(m_pPrivate->sWindowTitle));
  m_pPrivate->resize(ToQSize(m_pPrivate->oNormalRect.getSize()));
  m_pPrivate->move(ToQPoint(m_pPrivate->oNormalRect.getPoint()));
  m_pPrivate->show();
  while (m_pPrivate->eState != EWindowState::Close)
  {
    callEvents();
    QCoreApplication::processEvents();
  }
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
    oRect = m_pPrivate->oNormalRect;
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
  m_pPrivate->eState = eState;
}

CcWidget* CcWindow::getWidget()
{
  return m_pPrivate->pMainWidget;
}

CcEventHandler& CcWindow::getCloseHandler()
{
  return m_pPrivate->oCloseHandler;
}

CcMouseEventHandler& CcWindow::getMouseEventHandler()
{
  return m_pPrivate->oMouseEventHandler;
}

const CcString& CcWindow::getTitle()
{
  return m_pPrivate->sWindowTitle;
}

const CcSize& CcWindow::getSize() const
{
  return m_pPrivate->oNormalRect.getSize();
}

const CcPoint& CcWindow::getPos() const
{
  return m_pPrivate->oNormalRect.getPoint();
}

int32 CcWindow::getHeight() const
{
  return m_pPrivate->oNormalRect.getHeight();
}

int32 CcWindow::getWidth() const
{
  return m_pPrivate->oNormalRect.getWidth();
}

EWindowState CcWindow::getState()
{
  return m_pPrivate->eState;
}

void CcWindow::draw()
{
  m_pPrivate->update();
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
}

void CcWindow::onRectangleChanged()
{
}

void CcWindow::setTitle(const CcString& sTitle)
{
  m_pPrivate->sWindowTitle = sTitle;
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

void CcWindow::appendAction(const CcEventAction& oAction)
{
  m_pPrivate->oEvents.append(oAction);
}

void CcWindow::eventControl(EEventType* eCommand)
{
  switch (*eCommand)
  {
    case EEventType::WindowClose:
      setWindowState(EWindowState::Close);
      break;
    case EEventType::WindowRestore:
      if(m_pPrivate->eState != EWindowState::Close)
        m_pPrivate->eState = m_pPrivate->eLastState;
      break;
    case EEventType::WindowMaximimized:
      setWindowState(EWindowState::Maximimized);
      break;
    case EEventType::WindowMinimized:
      setWindowState(EWindowState::Minimized);
      break;
    case EEventType::WindowNormal:
      setWindowState(EWindowState::Normal);
      break;
    case EEventType::WindowTray:
      setWindowState(EWindowState::Tray);
      break;
    case EEventType::WindowSize:
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
    case EEventType::MouseEvent:
    {
      parseMouseEvent(*static_cast<CcMouseEvent*>(pInputEvent));
      break;
    }
    case EEventType::KeyEvent:
    default:
      break;
  }
}

void CcWindow::parseMouseEvent(CcMouseEvent& oMouseEvent)
{
  CCUNUSED(oMouseEvent);
  //m_pPrivate->oMouseEventHandler.call(pFound.ptr(), &oMouseEvent);
}

void CcWindow::callEvents()
{
  while (m_pPrivate->oEvents.size() > 0)
  {
    m_pPrivate->oEvents[0].call();
    m_pPrivate->oEvents.remove(0);
  }
}
