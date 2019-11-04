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
#ifndef H_CcInputEvent_H_
#define H_CcInputEvent_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "EEventType.h"

#define CC_MOUSE_FLAG_LEFT_BUTTON   0x0001      //!< Flag for identify left button event
#define CC_MOUSE_FLAG_RIGHT_BUTTON  0x0002      //!< Flag for identify right button event
#define CC_MOUSE_FLAG_MIDDLE_BUTTON 0x0004      //!< Flag for identify middle button event

 /**
  * @brief Default Class for Input Events like Keyboard, Mouse
  */
class CcKernelSHARED CcInputEvent
{
public:
  CcInputEvent(EEventType eType) :
    m_eEventType(eType)
  {}

  /**
   * @brief Get type of stored input event, Mouse/Keyboard
   * @return type of event
   */
  inline EEventType getType()
    { return m_eEventType; }

  bool isMouseEvent()
    { return m_eEventType >= EEventType::MouseEvent && m_eEventType <= EEventType::MouseEventMax; }
  bool isKeyEvent()
    { return m_eEventType >= EEventType::KeyEvent && m_eEventType <= EEventType::KeyEventMax; }

private:
  EEventType m_eEventType; //!< Type of Storage, default undefined
};

class CcKernelSHARED CcMouseEvent : public CcInputEvent
{
public:
  CcMouseEvent() :
    CcInputEvent(EEventType::MouseEvent)
  { }

  CcMouseEvent(EEventType eType, uint16 uiXorWheels, uint16 uiY);

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
  EEventType eType = EEventType::Undefined; //!< Type of mouse event
  int32 x               = 0;                          //!< X Coordinate of Mouse
  int32 y               = 0;                          //!< Y Coordinate of Mouse
  uint32 MouseFlags     = 0;                          //!< Additional Mouse flags to specify Event
};

/**
 * @brief Keyboard Event
 */
class CcKernelSHARED CcKeyEvent : public CcInputEvent
{
public:
  CcKeyEvent() :
    CcInputEvent(EEventType::KeyEvent)
  { }
  bool bKeyDown; //!< Type of event, up/down
};

/**
 * @brief Storage union for Input Events
 */

#endif // H_CcInputEvent_H_
