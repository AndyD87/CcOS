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
 * @brief     Class CcInputEvent
 */
#pragma once

#include "CcBase.h"
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
  /**
   * @brief Create input event with type of inheriting class
   * @param eType: Type of input event
   */
  CcInputEvent(EEventType eType) :
    m_eEventType(eType)
  {}

  /**
   * @brief Get type of stored input event, Mouse/Keyboard
   * @return type of event
   */
  inline EEventType getType()
  { return m_eEventType; }

  //! @param eType: Set type of input event
  inline void setType(EEventType eType)
  { m_eEventType = eType; }
  //! @brief Set handled
  inline void setHandled()
  { m_bHandled = true; }

  //! @return True if event was already handled
  inline bool isHandled()
  { return m_bHandled; }
  //! @return True if event is a mouse event
  inline bool isMouseEvent()
  { return m_eEventType >= EEventType::MouseEvent && m_eEventType <= EEventType::MouseEventMax; }
  //! @return True if event is a key event
  inline bool isKeyEvent()
  { return m_eEventType >= EEventType::KeyEvent && m_eEventType <= EEventType::KeyEventMax; }
  //! @return True if event is a style event
  inline bool isStyleEvent()
  { return m_eEventType >= EEventType::StyleEvent && m_eEventType <= EEventType::StyleEventMax; }
protected:
  EEventType m_eEventType;        //!< Type of inheriting class
  bool       m_bHandled = false;  //!< Marker if input was already handled
};

/**
 * @brief Mouse event
 */
class CcKernelSHARED CcMouseEvent : public CcInputEvent
{
public:
  CcMouseEvent() :
    CcInputEvent(EEventType::MouseEvent)
  { }

  /**
   * @brief Create a mouse event with parameters of a mouse event
   * @param eType:        Target EEventType::MouseEventXXX type
   * @param uiXorWheels:  Depending on eType, x coordinate or type or wheel value
   * @param uiY:          Y coordinate of mouse event
   */
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
  //! @return True if button left was pressed
  inline bool isLeftDown() const
    { return IS_FLAG_SET(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON); }
  //! @return True if button left was released
  inline bool isLeftUp() const
    { return IS_FLAG_NOT_SET(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON); }
  //! @return True if button right was pressed
  inline bool isRightDown() const
    { return IS_FLAG_SET(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON); }
  //! @return True if button right was released
  inline bool isRightUp() const
    { return IS_FLAG_NOT_SET(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON); }
  //! @return True if button center was pressed
  inline bool isMiddleDown() const
    { return IS_FLAG_SET(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON); }
  //! @return True if button center was released
  inline bool isMiddleUp() const
    { return IS_FLAG_NOT_SET(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON); }
  /**
   * @}
   */

public:
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
  /**
   * @brief Key by enum
   */
  enum class EKey : uint32
  {
    Unknown = 0,
    ESC = 0x1000000,
    FotoKey = 0x1000004,
  };

  /**
   * @brief Init key with target event type
   * @param eEvent:  Event of type EEventType::KeyEvent
   */
  CcKeyEvent(EEventType eEvent = EEventType::KeyEvent) :
    CcInputEvent(eEvent)
  { }

  //! @return get stored event key
  EKey getKey() const
  { return static_cast<EKey>(uiKey); }

  uint32 uiKey; //!< Key as num of EKey
};

/**
 * @brief Style Event
 */
class CcKernelSHARED CcStyleEvent : public CcInputEvent
{
public:
  /**
   * @brief Create style event for window/widget settings.
   * @param eEvent: Event of type EEventType::StyleEvent
   */
  CcStyleEvent(EEventType eEvent = EEventType::StyleEvent) :
    CcInputEvent(eEvent)
  { }
};
