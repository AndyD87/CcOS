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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcLinuxProcessThread
 */
#include "CcLinuxProcessThread.h"
#include "CcByteArray.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "CcLinuxPipe.h"

CcLinuxProcessThread::CcLinuxProcessThread(CcProcess& rProcess) :
  m_hProcess(&rProcess)
{
}

CcLinuxProcessThread::~CcLinuxProcessThread(void )
{
}

void CcLinuxProcessThread::run()
{
  pid_t uiResult = fork();
  if(uiResult == 0)
  {
    dup2(static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[1], STDOUT_FILENO);
    close(static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[0]);
    close(static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[1]);
    char** pArgv = new char*[m_hProcess->getArguments().size()+2];
    pArgv[0] = m_hProcess->getApplication().getCharString();
    pArgv[m_hProcess->getArguments().size() +1] = nullptr;
    size_t i = 1;
    for(CcString& sArgument : m_hProcess->getArguments())
    {
      pArgv[i] = sArgument.getCharString();
      i++;
    }
    // Change Working directory if set
    if(m_hProcess->getWorkingDirectory().length() > 0)
    {
      if(0 != chdir(m_hProcess->getWorkingDirectory().getCharString()))
      {
        CCDEBUG("Failed to change directory for process call");
      }
    }
    // Execute Process
    int iResult = execvp(pArgv[0], pArgv);
    if(iResult != 0)
    {
      CCDEBUG("Failed to create process");
    }
  }
  else if(uiResult > 0)
  {
    int iStatus;
    waitpid(uiResult, &iStatus, 0);
    close(static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[0]);
    close(static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[1]);
    static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[0] = -1;
    static_cast<CcLinuxPipe&>(m_hProcess->pipe()).getPipes()[1] = -1;
    m_hProcess->setExitCode(static_cast<uint32>(iStatus));
  }
}
