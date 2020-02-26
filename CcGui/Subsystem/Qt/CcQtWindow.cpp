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

#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QResizeEvent>
#include <QMoveEvent>

#include "CcWindow.h"
#include "CcQt.h"
#include "CcKernel.h"
#include "CcPainter.h"
#include "CcInputEvent.h"
#include "Widgets/CcTitlebar.h"
#include "CcGuiSubsystem.h"
#include "CcGlobalStrings.h"

class CcWindowMainWidget : public CcWidget, public QWidget
{
public:
  CcWindowMainWidget(CcWindow* pWindow, QWidget* pParent) :
    CcWidget(nullptr),
    QWidget(pParent),
    pWindow(pWindow)
  {
    setSubSystemHandle(static_cast<void*>(static_cast<QWidget*>(this)));
  }

  virtual CcWindow* getWindow() override
  {
    return pWindow;
  }

  CcWidget* getCcWidget()
  {
    return static_cast<CcWidget*>(this);
  }

  virtual void enterEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    CcMouseEvent oMouseEvent;
    oMouseEvent.setType(EEventType::MouseHover);
    getCcWidget()->event(&oMouseEvent);
  }

  virtual void leaveEvent(QEvent* pEvent) override
  {
    CCUNUSED(pEvent);
    CcMouseEvent oMouseEvent;
    oMouseEvent.setType(EEventType::MouseLeave);
    getCcWidget()->event(&oMouseEvent);
  }

  virtual bool event(QEvent* pEvent) override
  {
    bool bHandled = false;
    CCNEWARRAYTYPE(pData, char, CCMAX(sizeof(CcMouseEvent), sizeof(CcKeyEvent)));
    CcStatic::memset(pData, 0, CCMAX(sizeof(CcMouseEvent), sizeof(CcKeyEvent)));
    if(CcGuiSubsystem::convertMouseEvent(pEvent, *CCVOIDPTRCAST(CcMouseEvent*, pData)))
    {
      this->event(CCVOIDPTRCAST(CcInputEvent*, pData));
    }
    else if(CcGuiSubsystem::convertKeyEvent(pEvent, *CCVOIDPTRCAST(CcKeyEvent*, pData)))
    {
      this->event(CCVOIDPTRCAST(CcInputEvent*, pData));
    }
    else
    {
      switch(pEvent->type())
      {
        case QEvent::Type::Resize:
          this->setSize(ToCcSize(size()));
          break;
        default:
          bHandled = false;
      }

      if (!bHandled)
      {
        bHandled = QWidget::event(pEvent);
      }
    }
    CCDELETEARR(pData);
    return bHandled;
  }

  virtual void event(CcInputEvent* pEventData) override
  { CcWidget::event(pEventData); }

  CcWindow* pWindow;
};

/**
 * @brief Storage class for private members of CcWindow
 */
class CcWindow::CPrivate : public QMainWindow
{
public:
  CPrivate(CcWindow* pWindow) :
    QMainWindow(nullptr),
    pParent(pWindow)
  {
    CCNEW(pMainWidget, CcWindowMainWidget, pWindow, nullptr);
    setCentralWidget(pMainWidget);
  }

  virtual ~CPrivate() override
  {
    CCDELETE(pMainWidget);
  };

  virtual void resizeEvent(QResizeEvent* event) override
  {
     QMainWindow::resizeEvent(event);
     // Your code here.
     resizeReevent();
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
      case QEvent::MouseButtonPress:
        bSuccess = false;
        break;
      default:
        break;
    }
    bSuccess = QMainWindow::event(event);
    return bSuccess;
  }

  void resizeReevent()
  {
     oNormalRect.setSize(ToCcSize(size()));
     pParent->onRectangleChanged();
  }

  CcWindow*               pParent;
  CcGuiSubsystem*         pGuiSubsystem = nullptr;
  CcWindowMainWidget*     pMainWidget   = nullptr;

  CcString            sWindowTitle;
  CcRectangle         oNormalRect;
  EWindowState        eState = EWindowState::Normal;
  EWindowState        eLastState = EWindowState::Normal;
  CcEventHandler      oCloseHandler;
  CcMouseEventHandler oMouseEventHandler;
  CcStyleWidget       oWindowStyle;

  QApplication*       pApp = nullptr;
};

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
  if (m_pPrivate) CCDELETE(m_pPrivate->pGuiSubsystem);
  CCDELETE(m_pPrivate);
  CCDELETE(pApp);
}

bool CcWindow::init()
{
  return initWindow();
}

void CcWindow::show()
{
  m_pPrivate->show();
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
  bool bSuccess = true;
  switch(eState)
  {
    case EWindowState::Maximimized:
      m_pPrivate->setWindowState(Qt::WindowState::WindowMaximized);
      break;
    case EWindowState::Minimized:
      m_pPrivate->setWindowState(Qt::WindowState::WindowMinimized);
      break;
    case EWindowState::Normal:
      m_pPrivate->setWindowState(Qt::WindowState::WindowNoState);
      break;
    case EWindowState::FullScreen:
      m_pPrivate->setWindowState(Qt::WindowState::WindowFullScreen);
      break;
    default:
      bSuccess = false;
  }
  if(bSuccess)
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
  if (m_pPrivate->pGuiSubsystem != nullptr)
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
  CCNEWTYPE(pApp, QApplication, CcWindow__argc, &CcWindow__ppArgv);
  CCNEW(m_pPrivate, CPrivate, this);
  m_pPrivate->pApp = pApp;
}

void CcWindow::onRectangleChanged()
{
  m_pPrivate->pMainWidget->resize(ToQSize(getSize()));
}

bool CcWindow::onLoop()
{
  QCoreApplication::processEvents();
  return m_pPrivate->eState != EWindowState::Close;
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
