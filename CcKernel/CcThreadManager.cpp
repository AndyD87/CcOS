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

bool CcThreadManager::removeThread(IThread* pThread)
{
  m_oThreadListLock.lock();
  bool bFound = m_oThreadList.removeItem(pThread);
  m_oThreadListLock.unlock();
  return bFound;
}

void CcThreadManager::closeAll()
{
  m_oThreadListLock.lock();
  for (IThread *pThread : m_oThreadList)
  {
    pThread->stop();
  }
  m_oThreadListLock.unlock();
  size_t uiTimeout = 1000;
  while (m_oThreadList.size() > 0 && --uiTimeout > 0)
  {
    CcKernel::sleep(1);
  }
  // Clear list, but it looks like that have errors!
  if (uiTimeout == 0)
    m_oThreadList.clear();
}
