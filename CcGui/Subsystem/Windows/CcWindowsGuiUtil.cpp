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
 * @brief     Class CcWindowsGuiUtil
 */

#include "CcWindowsGuiUtil.h"

uint CcWindowsGuiUtil::s_uiIdInc = 2000;

void CcWindowsGuiUtil::trackMouse(HWND hWnd)
{
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_HOVER | TME_LEAVE; //Type of events to track & trigger.
  tme.dwHoverTime = 1; //How long the mouse has to be in the window to trigger a hover event.
  tme.hwndTrack = hWnd;
  TrackMouseEvent(&tme);
}

CcMouseEvent CcWindowsGuiUtil::fromCPoint(EEventType eType, const CPoint& rPoint)
{
  CcMouseEvent oEvent;
  oEvent.eType = eType;
  oEvent.x = rPoint.x;
  oEvent.y = rPoint.y;
  return oEvent;
}