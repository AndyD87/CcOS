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
 * @brief     Class CcProcess
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcDateTime.h"
#include "IThread.h"
#include "CcGlobalStrings.h"

class IIo;

/**
 * @brief Processes object with seperate thread for start and stop.
 */
class CcProcessThread : public IThread
{
public:
  /**
   * @brief CcProcessThread
   * @param sProcesName
   */
  CcProcessThread(const CcString& sProcesName = CcGlobalStrings::Names::CcProcessThreadName):
    IThread(sProcesName)
  {}
  virtual ~CcProcessThread() = default;

  //! @return True if process thread was started from system
  bool isProcessStarted() const
  { return m_bProcessStarted; }

protected:
  bool m_bProcessStarted = false; //!< Internal process start flag to change on run from system
};

/**
 * @brief Process-Connection to an external Process
 *        It can start stop them or @todo in future attach to one.
 */
class CcKernelSHARED CcProcess
{
public:
  /**
   * @brief Constructor
   */
  CcProcess();

  /**
   * @brief Constructor
   */
  CcProcess(const CcString& sApplication);

  /**
   * @brief Destructor
   */
  virtual ~CcProcess();

  /**
   * @brief Start process with already set configurations for Executable and Arguments
   * @return Status of startup
   */
  CcStatus start();

  /**
   * @brief Stop current process from execution.
   */
  void stop();

  /**
   * @brief Execute and wait until done or optional timeout is reached.
   * @param oTimeout: Target timeout or 0 if infinite waiting.
   * @return Return value from exection or EStatus::TimeoutReached
   */
  CcStatus exec(const CcDateTime& oTimeout = 0);

  /**
   * @brief Wait for specific state for process.
   * @param State:    State to wait for.
   * @param oTimeout: Target timeout or 0 if infinite waiting.
   * @return Success or EStatus::TimeoutReached if timeout reached
   */
  CcStatus waitForState(EThreadState State, const CcDateTime& oTimeout=0);

  /**
   * @brief Wait for process is started.
   * @param oTimeout: Maximum target timeout or 0 if infinite waiting.
   * @return Success or EStatus::TimeoutReached if timeout reached
   */
  CcStatus waitForStarted(const CcDateTime& oTimeout=0);

  /**
   * @brief Wait for process is in running state.
   * @param oTimeout: Maximum target timeout or 0 if infinite waiting.
   * @return Success or EStatus::TimeoutReached if timeout reached
   */
  CcStatus waitForRunning(const CcDateTime& oTimeout=0)
  { return waitForState(EThreadState::Running, oTimeout); }

  /**
   * @brief Wait for process is stopped.
   * @param oTimeout: Maximum target timeout or 0 if infinite waiting.
   * @return Success or EStatus::TimeoutReached if timeout reached
   */
  CcStatus waitForExit(const CcDateTime& oTimeout = 0);

  //! @param sApplication: Target executable to start
  void setApplication(const CcString& sApplication);
  //! @param sArguments: Set arguments as space seperated line.
  //!                    They will be pares to list of arguments
  void setArguments(const CcString& sArguments);
  //! @param slArguments: Set arguments as array of arguments.
  //!                     Arguments with containing spaces will be surrounded
  //!                     with quotes before start.
  void setArguments(const CcStringList& slArguments);
  //! @param pInput: Set target pipe for input and output of target application
  void setPipe(IIo* pInput);
  //! @param sArgument: Add an additional argument to already available.
  void addArgument(const CcString& sArgument);

  /**
   * @brief Clear current stored argument list.
   */
  void clearArguments();

  //! @param pThreadHandle: Set thread handle of started process to manage in system.
  //!                        Thread handle was created from system on process start.
  void setThreadHandle(CcProcessThread *pThreadHandle);

  //! @param uiExitCode: Exit code to set from system after finished execution
  void setExitCode(const CcStatus& uiExitCode)
  { m_oExitCode = uiExitCode; }
  //! @param sDir:  Set working directory for application to start at.
  void setWorkingDirectory(const CcString& sDir)
  { m_sWorkingDir = sDir; }

  //! @return Return value from application after execution finished.
  const CcStatus& getExitCode()
  { return m_oExitCode; }
  //! @return Get path of application to start
  CcString& getApplication();
  //! @return Get path of application to start
  const CcString& getApplication() const;
  //! @return Get current list of arguments currently set
  const CcStringList& getArguments() const;
  //! @return Get current list of arguments currently set
  CcStringList& getArguments();
  //! @return Get current working directory
  const CcString& getWorkingDirectory()
  { return m_sWorkingDir; }
  //! @return Pipe to read from or write to for interacting with process.
  IIo& pipe();
  //! @return True if application has finished.
  bool hasExited();

  /**
   * @brief Start application directly with different option.
   * @param sExecutable:  Executable to start
   * @param oParams:      Arguments to pass to executable
   * @param sWorkingDir:  Working directory to start application in
   * @param bDoWait:      If true, wait for application finished.
   *                      Otherwise, application will be started in background.
   * @param oTimeout:     If bDoWait is true, a timeout can be specified to maximal wait for.
   * @param pExecutable:  Optional capture output from application.
   * @return If waiting, return code of application.
   *         If not waiting, status of application start.
   */
  static CcStatus exec(const CcString& sExecutable,
                       const CcStringList& oParams = CcStringList(),
                       const CcString &sWorkingDir = CcGlobalStrings::Empty,
                       bool bDoWait = true,
                       const CcDateTime& oTimeout = 0,
                       CcString* pExecutable = nullptr
  );


private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
  CcString m_sApplication;
  CcStringList m_Arguments;
  CcString m_sWorkingDir;
  CcStatus m_oExitCode = 0;
};
