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
 * @brief     Class IThread
 */
#pragma once

#include "CcBase.h"
#include "CcObject.h"
#include "CcString.h"
#include "CcDateTime.h"
#include "CcMutex.h"
#include "CcEventActionList.h"
#include "CcReferenceCount.h"
#include "CcEventHandler.h"

class CcKernel;

/**
 * @brief Possible state a thread can have in it's livecycle
 */
enum class EThreadState : uint8
{
  Starting,
  Running,
  Stopping,
  Stopped,
};

/**
 * @brief Default Class to create a Application
 */
class CcKernelSHARED IThread  : public CcObject, protected CcReferenceCount
{
public:
  /**
   * @brief Create thread instantce with name.
   * @param sName: Target name of thread
   */
  IThread(const CcString& sName = "");

  /**
   * @brief Destroy Object and waiting until @ref getThreadState is set to EThreadState::Stopped
   */
  virtual ~IThread();

  /**
   * @brief Virtual function for Running-Code
   *        Must be implemented by target application.
   */
  virtual void run  () = 0;

  /**
   * @brief This virtual Method will be triggerd if a shutdown for thread was initiaded on a running thread.
   *        Do not wait for other thread state here, just handle stop operation.
   */
  virtual void onStop(){}

  /**
   * @brief This method will be called as last command of thread before it get's closed.
   * @return Stored status of thread operations
   */
  virtual CcStatus onStopped()
  { return getExitCode(); }

  /**
   * @brief Virtual function for Startup-Code
   *        Can be implemnted by inhering Class.
   *        Gets called before thread is starting.
   */
  CcStatus start();

  /**
   * @brief Call this method if this Thread should get started without creating a new thread
   * @return void
   */
  CcStatus startOnCurrent();

  /**
   * @brief Call this method to start a thread on current context.
   *        Thread has already to be in a starting state
   * @return void
   */
  CcStatus startOnThread();

  /**
   * @brief Send stop command to thread
   */
  void stop()
  { enterState(EThreadState::Stopping); }

  /**
   * @brief Stop Event receiver
   * @param pParam: not in use
   * @return send stop to thread
   */
  void stop(void* pParam)
  { CCUNUSED(pParam); stop(); }

  //! @return Name of thread, set on constructor.
  inline const CcString& getName() const
  { return m_sName;}

  /**
   * @brief Wait for thread until specific state was reached.
   * @param State:    Target set to wait for
   * @param oTimeout: Maximum number of time to wait or 0 for infinite wait
   * @return Success or EStatus::TimeoutReached on timeout
   */
  CcStatus waitForState(EThreadState State, const CcDateTime& oTimeout=0);

  /**
   * @brief Wait for thread until running state was reached.
   * @param oTimeout: Maximum number of time to wait or 0 for infinite wait
   * @return Success or EStatus::TimeoutReached on timeout
   */
  CcStatus waitForRunning(const CcDateTime& oTimeout=0)
  { return waitForState(EThreadState::Running, oTimeout); }

  /**
   * @brief Wait for thread until stopped state was reached.
   * @param oTimeout: Maximum number of time to wait or 0 for infinite wait
   * @return Success or EStatus::TimeoutReached on timeout
   */
  CcStatus waitForExit(const CcDateTime& oTimeout=0)
  { return waitForState(EThreadState::Stopped, oTimeout); }

  /**
   * @brief Get actual State of Thread
   * @return State value
   */
  EThreadState getThreadState()
  { return m_State; }

  /**
   * @brief Check if thread is in a not stopped stated
   * @return true if process not stopped
   */
  bool isInProgress()
  { return m_State != EThreadState::Stopped; }

  //! @return True if Thread is in running state
  bool isRunning()
  { return getThreadState() == EThreadState::Running; }

  /**
   * @brief Exit Code of application can updated from external and internal.
   * @param iExitCode: new exit code. Preferd values should come from EStatus
   */
  inline void setExitCode(CcStatus iExitCode)
  { m_oExitCode = iExitCode; }

  /**
   * @brief Get Exit Code wich is currently stored in application.
   * @return int32
   */
  const CcStatus& getExitCode() const
  { return m_oExitCode; }

  /**
   * @brief Define the stack size of thread by overloading this Method.
   *        Default value is 0 wich will create a stack with default system stack size
   * @return Defined stacksize for this thread.
   */
  virtual size_t getStackSize()
  { return 0; }

  void registerOnStateChange(const CcEvent& eEventHandle)
  { m_oStateHandler.append(eEventHandle); }
  void deregisterOnStateChange(CcObject* pObject)
  { m_oStateHandler.removeObject(pObject); }

protected:
  /**
   * @brief Signal to Thread next State;
   * @param State: State to set
   */
  CcStatus enterState(EThreadState State);

  /**
   * @brief Update name of thread if it was changed while running.
   *        It may be that systems can not change it at runtime and it may be only
   *        internaly changed.
   * @param oNewName: New name of thread.
   */
  inline void setName(const CcString& oNewName)
  { m_sName = oNewName;}

private:
  /**
   * @brief Set state value and emit on change event
   * @param State: State to set
   */
  void setState(EThreadState State)
  { m_State = State; m_oStateHandler.call(this); }

private:
  CcString m_sName;               //!< Name of this thread
  EThreadState m_State;           //!< Current thread state
  CcMutex m_oStateLock;           //!< Keep state changes thread safe with this lock
  CcStatus m_oExitCode = 0;       //!< Exit code wich will be returned if application ends
  CcEventHandler m_oStateHandler; //!< State change notification hanlder
};
