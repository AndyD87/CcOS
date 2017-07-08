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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindow
 */
#ifndef CCWINDOW_H_
#define CCWINDOW_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Devices/CcDisplay.h"
#include "Devices/CcDisplayArea.h"
#include "CcWidget.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"
#include "CcString.h"

class CcTitlebar;

enum class EWindowState
{
  Normal,
  Minimized,
  Maximimized,
  Tray,
};

class CcGuiSHARED CcWindow : public CcWidget
{
public:
  CcWindow(void);
  CcWindow(const CcHandle<CcDisplay>& oDisplay);
  CcWindow(uint16 sizeX, uint16 sizeY);
  CcWindow(uint16 sizeX, uint16 sizeY, const CcHandle<CcDisplay>& display);
  virtual ~CcWindow();

  bool init(void);
  void close(void);
  void draw(void) override;
  void drawPixel(const CcColor& oColor);
  bool setPixelArea(const CcRectangle& oRectangle);

  inline EWindowState getState() { return m_eState; }
  void setState(EWindowState eState);


  CcWidget* getLastLeftButtonDown()
    { return m_pLastLeftButtonDown;}
  CcWidget* getLastRightButtonDown()
    { return m_pLastRightButtonDown;}
  CcWidget* getLastMiddleButtonDown()
    { return m_pLastMiddleButtonDown;}
  CcWidget* getLastHovered()
    { return m_pLastHovered;}
  
  CcEventHandler& getCloseHandler()
    { return m_oCloseHandler;}
  CcEventHandler& getMouseHoverHandler()
    {return m_oMouseHoverEvents;}
  CcEventHandler& getMouseClickHanlder()
    {return m_oMouseClickEvents;}
  CcEventHandler& getMouseLeaveHanlder()
    {return m_oMouseLeaveEvents;}
  CcEventHandler& getMouseDoubleClickHanlder()
    {return m_oMouseDoubleClickEvents;}
  CcEventHandler& getMouseLeftDownHandler()
    {return m_oMouseLeftDownEvents; }
  CcEventHandler& getMouseLeftUpHandler()
    {return m_oMouseLeftUpEvents; }
  CcEventHandler& getMouseRightDownHandler()
    {return m_oMouseRightDownEvents; }
  CcEventHandler& getMouseRightUpHandler()
    {return m_oMouseRightUpEvents; }
  CcEventHandler& getMouseMiddleDownHandler()
    {return m_oMouseMiddleDownEvents; }
  CcEventHandler& getMouseMiddleUpHandler()
    {return m_oMouseMiddleUpEvents; }
  CcEventHandler& setMouseMoveHandler()
    {return m_oMouseMoveEvents; }

  void setSize(const CcSize& oSize);
  void setPos(const CcPoint& oPoint);
  void setWindowRect(const CcRectangle& oRect);
  void setTitle(const CcString& sTitle);
  const CcString& getTitle()
    {return m_sWindowTitle;}

private:
  bool initWindow();
  void eventInput(CcInputEvent* oInputEvent);
  void eventControl(EDisplayCommands* eCommand);
  void parseMouseEvent(CcMouseEvent& oMouseEvent);
  inline CcWindow* getWindow() override 
    { return this; }

  CcWidget* getHitTest(const CcPoint& oPointToFind);
  void onRectangleChanged()override;

private:
  CcString        m_sWindowTitle;
  CcRectangle     m_oNormalRect;
  CcDisplayArea*  m_oDisplayArea = NULL;
  EWindowState    m_eState = EWindowState::Normal;
  EWindowState    m_eLastState = EWindowState::Normal;
  CcEventHandler  m_oCloseHandler;
  CcHandle<CcDisplay> m_oDisplay;
  CcTitlebar*       m_oTitlebarWidget         = NULL;
  CcWidget*         m_pLastLeftButtonDown     = NULL;
  CcWidget*         m_pLastRightButtonDown    = NULL;
  CcWidget*         m_pLastMiddleButtonDown   = NULL;
  CcWidget*         m_pLastHovered            = NULL;
  CcEventHandler    m_oMouseLeftDownEvents;
  CcEventHandler    m_oMouseLeftUpEvents;
  CcEventHandler    m_oMouseRightDownEvents;
  CcEventHandler    m_oMouseRightUpEvents;
  CcEventHandler    m_oMouseMiddleDownEvents;
  CcEventHandler    m_oMouseMiddleUpEvents;
  CcEventHandler    m_oMouseHoverEvents;
  CcEventHandler    m_oMouseClickEvents;
  CcEventHandler    m_oMouseLeaveEvents;
  CcEventHandler    m_oMouseMoveEvents;
  CcEventHandler    m_oMouseDoubleClickEvents;
};

#endif /* CCWINDOW_H_ */
