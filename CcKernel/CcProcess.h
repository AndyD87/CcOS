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

class CcIODevice;
class CcThreadObject;
class CcProcessPrivate;

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
  void waitForExit();

  void setApplication(const CcString& sApplication);
  void setArguments(const CcString& sArguments);
  void setArguments(const CcStringList& slArguments);
  void setPipe(CcIODevice* pInput);
  void addArgument(const CcString& sArgument);
  void clearArguments(void);

  void setThreadHandle(CcThreadObject* pThreadHandle);
  void setExitCode(uint32 uiExitCode)
    { m_uiExitCode = uiExitCode; }
  uint32 getExitCode()
    { return m_uiExitCode; }
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
  uint32 m_uiExitCode = 0;
};

#endif /* _CcProcess_H_ */
