/**
 * @copyright  Andreas Dirmeier (C) 2022
 *
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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGuiSubsystem
 */
#include "CcGuiSubsystem.h"
#include "CcKernel.h"
#include "CcWindow.h"

INCLUDE_WARNING_LEVEL_START
#include <QEvent>
#include <QMouseEvent>
INCLUDE_WARNING_LEVEL_STOP

class CcGuiSubsystem::CPrivate
{
public:
  bool bDoRun     = true;
  bool bDoStopped = true;
};

CcGuiSubsystem::CcGuiSubsystem(CcWindow* hWindowHandle) : m_hWindow(hWindowHandle)
{
  if(m_pPrivate == nullptr)
    CCNEW(m_pPrivate, CPrivate);
}

CcGuiSubsystem::~CcGuiSubsystem()
{
  while (m_pPrivate->bDoStopped == false)
  {
    m_pPrivate->bDoRun = false;
    CcKernel::sleep(1);
  }
  CCDELETE(m_pPrivate);
}

void CcGuiSubsystem::loop()
{
  m_pPrivate->bDoStopped = false;
  while (m_pPrivate->bDoRun)
  {
  }
  m_pPrivate->bDoStopped = true;
}

void CcGuiSubsystem::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CCUNUSED(oPixel);
  CCUNUSED(uiNumber);
}

bool CcGuiSubsystem::setPixelArea(const CcRectangle& oArea)
{
  CCUNUSED(oArea);
  return false;
}

void CcGuiSubsystem::draw()
{
  // @todo flush data to display
}

void CcGuiSubsystem::getMaxArea(CcRectangle& oArea)
{
  CCUNUSED(oArea);
  // @todo flush data to display
}

void CcGuiSubsystem::hide()
{

}

void CcGuiSubsystem::show()
{

}

bool CcGuiSubsystem::hasFrame()
{
  return true;
}

CcRectangle CcGuiSubsystem::getInnerRect()
{
  // @todo Get Rectange from X11 Window
  CcRectangle oRect;
  return oRect;
}

void CcGuiSubsystem::setWindowTitle(const CcString& sTitle)
{
  // @todo Write Window Title to X11
  CCUNUSED(sTitle);

}

bool CcGuiSubsystem::setWindowState(EWindowState eState)
{
  bool bRet = false;
  // @todo Windowstate to X11
  CCUNUSED(eState);
  return bRet;
}

void CcGuiSubsystem::updateSize()
{

}

void CcGuiSubsystem::updatePos()
{

}

CcStatus CcGuiSubsystem::convertMouseEvent(void* pEvent, CcMouseEvent& rEvent)
{
  CcStatus oStatus(EStatus::NotSupported);
  QEvent* pQEvent = static_cast<QEvent*>(pEvent);
  switch(pQEvent->type())
  {
    case QEvent::Type::MouseButtonDblClick:
    {
      QMouseEvent* pQMouseEvent = static_cast<QMouseEvent*>(pEvent);
      switch (pQMouseEvent->button())
      {
        case Qt::MouseButton::LeftButton:
          rEvent.setLeft(true);
          rEvent.setType(EEventType::MouseLeftDoubleClick);
          oStatus = true;
          break;
        case Qt::MouseButton::RightButton:
          rEvent.setRight(true);
          rEvent.setType(EEventType::MouseRightDoubleClick);
          oStatus = true;
          break;
        case Qt::MouseButton::MiddleButton:
          rEvent.setMiddle(true);
          rEvent.setType(EEventType::MouseMiddleDoubleClick);
          oStatus = true;
          break;
      }
      break;
    }
    case QEvent::Type::MouseButtonRelease:
    {
      QMouseEvent* pQMouseEvent = static_cast<QMouseEvent*>(pEvent);
      switch (pQMouseEvent->button())
      {
        case Qt::MouseButton::LeftButton:
          rEvent.setLeft(false);
          oStatus = true;
          break;
        case Qt::MouseButton::RightButton:
          rEvent.setRight(false);
          oStatus = true;
          break;
        case Qt::MouseButton::MiddleButton:
          rEvent.setMiddle(false);
          oStatus = true;
          break;
      }
      break;
    }
    case QEvent::Type::MouseButtonPress:
    {
      QMouseEvent* pQMouseEvent = static_cast<QMouseEvent*>(pEvent);
      switch (pQMouseEvent->button())
      {
        case Qt::MouseButton::LeftButton:
          rEvent.setLeft(true);
          oStatus = true;
          break;
        case Qt::MouseButton::RightButton:
          rEvent.setRight(true);
          oStatus = true;
          break;
        case Qt::MouseButton::MiddleButton:
          rEvent.setMiddle(true);
          oStatus = true;
          break;
      }
      break;
    }
  }
  return oStatus;
}

CcStatus CcGuiSubsystem::convertKeyEvent(void* pEvent, CcKeyEvent& rEvent)
{
  CcStatus oStatus = EStatus::NotSupported;
  QEvent* pQEvent = static_cast<QEvent*>(pEvent);
  switch(pQEvent->type())
  {
    case QEvent::Type::KeyPress:
    {
      rEvent.setType(EEventType::KeyDown);
      rEvent.uiKey = static_cast<QKeyEvent*>(pEvent)->key();
      oStatus = true;
      break;
    }
    case QEvent::Type::KeyRelease:
    {
      rEvent.setType(EEventType::KeyUp);
      rEvent.uiKey = static_cast<QKeyEvent*>(pEvent)->key();
      oStatus = true;
      break;
    }
  }
  return oStatus;
}
