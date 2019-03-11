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
#ifndef _CCTHREADOBJECT_H_
#define _CCTHREADOBJECT_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcObject.h"
#include "CcString.h"
#include "CcDateTime.h"

class CcKernel;

enum class CcKernelSHARED EThreadState
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
  IThread();
  IThread(const CcString& sName);
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
  
  /**
   * @brief Virtual function for Startup-Code
   *        Can be implemnted by inhering Class.
   *        Gets called before thread is starting.
   */
  void start();
  
  /**
   * @brief Call this method if this Thread should get started without creating a new thread
   * @return void
   */
  void startOnCurrent();

  /**
   * @brief Send stop command to thread
   */
  void stop();

  /**
   * @brief Stop Event receiver
   * @param pParam: not in use
   * @return send stop to thread
   */
  void stop(void* pParam)
    { CCUNUSED(pParam); stop(); }

  inline const CcString& getName()
    { return m_sName;}
  

  /**
   * @brief Signal to Thread next State;
   * @param State: State to set
   */
  void enterState(EThreadState State);

  CcStatus waitForState(EThreadState State, const CcDateTime& oTimeout=0);
  CcStatus waitForRunning(const CcDateTime& oTimeout=0)
    {return waitForState(EThreadState::Running, oTimeout);}
  CcStatus waitForExit(const CcDateTime& oTimeout=0)
  {return waitForState(EThreadState::Stopped, oTimeout);}

  /**
   * @brief Get actual State of Thread
   * @return State value
   */
  EThreadState getThreadState();

  /**
   * @brief Check if thread is in a not stopped stated
   * @return true if process not stopped
   */
  bool isInProgress()
    { return m_State != EThreadState::Stopped; }
  
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

protected:
  
  inline void setName(const CcString& oNewName)
    { m_sName = oNewName;}
private:
  CcString m_sName;       //!< Name of this thread
  EThreadState m_State;   //!< Current thread state
  CcStatus m_oExitCode = 0;  //!< Exit code wich will be returned if application ends
};

#endif /* _CCTHREADOBJECT_H_ */
