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
 * @file
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindow
 */
#pragma once

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
  /**
   * @brief Initialize window with specific size
   * @param sizeX: Width
   * @param sizeY: Height
   */
  CcWindow(uint16 sizeX, uint16 sizeY);
  virtual ~CcWindow();

  /**
   * @brief Initialize windows
   * @return True if init succeeded
   */
  bool init();

  /**
   * @brief Hide window
   */
  void hide();

  /**
   * @brief Show/Unhide window
   */
  void show();

  /**
   * @brief Draw action for window and all child widgets
   */
  void draw();

  /**
   * @brief Draw one or more pixel with specific color to WindowWidget
   * @param oColor:   Target color of pixel
   * @param uiNumber: Number of pixels to draw
   */
  void drawPixel(const CcColor& oColor, uint64 uiNumber = 1);

  /**
   * @brief Get inner rectangle of window
   * @return Inner rectangel sizes
   */
  virtual CcRectangle getInnerRect();

  /**
   * @brief Set area for painting and drawing pixels
   * @param oRectangle: Target area for paingin
   * @return True if pixel area is valid
   */
  bool setPixelArea(const CcRectangle& oRectangle);

  //! @return Get main widget of window
  CcWidget* getWidget();
  //! @return Get on close handler
  CcEventHandler& getCloseHandler();
  //! @return Get on mouse event handler
  CcMouseEventHandler& getMouseEventHandler();
  //! @return Get current title
  const CcString& getTitle();
  //! @return Get current size of window
  const CcSize& getSize() const;
  //! @return Get current postition of window
  const CcPoint& getPos() const;
  //! @return Get current height of window
  int32 getHeight() const;
  //! @return Get current width of window
  int32 getWidth() const;
  //! @return Get current state of window as enum
  EWindowState getState();

  //! @param eState: New state to set to window
  void setWindowState(EWindowState eState);
  //! @param sTitle: Change title of window
  void setTitle(const CcString& sTitle);
  //! @param oSize: Change size of window
  void setSize(const CcSize& oSize);
  //! @param oPos: Change position of window
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
  static CcWindow* Null;  //!< Default null window element
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcWindow>;
#endif

/**
 * @brief Define window pointer as shared pointer
 */
typedef CcSharedPointer<CcWindow> CcWindowPointer;
