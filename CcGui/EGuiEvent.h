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
 * @page      Windows
 * @subpage   EGuiEvent
 *
 * @page      EGuiEvent
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class EGuiEvent
 **/
#ifndef _EGuiEvent_H_
#define _EGuiEvent_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcEventHandleMap.h"

enum class EGuiEvent
{
  NoEvent = 0,
  WindowEvent = 0x100,
  WindowRestore,
  WindowNormal,
  WindowMinimized,
  WindowMaximimized,
  WindowTray,
  WindowClose,
  WindowSize,
  WindowEventMax = 0x1ff,

  MouseEvent = 0x200,
  MouseMove,
  MouseHover,
  MouseLeave,
  MouseLeftDown,
  MouseLeftUp,
  MouseLeftDoubleClick,
  MouseEventMax = 0x2ff,

  KeyEvent = 0x300,
  KeyDown,
  KeyUp,
  KeyEventMax = 0x3ff,
};

#ifdef WIN32
template class CcGuiSHARED CcEventHandleMap<EGuiEvent>;
#endif

typedef CcEventHandleMap<EGuiEvent> CcGuiEventMap;

#endif /* _EGuiEvent_H_ */
