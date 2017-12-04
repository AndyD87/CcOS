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

void CcMouseEvent::setLeft(bool bDown)
{
  if (bDown)
    SET_FLAG(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
  else
    REMOVE_FLAG(MouseFlags, CC_MOUSE_FLAG_LEFT_BUTTON);
}

void CcMouseEvent::setRight(bool bDown)
{
  if (bDown)
    SET_FLAG(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON);
  else
    REMOVE_FLAG(MouseFlags, CC_MOUSE_FLAG_RIGHT_BUTTON);
}

void CcMouseEvent::setMiddle(bool bDown)
{
  if (bDown)
    SET_FLAG(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON);
  else
    REMOVE_FLAG(MouseFlags, CC_MOUSE_FLAG_MIDDLE_BUTTON);
}

void CcInputEvent::setMouseEvent(EMouseEventType eType, uint16 uiXorWheels, uint16 uiY)
{
  m_eEventType = EInputEventType::Mouse;
  m_oEventStorage.oMouseEvent.eType = eType;
  m_oEventStorage.oMouseEvent.x = uiXorWheels;
  m_oEventStorage.oMouseEvent.y = uiY;
  m_oEventStorage.oMouseEvent.MouseFlags = 0;
}
