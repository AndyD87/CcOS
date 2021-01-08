/*
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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGuiSubsystem
 */
#include "CcGuiSubsystem.h"
#include "CcWindow.h"
#include "CcKernel.h"
#include "stdio.h"
#include "unistd.h"
#include <X11/X.h>
#include <X11/Xlib.h>

class CcGuiSubsystem::CPrivate
{
public:
  bool      m_BackgroundLED;
  Window    m_Window;
  int       m_Screen;
  Display*  m_Display;
  XEvent    m_Event;

  int16     m_DrawXStart;
  int16     m_DrawYStart;
  uint16    m_DrawXSize;
  uint16    m_DrawYSize;
  uint16    m_CursorX;
  uint16    m_CursorY;
};

CcGuiSubsystem::CcGuiSubsystem(CcWindow* hWindowHandle) : m_hWindow(hWindowHandle)
{
  if(m_pPrivate == nullptr)
    CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->m_Display = XOpenDisplay(NULL);
  if (m_pPrivate->m_Display == NULL)
  {
     oStatus = false;
  }
  else
  {
    m_pPrivate->m_Screen = DefaultScreen(m_pPrivate->m_Display);
    m_pPrivate->m_Window = XCreateSimpleWindow(m_pPrivate->m_Display,
                                   RootWindow(m_pPrivate->m_Display, m_pPrivate->m_Screen), 0, 0,
                                   getWindowHandle()->getWidth(), getWindowHandle()->getHeight(), 0,
                                   BlackPixel(m_pPrivate->m_Display, m_pPrivate->m_Screen),
                                   BlackPixel(m_pPrivate->m_Display, m_pPrivate->m_Screen));
    XSelectInput(m_pPrivate->m_Display, m_pPrivate->m_Window, ExposureMask | KeyPressMask);
    XMapWindow(m_pPrivate->m_Display, m_pPrivate->m_Window);
  }
}

CcGuiSubsystem::~CcGuiSubsystem()
{
  XCloseDisplay(m_pPrivate->m_Display);
  CCDELETE(m_pPrivate);
  return true;
}

void CcGuiSubsystem::loop()
{
  while (1)
  {
    XNextEvent(m_pPrivate->m_Display, &m_pPrivate->m_Event);
    if (m_pPrivate->m_Event.type == Expose)
    {
      CCINFO("Event: Expose\n");
      fflush(stdout);
    }
    else if (m_pPrivate->m_Event.type == KeyPress)
    {
      CCINFO("Event: Keypress\n");
      fflush(stdout);
    }
  }
}

void CcGuiSubsystem::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CCUNUSED(oPixel);
  CCUNUSED(uiNumber);
  uint32 uiTemp = (m_pPrivate->m_CursorY + m_pPrivate->m_DrawYStart) * getWindowHandle()->getWidth();
  uiTemp += m_pPrivate->m_CursorX + m_pPrivate->m_DrawXStart;
}

bool CcGuiSubsystem::setPixelArea(const CcRectangle& oArea)
{
  m_pPrivate->m_DrawXStart = oArea.getX();
  m_pPrivate->m_DrawYStart = oArea.getY();
  m_pPrivate->m_DrawXSize = oArea.getWidth();
  m_pPrivate->m_DrawYSize = oArea.getHeight();
  m_pPrivate->m_CursorX = 0;
  m_pPrivate->m_CursorY = 0;
  if (( getWindowHandle()->getWidth()  > oArea.getX() &&
        getWindowHandle()->getHeight() > oArea.getY()) ||
      ( getWindowHandle()->getWidth()  > oArea.getBottomRightCorner().getX() &&
        getWindowHandle()->getHeight() > oArea.getBottomRightCorner().getY()))
    return true;
  else
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

void* CcGuiSubsystem::getHandle()
{
  // @todo return a common X11 widget
  return this;
}

void CcGuiSubsystem::updateSize()
{

}

void CcGuiSubsystem::updatePos()
{

}
