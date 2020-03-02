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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTimer
 */

#include "CcTimer.h"
#ifdef LINUX

#elif defined(GENERIC)
#else
CcTimer::CcTimer()
{

}

CcTimer::~CcTimer()
{

}

CcStatus CcTimer::setRepeates(size_t uiRepeates)
{
  m_uiRepeates = uiRepeates;
  return true;
}

bool CcTimer::timeout()
{
  bool bTimerDone = false;
  if(m_uiRepeates != 0)
  {
    if(m_uiRepeatesCount < m_uiRepeates)
    {
      m_uiRepeatesCount++;
      m_oEventHandler.call(this);
    }
  }
  else
  {
    m_oEventHandler.call(this);
  }
  return bTimerDone;
}
#endif
