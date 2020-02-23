/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGuiSubsystem
 */
#include "CcGuiSubsystem.h"
#include "CcKernel.h"
#include "CcWindow.h"
#include <QEvent>
#include <QMouseEvent>

class CcGuiSubsystem::CPrivate
{
public:
};

CcGuiSubsystem::CcGuiSubsystem(CcWindow* hWindowHandle) : m_hWindow(hWindowHandle)
{
  if(m_pPrivate == nullptr)
    CCNEW(m_pPrivate, CPrivate);
}

CcGuiSubsystem::~CcGuiSubsystem()
{
  CCDELETE(m_pPrivate);
}

void CcGuiSubsystem::loop()
{
  bool bDoRun = true;
  while (bDoRun)
  {
    bDoRun = false;
  }
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
      oStatus = true;
      QMouseEvent* pQMouseEvent = static_cast<QMouseEvent*>(pEvent);
      switch (pQMouseEvent->button())
      {
        case Qt::MouseButton::LeftButton:
          rEvent.setLeft(true);
          rEvent.setType(EEventType::MouseLeftDoubleClick);
          break;
        case Qt::MouseButton::RightButton:
          rEvent.setRight(true);
          rEvent.setType(EEventType::MouseRightDoubleClick);
          break;
        case Qt::MouseButton::MiddleButton:
          rEvent.setMiddle(true);
          rEvent.setType(EEventType::MouseMiddleDoubleClick);
          break;
        default:
          oStatus = EStatus::NotSupported;
      }
      break;
    }
    case QEvent::Type::MouseButtonRelease:
    {
      oStatus = true;
      QMouseEvent* pQMouseEvent = static_cast<QMouseEvent*>(pEvent);
      switch (pQMouseEvent->button())
      {
        case Qt::MouseButton::LeftButton:
          rEvent.setLeft(false);
          break;
        case Qt::MouseButton::RightButton:
          rEvent.setRight(false);
          break;
        case Qt::MouseButton::MiddleButton:
          rEvent.setMiddle(false);
          break;
        default:
          oStatus = EStatus::NotSupported;
      }
      break;
    }
    case QEvent::Type::MouseButtonPress:
    {
      oStatus = true;
      QMouseEvent* pQMouseEvent = static_cast<QMouseEvent*>(pEvent);
      switch (pQMouseEvent->button())
      {
        case Qt::MouseButton::LeftButton:
          rEvent.setLeft(true);
          break;
        case Qt::MouseButton::RightButton:
          rEvent.setRight(true);
          break;
        case Qt::MouseButton::MiddleButton:
          rEvent.setMiddle(true);
          break;
        default:
          oStatus = EStatus::NotSupported;
      }
      break;
    }
    default:
      oStatus = EStatus::NotSupported;
  }
  return oStatus;
}

CcStatus CcGuiSubsystem::convertKeyEvent(void* pEvent, CcKeyEvent& rEvent)
{
  CcStatus oStatus;
  QEvent* pQEvent = static_cast<QEvent*>(pEvent);
  switch(pQEvent->type())
  {
    case QEvent::Type::KeyPress:
    {
      rEvent.setType(EEventType::KeyDown);
      rEvent.uiKey = static_cast<QKeyEvent*>(pEvent)->key();
      break;
    }
    case QEvent::Type::KeyRelease:
    {
      rEvent.setType(EEventType::KeyUp);
      rEvent.uiKey = static_cast<QKeyEvent*>(pEvent)->key();
      break;
    }
    default:
      oStatus = EStatus::NotSupported;
  }
  return oStatus;
}
