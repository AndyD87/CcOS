/**
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @file      CcLinuxTimer
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcLinuxTimer
 */
#include "CcLinuxTimer.h"


CcLinuxTimer::CcLinuxTimer() {
}

CcLinuxTimer::~CcLinuxTimer()
{
  // nothing to do
}

void CcLinuxTimer::delayMs(uint32 uiDelay)
{
  m_uiCountDown = uiDelay;
}

void CcLinuxTimer::delayS(uint32 uiDelay)
{
  m_uiCountDown = uiDelay * 1000;
}

void CcLinuxTimer::tick( void )
{
  m_uiCountDown--;
}
