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
#ifndef H_CCWINDOW_H_
#define H_CCWINDOW_H_

#include "CcBase.h"
#include "CcBase.h"
#include "Devices/IDisplay.h"
#include "Events/CcMouseEventHandler.h"
#include "Events/CcKeyboardEventHandler.h"
#include "CcWidget.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"
#include "CcString.h"
#include "CcSharedPointer.h"
#include "CcHandle.h"
#include "CcEventActionLoop.h"

class CcTitlebar;
class CcWindow;
class CcEventAction;

/**
 * @brief The CcWindow class for creating a new Window.
 */
class CcGuiSHARED CcWindow : public CcObject, public CcEventActionLoop
{
public:
  CcWindow();
  CcWindow(uint16 sizeX, uint16 sizeY);
  virtual ~CcWindow();

  bool init();
  void show();
  void draw();
  void drawPixel(const CcColor& oColor, uint64 uiNumber = 1);
  virtual CcRectangle getInnerRect();
  bool setPixelArea(const CcRectangle& oRectangle);

  CcWidget* getWidget();
  CcEventHandler& getCloseHandler();
  CcMouseEventHandler& getMouseEventHandler();
  const CcString& getTitle();
  const CcSize& getSize() const;
  const CcPoint& getPos() const;
  int32 getHeight() const;
  int32 getWidth() const;
  EWindowState getState();

  void setWindowState(EWindowState eState);
  void setTitle(const CcString& sTitle);
  void setSize(const CcSize& oSize);
  void setPos(const CcPoint& oPos);

private:
  bool initWindow();
  void initWindowPrivate();
  void eventInput(CcInputEvent* oInputEvent);
  void eventControl(EEventType* eCommand);
  void parseMouseEvent(CcMouseEvent& oMouseEvent);
  void onRectangleChanged();
  virtual bool onLoop() override;

private: // Types
  class CPrivate;
private: // Member
  CPrivate*         m_pPrivate = nullptr;

public:
  static CcWindow* Null;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcWindow>;
#endif

typedef CcSharedPointer<CcWindow> CcWindowPointer;

#endif // H_CcWINDOW_H_
