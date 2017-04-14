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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsDisplay
 */

#include "WindowsDisplay.h"
#include "CcSystem.h"
#include "windowsx.h"
#include "CcKernel.h"

CcVector<sWindowHandle> WindowsDisplay::m_WidgetList;
uint16 WindowsDisplay::s_WindowCnt = 0;

WindowsDisplay::WindowsDisplay(void) :
  CcDisplay(CcSize(0,0))
{}

LRESULT CALLBACK WindowsDisplay::mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT lRet = 0;
  WindowsDisplayArea* wRet = getWindow(hWnd);
  if (wRet != 0)
  {
    lRet = wRet->executeMessage(hWnd, message, wParam, lParam);
  }
  else
    lRet = DefWindowProc(hWnd, message, wParam, lParam);
  return lRet;
}

void WindowsDisplay::registerWindow(HWND hWnd, WindowsDisplayArea* Window)
{
  sWindowHandle sItem;
  sItem.hWindow = hWnd;
  sItem.Window = Window;
  m_WidgetList.append(sItem);
}

void WindowsDisplay::deleteWindow(HWND hWnd, WindowsDisplayArea* Window)
{
  sWindowHandle sItem;
  sItem.hWindow = hWnd;
  sItem.Window = Window;
  for (uint16 i = 0; i < m_WidgetList.size(); i++)
  {
    if (m_WidgetList.at(i).hWindow == hWnd &&
      m_WidgetList.at(i).Window == Window)
      m_WidgetList.remove(i);
  }
}

WindowsDisplayArea* WindowsDisplay::getWindow(HWND hWnd)
{
  for (uint16 i = 0; i < m_WidgetList.size(); i++)
  {
    if (m_WidgetList.at(i).hWindow == hWnd)
      return m_WidgetList.at(i).Window;
  }
  return nullptr;
}

bool WindowsDisplay::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

bool WindowsDisplay::close(void)
{
  return true;
}

void WindowsDisplay::setBackgroundLed(bool bState)
{
  CCUNUSED(bState);
}

CcDisplayArea* WindowsDisplay::getDisplayArea(const CcRectangle& oArea)
{
  CcDisplayArea* wRet = new WindowsDisplayArea(s_WindowCnt, oArea); CCMONITORNEW(wRet);
  s_WindowCnt++;
  return wRet;
}
