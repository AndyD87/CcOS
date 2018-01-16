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
 * @page      CcGui
 * @subpage   CcWindow
 *
 * @page      CcWindow
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindow
 */
#ifndef _CCWINDOW_H_
#define _CCWINDOW_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Devices/CcDisplay.h"
#include "Devices/CcDisplayArea.h"
#include "Events/CcMouseEventHandler.h"
#include "Events/CcKeyboardEventHandler.h"
#include "CcWidget.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"
#include "CcString.h"
#include "CcSharedPointer.h"
#include "CcHandle.h"

class CcTitlebar;
class CcWindowPrivate;

/**
 * @brief Current visual state of Window as enum
 */
enum class EWindowState
{
  Normal,
  Minimized,
  Maximimized,
  Tray,
};

/**
 * @brief The CcWindow class for creating a new Window.
 */
class CcGuiSHARED CcWindow : public CcWidget
{
public:
  CcWindow(void);
  CcWindow(const CcHandle<CcDisplay>& oDisplay);
  CcWindow(uint16 sizeX, uint16 sizeY, const CcHandle<CcDisplay>& display);
  virtual ~CcWindow();

  bool init(void);
  void loop(void);
  void close(void);
  void draw(void) override;
  void drawPixel(const CcColor& oColor);
  bool setPixelArea(const CcRectangle& oRectangle);

  inline EWindowState getState() { return m_eState; }
  void setState(EWindowState eState);
  
  CcEventHandler& getCloseHandler()
    { return m_oCloseHandler;}
  CcMouseEventHandler& getMouseEventHandler()
    { return m_oMouseEventHandler; }

  void setSize(const CcSize& oSize);
  void setPos(const CcPoint& oPoint);
  void setWindowRect(const CcRectangle& oRect);
  void setTitle(const CcString& sTitle);
  const CcString& getTitle()
    {return m_sWindowTitle;}

private:
  bool initWindow();
  void initWindowPrivate();
  void eventInput(CcInputEvent* oInputEvent);
  void eventControl(EDisplayCommands* eCommand);
  void parseMouseEvent(CcMouseEvent& oMouseEvent);
  inline CcWindowHandle getWindow() override
    { return this; }

  CcWidgetHandle getHitTest(const CcPoint& oPointToFind);
  void onRectangleChanged()override;

private:
  CcWindowPrivate*  m_pPrivate = nullptr;
  CcString          m_sWindowTitle;
  CcRectangle       m_oNormalRect;
  EWindowState      m_eState = EWindowState::Normal;
  EWindowState      m_eLastState = EWindowState::Normal;
  CcEventHandler    m_oCloseHandler;
  CcMouseEventHandler m_oMouseEventHandler;
};

#if WIN32
template class CcGuiSHARED CcSharedPointer<CcWindow>;
#endif

typedef CcSharedPointer<CcWindow> CcWindowPointer;

#endif /* _CCWINDOW_H_ */
