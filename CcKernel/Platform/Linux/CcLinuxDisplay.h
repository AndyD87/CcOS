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
 * @page      Linux
 * @subpage   CcLinuxDisplay
 *
 * @page      CcLinuxDisplay
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxDisplay
 */
#if (CCOS_GUI > 0)

#ifndef _CcLinuxDisplay_H_
#define _CcLinuxDisplay_H_

#include "Devices/CcTimer.h"
#include "Devices/CcDisplay.h"
#include "CcColor.h"
#include <X11/Xlib.h>
#include "CcThread.h"

typedef struct{
  uint8 B;
  uint8 G;
  uint8 R;
} bitmapRGB;

typedef  struct{
  uint16 width;
  uint16 height;
  uint32 pixCount;
  bitmapRGB *bitmap;
} bitmapAll;

extern bitmapAll g_Bitmap;

class CcLinuxDisplay : public CcDisplay
{
public:
  CcLinuxDisplay(uint16 width, uint16 height);
  virtual ~CcLinuxDisplay();

  bool open( eOpenFlags flags = eNoDevice);
  bool close(void);

  void setCursor( uint16 x, uint16 y );
  void setAddress( uint16 x, uint16 y, uint16 xSize, uint16 ySize);
  void setBacklight( uint8 uiBrightness );
  void drawPixel(uchar R, uchar G, uchar B);
private: //member
  bool    m_BackgroundLED;
  Display *m_Display;
  Window  m_Window;
  int     m_Screen;

  CcThread *m_EventThread;
};

#endif /* _CcLinuxDisplay_H_ */

#endif // use gui
