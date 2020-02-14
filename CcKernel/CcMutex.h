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
 * @page      CcKernel
 * @subpage   CcMutex
 *
 * @page      CcMutex
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMutex
 */
#ifndef H_CcMutex_H_
#define H_CcMutex_H_

#include "CcBase.h"
#include "CcBase.h"
#ifdef _MSC_VER
  #include <mutex>
#endif

/**
 * @brief This object simplifies the synchronization of threads.
 */
class CcKernelSHARED CcMutex
{
public:
  CcMutex();
  ~CcMutex();
  /**
   * @brief Aquire a lock on this mutex. Release Mutex with unlock.
   *        It will wait until Mutex is unlocked from currently holding Object.
   *
   *        Check if lock is possible with isLocked() to avoid endless waitings.
   */
  void lock();

  /**
   * @brief Release a lock on this mutex.
   *        It is required that lock was called before, otherwise it will unlock an other session.
   */
  void unlock();

  /**
   * @brief Check if mutex is already locked
   * @return true if mutex is locked.
   */
  bool isLocked();

private:
#ifdef _MSC_VER
  std::mutex* m_oMutex;
#else
   volatile bool m_bLocked = false;
#endif
};

#endif // H_CcMutex_H_
