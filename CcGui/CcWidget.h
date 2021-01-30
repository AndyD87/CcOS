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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWidget
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcObject.h"
#include "CcEventHandleMap.h"
#include "CcInputEvent.h"
#include "Types/CcColor.h"
#include "Types/CcRectangle.h"
#include "Style/CcStyle.h"
#include "CcSharedPointer.h"
#include "EEventType.h"
#include "Style/CcStyleWidget.h"

/**
 * @brief Current visual state of Window as enum
 */
enum class EWindowState
{
  Close,
  Normal,
  Minimized,
  Maximimized,
  FullScreen,
  Hide,
  Tray,
};

class CcWindow;

/**
 * @brief Basic class of every visual element on displays and windows.
 */
class CcGuiSHARED CcWidget : public CcObject
{
public:
  /**
   * @brief An empty CcWidget will be initialized with a parent object if available.de
   * @param rParent: Parent Widget of this widget.
   */
  CcWidget(CcWidget* rParent);

  /**
   * @brief Create a widget with parent and define it's sized
   * @param oWindowRect:  Rectangle with position and size of widget in parent.
   * @param rParent:      Parent where widget will be displayed in.
   */
  CcWidget(const CcRectangle& oWindowRect, CcWidget* rParent);

  /**
   * @brief Create a widget with parent and define it's sized
   * @param iPosX:    X Coordinate of widget start
   * @param iPosY:    Y Coordinate of widget start
   * @param uiWidth:  Width of widget
   * @param uiHeight: Height of widget
   * @param rParent:  Parent where widget will be displayed in.
   */
  CcWidget(int32 iPosX, int32 iPosY, int32 uiWidth, int32 uiHeight, CcWidget* rParent);

  CcWidget(const CcWidget& oToCopy) = delete;
  virtual ~CcWidget();

  //! @param oSize: New size for this widget.
  void setSize(const CcSize& oSize);
  //! @param sPath: Set background image for this widget
  void setBackgroundImage(const CcString& sPath);
  //! @param oColor: Set color for background
  void setBackgroundColor(const CcColor& oColor);
  //! @param oColor: Set color for foreground
  void setForegroundColor(const CcColor& oColor);
  //! @brief Change focus to this widget.
  void setFocus();
  //! @param oPoint: Update location within parent widget
  void setPos(const CcPoint& oPoint);
  //! @param oRectangle: Set new size and new position of widget.
  void setRectangle(const CcRectangle& oRectangle);
  //! @param rParent: Change parent element.
  void setParent(CcWidget* rParent);
  //! @param pStyleSheet: Change complete style of widget.
  void setStyle(const CcStyleWidget& pStyleSheet);
  //! @param eWindowState: Change window state to hide, maxmize, etc. the widget.
  void setWindowState(EWindowState eWindowState);
  //! @param hSubSystem: Set new subsystem handle
  void setSubSystemHandle(void* hSubSystem);
  //! @param oColor: Change border color of widget
  void setBorderColor(const CcColor& oColor);
  //! @param uiSize: Change border size of widget
  void setBorderSize(uint16 uiSize);

  //! @return
  const CcColor& getBorderColor();
  //! @return
  uint32 getBorderSize();
  //! @return
  CcWidget* getParent();
  //! @return
  CcWidget* getHitTest(const CcPoint& oPointToFind);
  //! @return
  virtual const CcSize& getSize();
  //! @return
  CcStyleWidget& getStyle();
  //! @return
  const CcStyleWidget& getStyle() const;
  /**
   * @brief Get a Subsystem defined Handle
   * @return Handle as Pointer, Type is defined by Subsystem.
   */
  virtual void* getSubSysHandle();

  //! @return Get position and size of widget as rectangle
  const CcRectangle& getRectangle() const;
  //! @return Get inner rectangle, relative to rectangle without border and margins
  CcRectangle getInnerRect() const;
  //! @return Get current width of widget
  int32 getWidth() const;
  //! @return Get current height of widget
  int32 getHeight() const;
  //! @return Get current position of widget within parent
  const CcPoint& getPos();
  //! @return Get current window state, if it is minimized, hidden, etc.
  EWindowState getWindowState();
  //! @return Get current background color
  const CcColor& getBackgroundColor();
  //! @return Get current foreground color
  const CcColor& getForegroundColor();
  //! @return Get list of child elements within this widget
  const CcList<CcWidget*>& getChildList();

