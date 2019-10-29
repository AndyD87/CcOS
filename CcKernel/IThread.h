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
 * @subpage   IThread
 *
 * @page      IThread
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IThread
 */
#ifndef H_CCTHREADOBJECT_H_
#define H_CCTHREADOBJECT_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcObject.h"
#include "CcString.h"
#include "CcDateTime.h"
#include "CcMutex.h"
#include "CcEventActionList.h"

class CcKernel;

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
class CcKernelSHARED IThread  : public CcObject
{
public:
  IThread(const CcString& sName = "");
  /**
   * @brief Destroy Object and waiting until @ref getThreadState is set to @ref Stopped
   */
  virtual ~IThread();

  /**
   * @brief Virtual function for Running-Code
   *        Must be implemented by target application.
   */
  virtual void run  () = 0;

  virtual void onStop()
  {
  }

  virtual void onStopped()
  {
  }
  
  bool isRunning()
    { return getThreadState() == EThreadState::Running; }

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
    {enterState(EThreadState::Stopping);}

  /**
   * @brief Stop Event receiver
   * @param pParam: not in use
   * @return send stop to thread
   */
  void stop(void* pParam)
    { CCUNUSED(pParam); stop(); }

  inline const CcString& getName() const
    { return m_sName;}
  

  /**
   * @brief Signal to Thread next State;
   * @param State: State to set
   */
  CcStatus enterState(EThreadState State);

  CcStatus waitForState(EThreadState State, const CcDateTime& oTimeout=0);
  CcStatus waitForRunning(const CcDateTime& oTimeout=0)
    {return waitForState(EThreadState::Running, oTimeout);}
  CcStatus waitForExit(const CcDateTime& oTimeout=0)
    {return waitForState(EThreadState::Stopped, oTimeout);}

  /**
   * @brief Get actual State of Thread
   * @return State value
   */
  EThreadState getThreadState()
    {return m_State;}

  /**
   * @brief Check if thread is in a not stopped stated
   * @return true if process not stopped
   */
  bool isInProgress()
    {return m_State != EThreadState::Stopped; }
  
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
  CcStatus getExitCode() const
    { return m_oExitCode; }

  /**
   * @brief Define the stack size of thread by overloading this Method.
   *        Default value is 0 wich will create a stack with default system stack size
   * @return Defined stacksize for this thread.
   */
  virtual size_t getStackSize()
    { return 0; }

protected:
  
  inline void setName(const CcString& oNewName)
    { m_sName = oNewName;}
private:
  CcString m_sName;             //!< Name of this thread
  EThreadState m_State;         //!< Current thread state
  CcMutex m_oStateLock;         //!< Keep state changes thread safe with this lock
  CcStatus m_oExitCode = 0;     //!< Exit code wich will be returned if application ends
};

#endif // H_CcTHREADOBJECT_H_
