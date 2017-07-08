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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTimer
 */

#include "Devices/CcTimer.h"

volatile uint32 CcTimer::s_CountDown = 0;

uint32 CcTimer::getCounterState( void )
{
  return s_CountDown;
}

CcTimer::CcTimer() {
}

CcTimer::~CcTimer() {
  // nothing to do
}

void CcTimer::tick()
{
  if(s_CountDown != 0)
    s_CountDown--;
}

bool CcTimer::open(EOpenFlags flags) {
  CCUNUSED(flags);
  return false;
}

bool CcTimer::close(void)
{
  return true;
}

size_t CcTimer::read(char* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}

size_t CcTimer::write(const char* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}
