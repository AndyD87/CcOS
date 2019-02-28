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
 * @page      CcKernel
 * @subpage   CcInputEvent
 *
 * @page      CcInputEvent
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcInputEvent
 */
#ifndef _CcInputEvent_H_
#define _CcInputEvent_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "EInputEventTypes.h"

#define CC_MOUSE_FLAG_LEFT_BUTTON   0x0001      //!< Flag for identify left button event
#define CC_MOUSE_FLAG_RIGHT_BUTTON  0x0002      //!< Flag for identify right button event
#define CC_MOUSE_FLAG_MIDDLE_BUTTON 0x0004      //!< Flag for identify middle button event

class CcMouseEvent 
{
public:
  /**
   * @brief Set event to an left button event.
   * @param bDown: set true if event should be a press event
   */
  void setLeft(bool bDown);

  /**
   * @brief Set event to an right button event.
   * @param bDown: set true if event should be a press event
   */
  void setRight(bool bDown);

  /**
   * @brief Set event to an middle button event.
   * @param bDown: set true if event should be a press event
   */
  void setMiddle(bool bDown);

  /**
   * @brief Check if an specific flag is set.
   * @return true correct flag was set.
   * @{
   */
  inline bool isLeftDown() const
    { return IS_FLAG_SET(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON); }
  inline bool isLeftUp() const
    { return IS_FLAG_NOT_SET(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON); }
  inline bool isRightDown() const
    { return IS_FLAG_SET(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON); }
  inline bool isRightUp() const
    { return IS_FLAG_NOT_SET(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON); }
  inline bool isMiddleDown() const
    { return IS_FLAG_SET(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON); }
  inline bool isMiddleUp() const
    { return IS_FLAG_NOT_SET(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON); }
  /**
   * @}
   */

public:
  EMouseEventType eType;  //!< Type of mouse event
  int32 x;               //!< X Coordinate of Mouse
  int32 y;               //!< Y Coordinate of Mouse
  uint32 MouseFlags;      //!< Additional Mouse flags to specify Event
};

/**
 * @brief Keyboard Event
 */
class CcKeyEvent
{
public:
  bool bKeyDown; //!< Type of event, up/down
};

/**
 * @brief Storage union for Input Events
 */
typedef union
{
  CcMouseEvent    oMouseEvent;
  CcKeyEvent oKeyboardEvent;
} UEventStoarge;

/**
 * @brief Default Class for Input Events like Keyboard, Mouse
 */
class CcKernelSHARED CcInputEvent
{
public:
  /**
   * @brief Set type of event to an mouse event
   * @param eType: Type of MouseEvent
   * @param uiXorWheels: X-Coordinate of mouse event, or if wheel event, number of turns.
   * @param uiY: Y-Coordinate of mouse event.
   */
  void setMouseEvent(EMouseEventType eType, uint16 uiXorWheels, uint16 uiY);

  /**
   * @brief Get MouseEvent of stored input data.
   * @return Reference to InputEvent
   */
  CcMouseEvent& getMouseEvent()
    { return m_oEventStorage.oMouseEvent; }

  /**
   * @brief Get KeyEvent of stored input data.
   * @return Reference to KeyEvent
   */
  CcKeyEvent& getKeyboardEvent()
    { return m_oEventStorage.oKeyboardEvent; }

  /**
   * @brief Get type of stored input event, Mouse/Keyboard
   * @return type of event
   */
  inline EInputEventType getType() 
    { return m_eEventType; }

private:
  EInputEventType m_eEventType    = EInputEventType::Undefined; //!< Type of Storage, default undefined
  UEventStoarge   m_oEventStorage;                              //!< Data for Event
};

#endif /* _CcInputEvent_H_ */
