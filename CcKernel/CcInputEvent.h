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
 * @page      CcInputEvent
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcInputEvent
 */
#ifndef CcInputEvent_H_
#define CcInputEvent_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "EInputEventTypes.h"

#define CC_MOUSE_FLAG_LEFT_BUTTON   0x0001
#define CC_MOUSE_FLAG_RIGHT_BUTTON  0x0002
#define CC_MOUSE_FLAG_MIDDLE_BUTTON 0x0004

class CcMouseEvent 
{
public:
  void setLeft(bool bDown);
  void setRight(bool bDown);
  void setMiddle(bool bDown);
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
  EMouseEventType eType;
  uint16 x;
  uint16 y;
  uint16 MouseFlags;
};

class CcKeyboardEvent
{
public:
  bool bKeyDown;
};

typedef union
{
  CcMouseEvent    oMouseEvent;
  CcKeyboardEvent oKeyboardEvent;
} uEventStoarge;

/**
 * @brief Default Class for Input Events like Keyboard, Mouse
 */
class CcKernelSHARED CcInputEvent {
public:
  CcInputEvent()
    {}
  ~CcInputEvent()
    {}

  void setMouseEvent(EMouseEventType type, uint16 uiXorWheels, uint16 uiY);
  CcMouseEvent& getMouseEvent(void)
    { return m_oEventStorage.oMouseEvent; }

  inline EInputEventType getType() 
    { return m_eEventType; }

private:
  EInputEventType m_eEventType;
  uEventStoarge   m_oEventStorage;
};

#endif /* CcInputEvent_H_ */
