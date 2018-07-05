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
 * @subpage   CcThreadObject
 *
 * @page      CcThreadObject
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcThreadObject
 */
#ifndef _CCTHREADOBJECT_H_
#define _CCTHREADOBJECT_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcObject.h"
#include "CcString.h"

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
class CcKernelSHARED CcThreadObject  : public CcObject
{
public:
  CcThreadObject() : m_State(EThreadState::Stopped)
  {
  }

  CcThreadObject(const CcString& sName) : m_sName(sName), m_State(EThreadState::Stopped)
  {
  }

  /**
   * @brief Destroy Object and waiting until @ref getThreadState is set to @ref Stopped
   */
  virtual ~CcThreadObject();

  /**
   * @brief Virtual function for Running-Code
   *        Must be implemented by target application.
   */
  virtual void run  (void ) = 0;

  virtual void onStop(void)
  {
  }

  virtual void onStopped(void)
  {
  }
  
  /**
   * @brief Virtual function for Startup-Code
   *        Can be implemnted by inhering Class.
   *        Gets called before thread is starting.
   */
  void start(void );
  
  /**
   * @brief Call this method if this Thread should get started without creating a new thread
   * @return void
   */
  void startOnCurrent(void );

  /**
   * @brief Send stop command to thread
   */
  void stop(void);
  
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

  void waitForExit();

  /**
   * @brief Get actual State of Thread
   * @return State value
   */
  EThreadState getThreadState(void);

  /**
   * @brief Check if thread is in a not stopped stated
   * @return true if process not stopped
   */
  bool isInProgress()
    { return m_State != EThreadState::Stopped; }
protected:
  
  inline void setName(const CcString& oNewName)
    { m_sName = oNewName;}
private:
  CcString m_sName;
  EThreadState m_State;
};

#endif /* _CCTHREADOBJECT_H_ */
