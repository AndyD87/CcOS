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
 * @copyright Andreas Dirmeier (C) 2021
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IWlan
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "CcThreadContext.h"

class IThread;
class CcThreadContext;

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED ICpu : public IDevice
{
public: // types
  //! System tick method declaration
  typedef void(*FSystemTick)();
  //! Thread tick method declaration
  typedef void(*FThreadTick)();
  //! Thread function for CPU entries
  typedef void(*FThreadFunction)(void* pParam);

public:
  ICpu() = default;
  virtual ~ICpu() = default;

  /**
   * @brief Get number of cores available on cpu
   * @return Number of cores
   */
  virtual size_t coreNumber() = 0;

  /**
   * @brief Get system main thread
   * @return Pointer to running system main thread
   */
  virtual CcThreadContext* mainThread() = 0;

  /**
   * @brief Create new system thread.
   * @param pTargetThread: Target thread data to execute on start.
   * @return Handle to thread with cpu context
   */
  virtual CcThreadContext* createThread(IThread* pTargetThread) = 0;

  /**
   * @brief Load Thread and execute it next.
   * @param pThreadData: Netxt thread to execute
   */
  virtual void loadThread(CcThreadContext* pThreadData) = 0;

  /**
   * @brief Thread is done, clean up
   * @param pThreadData: Thread to cleanup
   */
  virtual void deleteThread(CcThreadContext* pThreadData) = 0;

  /**
   * @brief Change current thread to next loaded thread.
   */
  virtual void nextThread() = 0;

  /**
   * @brief Get context of currently running thread.
   * @return Handle to current thread.
   */
  virtual CcThreadContext* currentThread() = 0;

  /**
   * @brief Each heap has pattern at the end. if they are changed,
   *        Thread was working behind the heap.
   * @return True if overflow happended
   */
  virtual bool checkOverflow() = 0;

  /**
   * @brief Disable all interrupts
   */
  virtual void enterCriticalSection() = 0;

  /**
   * @brief Enable all interrupts
   */
  virtual void leaveCriticalSection() = 0;

  /**
   * @brief Are we running in interupt context
   * @return True if we are in interrupt service routine
   */
  virtual bool isInIsr() = 0;

  /**
   * @brief Set a method to call ever system tick has called
   * @param pSystemTickMethod
   */
  void setSystemTick(FSystemTick pSystemTickMethod)
  { m_pSystemTickMethod = pSystemTickMethod; }

  /**
   * @brief Set a method wich get called if
   * @param pThreadTickMethod
   */
  void setThreadTick(FThreadTick pThreadTickMethod)
  { m_pThreadTickMethod = pThreadTickMethod; }

  /**
   * @brief Create and start thread method.
   * @param pParam: Thread to start.
   */
  static void CreateThreadMethod(CcThreadContext* pParam);

protected:
  /**
   * @brief Change thred context.
   */
  virtual void changeThread() = 0;

  /**
   * @brief Execute a system tick.
   */
  virtual void tick() = 0;
protected:
  FSystemTick m_pSystemTickMethod = nullptr;  //!< Target method to call for System tick event
  FThreadTick m_pThreadTickMethod = nullptr;  //!< Target method to call for Thread tick event
};
