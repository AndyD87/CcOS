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
 * @brief     Class CcGuiSubsystem
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcObject.h"
#include "CcEventHandler.h"
#include "CcString.h"
#include "Types/CcRectangle.h"
#include "Types/CcColor.h"
#include "CcHandle.h"
#include "CcSharedPointer.h"
#include "CcWidget.h"
#include "CcInputEvent.h"

class IDisplay;

#ifdef _MSC_VER
template class CcGuiSHARED CcHandle<IDisplay>;
#endif

/**
 * @brief Definition of a gui subsystem.
 *        This class will be created from a system depending gui platform.
 *        It can be a window on full os device or connection to display on generic devices.
 */
class CcGuiSHARED CcGuiSubsystem : public CcObject
{
public:
  /**
   * @brief Target window which created this interface.
   * @param hWindowHandle: Handle to parent window
   */
  CcGuiSubsystem(CcWindow* hWindowHandle);
  ~CcGuiSubsystem();

  /**
   * @brief Action loop of subsystem.
   */
  void loop();

  /**
   * @brief Draw a single pixel to subsystem, which has previously set
   *        an area and cursor.
   * @param oPixel:   Target color of pixel
   * @param uiNumber: Number of pixels to draw at once.
   */
  void drawPixel(const CcColor& oPixel, uint64 uiNumber);

  /**
   * @brief Refresh the display
   */
  void draw();

  /**
   * @brief Get maximum available area of the gui interface.
   * @param[out] oArea: Filled Area information.
   */
  void getMaxArea(CcRectangle& oArea);

  /**
   * @brief Hide current subsystem. On generic device, make all blank.
   */
  void hide();

  /**
   * @brief Show current subsystem, if it was hided before.
   */
  void show();

  /**
   * @brief Check if frame is available
   * @return
   */
  bool hasFrame();

  /**
   * @brief Get rect of drawable arrea.
   * @return Drawable area within maximum area
   */
  CcRectangle getInnerRect();

  /**
   * @brief Update size if it was change from external
   */
  void updateSize();

  /**
   * @brief Update position if it was change from external
   */
  void updatePos();

  /**
   * @brief Set new drawing area
   * @param oArea: Area to draw to.
   * @return true if pixelarea can be placed within this display area
   */
  bool setPixelArea(const CcRectangle& oArea);

  /**
   * @brief Set title of window in subsystem
   * @param sTitle: Taret title
   */
  void setWindowTitle(const CcString& sTitle);

  /**
   * @brief Change window state on subsystem.
   * @param eState: Target new state
   * @return True if change was successfuly
   */
  bool setWindowState(EWindowState eState);

  /**
   * A inheriting class can create a main widget, with it's default handle type.
   * @return Handly casted as void
   */
  void* getHandle();

  /**
   * @brief Set a drawing display as target for subsystem
   * @param pDisplay: Target display for CcGui
   */
  void setDisplay(const CcHandle<IDisplay>& pDisplay);

  //! @return Get handle of parent window
  CcWindow* getWindowHandle()
  { return m_hWindow; }
  //! @return Get handler fol all input events
  inline CcEventHandler& getInputEventHandler()
  {return m_InputEventHandler;}
  //! @return Get handler fol all control events.
  inline CcEventHandler& getControlEventHandler()
  {return m_ControlEventHandler;}

#ifdef WINDOWS
  /**
   * @brief An window message queue for forward input data to next instances.
   * @param hWnd:     Window handle we are running on
   * @param message:  Message receiced from loop
   * @param wParam:   First paramter with
   * @param lParam:   Second paramter depending on message
   * @return Return code 0 on success
   */
  intptr executeMessage(void* hWnd, uint32 message, intptr wParam, intptr lParam);
#endif
  /**
   * @brief Convert subsystem mouse event to CcMouseEvent
   * @param pEvent: Subsystem event.
   * @param[out] rEvent: Target CcMouse event to generate
   * @return Success if conversion succeeded, otherwise look at error message.
   */
  static CcStatus convertMouseEvent(void* pEvent, CcMouseEvent& rEvent);

  /**
   * @brief Convert subsystem keyboard event to CcKeyEvent
   * @param pEvent: Subsystem event.
   * @param[out] rEvent: Target CcKeyEvent event to generate
   * @return Success if conversion succeeded, otherwise look at error message.
   */
  static CcStatus convertKeyEvent(void* pEvent, CcKeyEvent& rEvent);

private:
  class CPrivate;
private:
  CPrivate*      m_pPrivate = nullptr;
  CcHandle<IDisplay> m_Display;
  CcEventHandler m_InputEventHandler;
  CcEventHandler m_ControlEventHandler;
  CcWindow* m_hWindow;
};

#ifdef _MSC_VER
template class CcGuiSHARED CcSharedPointer<CcGuiSubsystem>;
#endif
