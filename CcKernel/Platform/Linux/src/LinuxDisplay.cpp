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
 * @file      LinuxDisplay
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class LinuxDisplay
 */
#include "TargetConfig.h"

#if (CC_USE_GUI > 0)

#include "LinuxDisplay.h"
#include "CcKernel.h"
#include "stdio.h"
#include "unistd.h"

class LinuxDisplayThread : public CcThread {
public:
  LinuxDisplayThread( Display *Display){
    m_Display = Display;
  }

  void start(){
  }
  void run(){
    while (1) {
      XNextEvent(m_Display, &m_Event);
      if (m_Event.type == Expose) {
        CCINFO("Event: Expose\n");
        fflush(stdout);
      }
      else if (m_Event.type == KeyPress){
        CCINFO("Event: Keypress\n");
        fflush(stdout);
      }
    }
  }
  void stop(){}
private:
  Display *m_Display;
  XEvent   m_Event;
};

LinuxDisplay::LinuxDisplay(uint16 width, uint16 height) :
  CcDisplay(width, height)
{
  setAddress(0, 0, getSizeX(), getSizeY());
  setCursor(0,0);
  m_BackgroundLED= false;
}

LinuxDisplay::~LinuxDisplay() {
  XClosDisplay(m_Display);
}

bool LinuxDisplay::open( eOpenFlags flags ){
  CC_UNUSED(flags);
  m_Display = XOpenDisplay(NULL);
  if (m_Display == NULL) {
     return false;
  }

  m_Screen = DefaultScreen(m_Display);
  m_Window = XCreateSimpleWindow(m_Display,
                                 RootWindow(m_Display, m_Screen), 0, 0,
                                 getSizeX(), getSizeY(), 0,
                                 BlackPixel(m_Display, m_Screen),
                                 BlackPixel(m_Display, m_Screen));
  XSelectInput(m_Display, m_Window, ExposureMask | KeyPressMask);
  XMapWindow(m_Display, m_Window);

  m_EventThread = new LinuxDisplayThread(m_Display);
  Kernel.createThread(m_EventThread);
  return true;
}

bool LinuxDisplay::close(void){
  return true;
}

void LinuxDisplay::drawPixel(uchar R, uchar G, uchar B){
  uint32 uiTemp = (m_CursorY + m_DrawStartY) * getSizeX();
  uiTemp += m_CursorX + m_DrawStartX;
}

void LinuxDisplay::setAddress(uint16 x, uint16 y, uint16 xSize, uint16 ySize){
  if(x +xSize > getSizeX() ||
     y +ySize > getSizeY()){
    m_DrawSizeX = 0;
  }
  m_DrawStartX = x;
  m_DrawStartY = y;
  m_DrawSizeX = xSize;
  m_DrawSizeY = ySize;
  setCursor(0,0);
}


void LinuxDisplay::setBackgroundLed( bool bState )
{
  m_BackgroundLED = bState;
}

void LinuxDisplay::setCursor( uint16 x, uint16 y )
{
  m_CursorX = x;
  m_CursorY = y;
}

#endif // CC_USE_GUI
