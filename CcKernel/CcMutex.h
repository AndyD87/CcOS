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
#ifndef _CcMutex_H_
#define _CcMutex_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IWaitable.h"

/**
 * @brief This object simplifies the synchronization of threads.
 */
class CcKernelSHARED CcMutex : public IWaitable
{
public:
  CcMutex();

  /**
   * @brief The destructor will call lock and unlock before it ends.
   */
  ~CcMutex();

  /**
   * @brief Aquire a lock on this mutex. Release Mutex with unlock.
   *        It will wait until Mutex is unlocked from currently holding Object.
   *
   *        Check if lock is possible with isLocked() to avoid endless waitings.
   */
  void lock()
    { wait(); m_bLocked = true; }

  /**
   * @brief Release a lock on this mutex.
   *        It is required that lock was called before, otherwise it will unlock an other session.
   */
  void unlock()
    { signal(); }

  /**
   * @brief Check if mutex is already locked
   * @return true if mutex is locked.
   */
  bool isLocked()
  { return !condition(); }

  virtual void signal() override;
  virtual bool condition() override
    { return !m_bLocked; }

private:
   bool m_bLocked = false;
};

#endif /* _CcMutex_H_ */
