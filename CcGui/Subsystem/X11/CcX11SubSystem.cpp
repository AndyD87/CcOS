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
 * @brief     Implementation of Class CcX11SubSystem
 */
#include "CcX11SubSystem.h"
#include "CcWindow.h"
#include "CcKernel.h"
#include "stdio.h"
#include "unistd.h"
#include "X11/X.h"

CcGuiSubSystem* CcGuiSubSystem::create(const CcWindowHandle& hWindow)
{
  CcGuiSubSystem* pGuiSubSys = new CcX11SubSystem(hWindow);
  CCMONITORNEW(pGuiSubSys);
  return pGuiSubSys;
}

class CcX11SubSystemPrivate
{
public:
  bool      m_BackgroundLED;
  Window    m_Window;
  int       m_Screen;
  Display *m_Display;
  XEvent   m_Event;

  int16 m_DrawXStart;
  int16 m_DrawYStart;
  uint16 m_DrawXSize;
  uint16 m_DrawYSize;
  uint16 m_CursorX;
  uint16 m_CursorY;
};

CcX11SubSystem::CcX11SubSystem(const CcWindowHandle &hWindowHandle):
  CcGuiSubSystem(hWindowHandle)
{
  m_pPrivate = new CcX11SubSystemPrivate();
}

CcX11SubSystem::~CcX11SubSystem()
{
  XCloseDisplay(m_pPrivate->m_Display);
}

CcStatus CcX11SubSystem::open( )
{
  CcStatus oStatus;
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
  return oStatus;
}

CcStatus CcX11SubSystem::close()
{
  return true;
}

void CcX11SubSystem::loop()
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

void CcX11SubSystem::drawPixel(const CcColor& oPixel, uint64 uiNumber)
{
  CCUNUSED(oPixel);
  CCUNUSED(uiNumber);
  uint32 uiTemp = (m_pPrivate->m_CursorY + m_pPrivate->m_DrawYStart) * getWindowHandle()->getWidth();
  uiTemp += m_pPrivate->m_CursorX + m_pPrivate->m_DrawXStart;
}

bool CcX11SubSystem::setPixelArea(const CcRectangle& oArea)
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

void CcX11SubSystem::draw()
{
  // @todo flush data to display
}

void CcX11SubSystem::getMaxArea(CcRectangle& oArea)
{
  CCUNUSED(oArea);
  // @todo flush data to display
}

void CcX11SubSystem::hide()
{

}

void CcX11SubSystem::show()
{

}

bool CcX11SubSystem::hasFrame()
{
  return true;
}

CcRectangle CcX11SubSystem::getInnerRect()
{
  // @todo Get Rectange from X11 Window
  CcRectangle oRect;
  return oRect;
}

void CcX11SubSystem::setWindowTitle(const CcString& sTitle)
{
  // @todo Write Window Title to X11
  CCUNUSED(sTitle);

}

bool CcX11SubSystem::setWindowState(EWindowState eState)
{
  bool bRet = false;
  // @todo Windowstate to X11
  CCUNUSED(eState);
  return bRet;
}

CcSubSysHandle CcX11SubSystem::getHandle()
{
  // @todo return a common X11 widget
  return CcSubSysHandle();
}

void CcX11SubSystem::updateSize()
{

}

void CcX11SubSystem::updatePos()
{

}
