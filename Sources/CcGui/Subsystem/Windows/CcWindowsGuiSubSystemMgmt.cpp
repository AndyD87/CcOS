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
 */

#include "CcWindowsGuiSubSystemMgmt.h"
#include "CcSystem.h"
#include "windowsx.h"
#include "CcKernel.h"

CcVector<CWindowHandle> CcWindowsGuiSubSystemMgmt::m_WidgetList;
uint16 CcWindowsGuiSubSystemMgmt::s_WindowCnt = 0;

LRESULT CALLBACK CcWindowsGuiSubSystemMgmt::mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT lRet = 0;
  CcGuiSubsystem* wRet = getWindow(hWnd);
  if (wRet != nullptr)
  {
    lRet = wRet->executeMessage(hWnd, message, wParam, lParam);
  }
  else
    lRet = DefWindowProc(hWnd, message, wParam, lParam);
  return lRet;
}

void CcWindowsGuiSubSystemMgmt::registerWindow(HWND hWnd, CcGuiSubsystem* Window)
{
  CWindowHandle sItem;
  sItem.hWindow = hWnd;
  sItem.Window = Window;
  m_WidgetList.append(sItem);
}

void CcWindowsGuiSubSystemMgmt::deleteWindow(HWND hWnd, CcGuiSubsystem* Window)
{
  CWindowHandle sItem;
  sItem.hWindow = hWnd;
  sItem.Window = Window;
  for (uint16 i = 0; i < m_WidgetList.size(); i++)
  {
    if (m_WidgetList.at(i).hWindow == hWnd &&
      m_WidgetList.at(i).Window == Window)
      m_WidgetList.remove(i);
  }
}

CcGuiSubsystem* CcWindowsGuiSubSystemMgmt::getWindow(HWND hWnd)
{
  for (uint16 i = 0; i < m_WidgetList.size(); i++)
  {
    if (m_WidgetList.at(i).hWindow == hWnd)
      return m_WidgetList.at(i).Window;
  }
  return nullptr;
}
