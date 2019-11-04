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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcInputEvent
 */
#include "CcInputEvent.h"

CcMouseEvent::CcMouseEvent(EEventType eType, uint16 uiXorWheels, uint16 uiY) :
  CcInputEvent(EEventType::MouseEvent)
{
  eType = eType;
  x = uiXorWheels;
  y = uiY;
  MouseFlags = 0;
}

void CcMouseEvent::setLeft(bool bDown)
{
  if (bDown)
  {
    SET_FLAG(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
    eType = EEventType::MouseLeftDown;
  }
  else
  {
    REMOVE_FLAG(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
    eType = EEventType::MouseLeftUp;
  }
}

void CcMouseEvent::setRight(bool bDown)
{
  if (bDown)
  {
    SET_FLAG(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON);
    eType = EEventType::MouseRightDown;
  }
  else
  {
    REMOVE_FLAG(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON);
    eType = EEventType::MouseRightUp;
  }
}

void CcMouseEvent::setMiddle(bool bDown)
{
  if (bDown)
  {
    SET_FLAG(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON);
    eType = EEventType::MouseMiddleDown;
  }
  else
  {
    REMOVE_FLAG(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON);
    eType = EEventType::MouseMiddleUp;
  }
}
