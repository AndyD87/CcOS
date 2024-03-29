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
 * @brief     Implemtation of class CcLinuxProcessThread
 */
#include "CcLinuxProcessThread.h"
#include "CcByteArray.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include "CcLinuxPipe.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"

CcLinuxProcessThread::CcLinuxProcessThread(CcProcess& rProcess) :
  m_hProcess(&rProcess)
{
}

CcLinuxProcessThread::~CcLinuxProcessThread()
{
  kill();
}

void CcLinuxProcessThread::run()
{
  static_cast<CcLinuxPipe&>(m_hProcess->pipe()).init();
  m_iChildId = fork();
  if(m_iChildId == 0)
  {
    static_cast<CcLinuxPipe&>(m_hProcess->pipe()).setupChild();

    CcKernel::setEnvironmentVariable(CcGlobalStrings::EnvVars::AppNoIoBuffering, CcGlobalStrings::True);

    CCNEWARRAYTYPE(pArgv, char*, m_hProcess->getArguments().size()+2);
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
    execvp(pArgv[0], pArgv);
    CCDELETEARR(pArgv);
    static_cast<CcLinuxPipe&>(m_hProcess->pipe()).closeChild();
    exit(errno);
  }
  else if(m_iChildId > 0)
  {
    static_cast<CcLinuxPipe&>(m_hProcess->pipe()).setupParent();
    m_bProcessStarted = true;
    int iStatus;

    waitpid(m_iChildId, &iStatus, 0);
    static_cast<CcLinuxPipe&>(m_hProcess->pipe()).closeParent();
    m_iChildId = -1;
    iStatus = WEXITSTATUS(iStatus);
    setExitCode(iStatus);
    m_hProcess->setExitCode(iStatus);
    m_bProcessStarted = false;
  }
}

void CcLinuxProcessThread::onStop()
{
  term();
}

void CcLinuxProcessThread::kill()
{
  if(m_iChildId != -1)
  {
    if( 0 == ::kill(m_iChildId, SIGKILL))
    {
      size_t uiTimeout = 100;
      while (m_bProcessStarted && uiTimeout-- > 0)
      {
        CcKernel::sleep(10);
      }
    }
  }
}

void CcLinuxProcessThread::term()
{
  if(m_iChildId != -1)
  {
    if( 0 == ::kill(m_iChildId, SIGKILL))
    {
      size_t uiTimeout = 100;
      while (m_bProcessStarted && uiTimeout-- > 0)
      {
        CcKernel::sleep(10);
      }
    }
  }
}
