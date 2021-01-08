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
 * @subpage   EEventType
 *
 * @page      EEventType
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS @par       Language: C++11
 * @brief     Class EEventType
 **/
#ifndef H_EEventType_H_
#define H_EEventType_H_

#include "CcBase.h"
#include "CcKernel.h"
#include "CcEventHandleMap.h"

enum class EEventType
{
  NoEvent = 0,
  Undefined = 0,

  WindowEvent = 0x100,
  WindowRestore,
  WindowNormal,
  WindowMinimized,
  WindowMaximimized,
  WindowTray,
  WindowClose,
  WindowSize,
  WindowPosition,
  WindowEventMax,

  MouseEvent = 0x200,
  MouseMove,
  MouseHover,
  MouseLeave,
  MouseLeftDown,
  MouseLeftUp,
  MouseLeftDoubleClick,
  MouseRightDown,
  MouseRightUp,
  MouseRightDoubleClick,
  MouseMiddleDown,
  MouseMiddleUp,
  MouseMiddleDoubleClick,
  MouseEventMax,

  KeyEvent = 0x300,
  KeyDown,
  KeyUp,
  KeyEventMax,

  StyleEvent = 0x400,
  StyleBackgroundImage,
  StyleBackgroundColor,
  StyleForegroundColor,
  StyleBorderStyle,
  StyleHoverColor,
  StyleFillParent,
  StyleEventMax,

  WidgetEvent = 0x500,
  WidgetSetFocus,
  WidgetHide,
  WidgetShow,
  WidgetEventMax,
};

#ifdef _MSC_VER
template class CcKernelSHARED CcEventHandleMap<EEventType>;
#endif

typedef CcEventHandleMap<EEventType> CcGuiEventMap;

#endif // H_EEventType_H_
