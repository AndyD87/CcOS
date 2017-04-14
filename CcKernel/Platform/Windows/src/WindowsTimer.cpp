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
 * @brief     Class WindowsTimer
 **/
#include "WindowsTimer.h"


WindowsTimer::WindowsTimer() {
}

WindowsTimer::~WindowsTimer() {
  // nothing to do
}

void WindowsTimer::delayMs(uint32 uiDelay)
{
  CCUNUSED(uiDelay);
}

void WindowsTimer::delayS(uint32 uiDelay)
{
  CCUNUSED(uiDelay);
}

void WindowsTimer::tick( void )
{

}

bool WindowsTimer::open(EOpenFlags flags) {
  CCUNUSED(flags);
  return false;
}

bool WindowsTimer::close(void)
{
  return true;
}

size_t WindowsTimer::read(char* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}

size_t WindowsTimer::write(const char* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}