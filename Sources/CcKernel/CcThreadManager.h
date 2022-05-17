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
 * @brief     Class CcThreadManager
 */
#pragma once

#include "CcBase.h"
#include "IThread.h"
#include "CcVector.h"
#include "CcMutex.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<IThread*>;
#endif

/**
 * @brief Thread manager to get an overview of running threads.'
 *        All threads can be closed if required.
 */
class CcKernelSHARED CcThreadManager
{
public:
  CcThreadManager() = default;
  ~CcThreadManager() = default;

  /**
   * @brief Add thread to manager
   * @param pThread: Target thread to add
   */
  void addThread(IThread* pThread);

  /**
   * @brief Remove existing thread from manager.
   * @param pThread: Target thread to remove
   * @return True if thread was found and closed
   */
  bool removeThread(IThread* pThread);

  /**
   * @brief Close all running threads
   */
  void closeAll();

private:
  CcVector<IThread*> m_oThreadList;
  CcMutex            m_oThreadListLock;

  static const int c_iThreadWaitingTime = 10;
  static const int c_iThreadDelayTime = 1000 / c_iThreadWaitingTime;
};
