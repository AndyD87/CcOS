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
 * @brief     Class CcThreadManager
 */
#include "CcThreadManager.h"
#include "CcKernel.h"

CcThreadManager::CcThreadManager( void )
{
}

CcThreadManager::~CcThreadManager()
{
}

void CcThreadManager::addThread(CcThreadObject& oThread)
{
  m_ThreadList.append(&oThread);
}

void CcThreadManager::closeAll(void)
{
  while (m_ThreadList.size() > 0)
  {
    CcThreadObject *thread = m_ThreadList.at(0);
    if (thread->getThreadState() != EThreadState::Stopped)
      thread->enterState(EThreadState::Stopping);
    int iCounter = 0;
    while ( thread->getThreadState() != EThreadState::Stopped &&
            iCounter < c_iThreadWaitingTime)
    {
      iCounter++;
      CcKernel::delayMs(c_iThreadDelayTime);
    }
    m_ThreadList.remove(0);
  }
}
