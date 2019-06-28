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
#include "Devices/IDisplay.h"
#include "Events/CcMouseEventHandler.h"
#include "Events/CcKeyboardEventHandler.h"
#include "CcWidget.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"
#include "CcString.h"
#include "CcSharedPointer.h"
#include "CcHandle.h"

class CcTitlebar;
class IGuiSubsystem;
class CcWindow;

/**
 * @brief The CcWindow class for creating a new Window.
 */
class CcGuiSHARED CcWindow : public CcObject
{
public:
  CcWindow();
  CcWindow(uint16 sizeX, uint16 sizeY);
  virtual ~CcWindow();

  bool init();
  void loop();
  void draw();
  void drawPixel(const CcColor& oColor, uint64 uiNumber = 1);
  virtual CcRectangle getInnerRect();
  bool setPixelArea(const CcRectangle& oRectangle);
  
  const CcSize& getSize() const
    { return m_oNormalRect.getSize();}
  const CcPoint& getPos() const
    { return m_oNormalRect.getPoint();}
  int32 getHeight() const
    { return m_oNormalRect.getHeight();}
  int32 getWidth() const
    { return m_oNormalRect.getWidth();}
  inline EWindowState getState() { return m_eState; }
  void setWindowState(EWindowState eState);
  
  CcWidgetHandle& getHandle();
  
  CcEventHandler& getCloseHandler()
    { return m_oCloseHandler;}
  CcMouseEventHandler& getMouseEventHandler()
    { return m_oMouseEventHandler; }



  void setTitle(const CcString& sTitle);
  void setSize(const CcSize& oSize);
  void setPos(const CcPoint& oPos);

  const CcString& getTitle()
    {return m_sWindowTitle;}

private:
  bool initWindow();
  void initWindowPrivate();
  void eventInput(CcInputEvent* oInputEvent);
  void eventControl(EGuiEvent* eCommand);
  void parseMouseEvent(CcMouseEvent& oMouseEvent);

  CcWindowHandle& getWindow();
  void onRectangleChanged();

private: // Types
  class CPrivate;
private: // Member
  CPrivate*         m_pPrivate = nullptr;
  CcWindowHandle    m_hThis;
  CcString          m_sWindowTitle;
  CcRectangle       m_oNormalRect;
  EWindowState      m_eState = EWindowState::Normal;
  EWindowState      m_eLastState = EWindowState::Normal;
  CcEventHandler    m_oCloseHandler;
  CcMouseEventHandler m_oMouseEventHandler;
  CcStyleWidget     m_oWindowStyle;

public:
  static CcWindowHandle Null;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcWindow>;
#endif

typedef CcSharedPointer<CcWindow> CcWindowPointer;

#endif /* _CCWINDOW_H_ */
