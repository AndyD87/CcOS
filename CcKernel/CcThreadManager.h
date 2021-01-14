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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcThreadManager
 */
#ifndef H_CCTHREADMANAGER_H_
#define H_CCTHREADMANAGER_H_

#include "CcBase.h"
#include "CcBase.h"
#include "IThread.h"
#include "CcVector.h"
#include "CcMutex.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<IThread*>;
#endif

/**
 * @brief Default Class to create a Application
 */
class CcKernelSHARED CcThreadManager
{
public:
  CcThreadManager();

  void addThread(IThread* pThread);
  bool removeThread(IThread* pThread);
  void closeAll();

private:
  CcVector<IThread*> m_oThreadList;
  CcMutex            m_oThreadListLock;

  static const int c_iThreadWaitingTime = 10;
  static const int c_iThreadDelayTime = 1000 / c_iThreadWaitingTime;
};

#endif // H_CcTHREADMANAGER_H_
