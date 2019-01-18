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
 * @subpage   CcProcess
 *
 * @page      CcProcess
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcProcess
 */
#ifndef _CcProcess_H_
#define _CcProcess_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcDateTime.h"
#include "CcThreadObject.h"
#include "CcGlobalStrings.h"

class CcIODevice;
class CcProcessPrivate;

class CcProcessThread : public CcThreadObject
{
public:
  CcProcessThread(const CcString& sProcesName = CcGlobalStrings::Names::CcProcessThreadName):
    CcThreadObject(sProcesName)
    {}
  virtual ~CcProcessThread(){}

  bool isProcessStarted() const
    {return m_bProcessStarted;}
protected:
  bool m_bProcessStarted = false;
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
  CcProcess(void );

  /**
   * @brief Constructor
   */
  CcProcess(const CcString& sApplication);

  /**
   * @brief Destructor
   */
  virtual ~CcProcess(void );

  void start(void);
  void stop(void);
  CcStatus exec(const CcDateTime& oTimeout = 0)
    {start(); return waitForExit(oTimeout);  }

  CcStatus waitForState(EThreadState State, const CcDateTime& oTimeout=0);
  CcStatus waitForRunning(const CcDateTime& oTimeout=0)
    {return waitForState(EThreadState::Running, oTimeout);}
  CcStatus waitForExit(const CcDateTime& oTimeout = 0);

  void setApplication(const CcString& sApplication);
  void setArguments(const CcString& sArguments);
  void setArguments(const CcStringList& slArguments);
  void setPipe(CcIODevice* pInput);
  void addArgument(const CcString& sArgument);
  void clearArguments(void);

  void setThreadHandle(CcProcessThread *pThreadHandle);
  void setExitCode(const CcStatus& uiExitCode)
    { m_oExitCode = uiExitCode; }
  const CcStatus& getExitCode()
    { return m_oExitCode; }
  void setWorkingDirectory(const CcString& sDir)
    { m_sWorkingDir = sDir; }

  CcString& getApplication(void);
  const CcString& getApplication(void) const;
  const CcStringList& getArguments(void) const;
  CcStringList& getArguments(void);
  const CcString& getWorkingDirectory(void)
    { return m_sWorkingDir; }
  CcIODevice& pipe();
  bool hasExited();

private:
  CcProcessPrivate* m_pPrivate = nullptr;
  CcString m_sApplication;
  CcStringList m_Arguments;
  CcString m_sWorkingDir;
  CcStatus m_oExitCode = 0;
};

#endif /* _CcProcess_H_ */