  /**
   * @brief Append child to child list, this widget becomes parent of appended child.
   * @param oChildWidget: Child to append
   */
  void registerChild(CcWidget* oChildWidget);

  /**
   * @brief Remove child from current child list:
   * @param oChildWidget: Target widget to remove
   * @return True if child was found and removed.
   */
  bool removeChild(CcWidget* oChildWidget);

  /**
   * @brief Register an event receiver for specific event the widget receives.
   * @param eEvent:       Target type of event to receive
   * @param eEventHandle: Target action to do on event.
   */
  void registerOnEvent(EEventType eEvent, const CcEvent& eEventHandle);

  /**
   * @brief Remove event receiver from list by event type and object:
   * @param eEvent:   Target event type to remove from list
   * @param pObject:  Target object to remove from receiver.
   */
  void removeOnEvent(EEventType eEvent, CcObject* pObject);

  /**
   * @brief Increase size of child until inner rect of parent widget is reached.
   */
  void fillParent();

  /**
   * @brief Hide current widget
   */
  void hide();

  /**
   * @brief Show current wideget if it was hidden
   */
  void show();

  /**
   * @brief Execute event on widget.
   * @param pEventData: Incomming event data
   */
  virtual void event(CcInputEvent* pEventData);

  /**
   * @brief Redraw the widget and it's child
   * @param bDoFlush: Do instant flush, otherwise drawing will managed by subsystem
   */
  virtual void draw(bool bDoFlush = true);

  /**
   * @brief Draw a specific pixel on current widget cursor
   * @param oColor:   Color of pixel
   * @param uiNumber: Number of repeats
   */
  virtual void drawPixel(const CcColor& oColor, uint64 uiNumber = 1);

  /**
   * @brief Flush drawing of widget
   */
  virtual void flush();

  //! @return Get inner rect of widget.
  virtual CcRectangle getInnerRect();

  //! @return Get window if this widget is window, otherwise query will be
  //!         forwarded to parent.
  virtual CcWindow* getWindow();
  //! @return Set pixel area for drawing and reset cursor to beginning
  virtual bool setPixelArea(const CcRectangle& oRectangle);

protected:
  /**
   * @brief Fill background with specific color
   * @param oColor: Target collor for background
   */
  void drawBackground(const CcColor& oColor);

  /**
   * @brief Draw border onspecific width inside around the widget
   * @param oColor: Target color of border
   * @param uiSize: Width of border
   */
  void drawBorder(const CcColor& oColor, uint32 uiSize);

  /**
   * @brief Send draw command to all childs.
   */
  void drawAllChilds();

  /**
   * @brief This method will be called if event() was received.
   * @param pEventData: Event data to forward
   */
  virtual void onEvent(CcInputEvent* pEventData);

  /**
   * @brief This method will be called if event() received an mouse event.
   * @param pMouseEvent: Mouse event data to forward
   */
  virtual void onMouseEvent(CcMouseEvent* pMouseEvent);

  /**
   * @brief This method will be called if event() received an key event.
   * @param pEventData: Key event data to forward
   */
  virtual void onKeyEvent(CcKeyEvent* pEventData);

  /**
   * @brief This method will be called if event() received an window event like stop, hide, etc.
   * @param pEventData: Event data to forward
   */
  virtual void onWindowEvent(CcInputEvent* pEventData);

  /**
   * @brief Enable custom paintin on this widget.
   *        Inheriting object will care of full drawing.
   * @param bEnable: True for enable, False for disable.
   */
  virtual void setCustomPainting(bool bEnable);

  /**
   * @brief Will be called if window dimension or postion was changed
   */
  virtual void onRectangleChanged();

  /**
   * @brief Will be called if background color was changed
   */
  virtual void onBackgroundChanged();

  /**
   * @brief Will be called if foreground color was changed
   */
  virtual void onForegroundChanged();

private:
  void initWidget(CcWidget* rParent);

private: // Types
  class CPrivate;
private: // Member
  CPrivate*             m_pPrivate = nullptr;
  static CcWidget* s_hNullHandle;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcWidget>;
#endif

//! @brief Shared pointer on a widget.
typedef CcSharedPointer<CcWidget> CcWidgetPointer;
