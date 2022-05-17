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
 * @brief     Class CcWindowsGuiSubSystemMgmt
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWindowsGlobals.h"
#include "CcVector.h"
#include "CcGuiSubsystem.h"

class CcWindow;

/**
 * @brief Definition for connecting a windows window handle and a gui subsystem
 */
class CWindowHandle
{
public:
  HWND hWindow;           //!< Created window handle from windows
  CcGuiSubsystem* Window; //!< CcOS subsystem for handling windows events from hWindow
};

/**
 * @brief Manager for all created windows window handles and their interfaces to gui subsystems.
 */
class CcWindowsGuiSubSystemMgmt
{
public:
  /**
   * @brief Get subsystem connected to window handle
   * @param hWnd: Windows window handle
   * @return Associated subsystem
   */
  static CcGuiSubsystem* getWindow(HWND hWnd);

  /**
   * @brief Register an windows window handles and their interface to gui subsystems
   * @param hWnd:   Windows window handle
   * @param Window: Associated subsystem
   */
  static void registerWindow(HWND hWnd, CcGuiSubsystem* Window);

  /**
   * @brief Remove a connection by it's members
   * @param hWnd:   Windows window handle
   * @param Window: Associated subsystem
   */
  static void deleteWindow(HWND hWnd, CcGuiSubsystem* Window);

  /**
   * @brief Main windows process to receive gui event messages and forwarding them
   *        to coressponding subsystem
   * @param hWnd:     Window handle
   * @param message:  Received message from system loop
   * @param wParam:   Parameter for message
   * @param lParam:   Additional Parameter for message
   * @return TRUE if message was processed successfully
   */
  static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  //! @return True if at least one Window was created
  static bool hasOpenedWindows()
  { return m_WidgetList.size() > 0; }

private: //member
  static CcVector<CWindowHandle> m_WidgetList;
  static uint16 s_WindowCnt;
};
