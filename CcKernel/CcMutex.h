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
#ifdef USE_STD_MUTEX
  #include <mutex>
  #define CcMutex_Type std::mutex
#elif defined(LINUX)
  #include <pthread.h>
  #define CcMutex_Type pthread_mutex_t
#elif defined(WINDOWS)
  #include <windows.h>
  #define CcMutex_Type CRITICAL_SECTION
#else
  #define CcMutex_Type uint32
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
   * @brief Aquire a lock on this mutex if possible. Release Mutex with unlock.
   *        If lock will not be available, this method will return immediatly.
   * @return true if successfully locked, otherwise false
   */
  bool tryLock();

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
  CcMutex_Type m_oContext;
};

#endif // H_CcMutex_H_
