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
 * @brief     Class CcThreadManager
 */
#include "CcThreadManager.h"
#include "CcKernel.h"

CcThreadManager::CcThreadManager()
{
}

void CcThreadManager::addThread(IThread* pThread)
{
  m_oThreadListLock.lock();
  m_oThreadList.append(pThread);
  m_oThreadListLock.unlock();
}

void CcThreadManager::removeThread(IThread* pThread)
{
  m_oThreadListLock.lock();
  m_oThreadList.removeItem(pThread);
  m_oThreadListLock.unlock();
}

void CcThreadManager::closeAll()
{
  while (m_oThreadList.size() > 0)
  {
    m_oThreadListLock.lock();
    IThread *pThread = m_oThreadList.at(0);
    m_oThreadListLock.unlock();
    pThread->stop();
    if(!pThread->waitForExit(CcDateTimeFromSeconds(1)))
    {
      CCDEBUG("Failed to stop thread: " + pThread->getName());
    }
    m_oThreadListLock.lock();
    if(m_oThreadList.size() > 0)
      m_oThreadList.remove(0);
    m_oThreadListLock.unlock();
  }
}
