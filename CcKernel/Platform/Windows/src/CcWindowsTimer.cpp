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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindowsTimer
 **/
#include "CcWindowsTimer.h"


CcWindowsTimer::CcWindowsTimer() {
}

CcWindowsTimer::~CcWindowsTimer() {
  // nothing to do
}

void CcWindowsTimer::delayMs(uint32 uiDelay)
{
  CCUNUSED(uiDelay);
}

void CcWindowsTimer::delayS(uint32 uiDelay)
{
  CCUNUSED(uiDelay);
}

void CcWindowsTimer::tick( void )
{

}

CcStatus CcWindowsTimer::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

CcStatus CcWindowsTimer::close(void)
{
  return true;
}

size_t CcWindowsTimer::read(void* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}

size_t CcWindowsTimer::write(const void* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}