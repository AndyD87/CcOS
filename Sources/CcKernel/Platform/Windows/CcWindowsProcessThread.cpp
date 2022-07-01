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
 * @brief     Implemtation of class CcWindowsProcessThread
 */
#include "CcWindowsProcessThread.h"
#include "CcWindowsPipe.h"
#include "CcWString.h"
#include "CcKernel.h"

class CcWindowsProcessThread::CPrivate
{
public:
  PROCESS_INFORMATION pi;
};

CcWindowsProcessThread::CcWindowsProcessThread(CcProcess& rProcess) :
  m_hProcess(&rProcess)
{
  CCNEW(m_pPrivate, CPrivate);
}

CcWindowsProcessThread::~CcWindowsProcessThread()
{
  kill();
  CCDELETE(m_pPrivate);
}

void CcWindowsProcessThread::run()
{
  CcString commandline;
  commandline << "\"" << m_hProcess->getApplication().getOsPath() << "\" ";
  commandline << m_hProcess->getArguments().collapse("", " \"", "\"");
  STARTUPINFOW si;
  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&m_pPrivate->pi, sizeof(m_pPrivate->pi));
  m_pPrivate->pi.hProcess = INVALID_HANDLE_VALUE;
  m_pPrivate->pi.hThread = INVALID_HANDLE_VALUE;
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags |= STARTF_USESTDHANDLES;
  si.hStdInput  = static_cast<CcWindowsPipe&>(m_hProcess->pipe()).m_HandleIn;
  si.hStdOutput = static_cast<CcWindowsPipe&>(m_hProcess->pipe()).m_HandleOut;
  si.hStdError  = static_cast<CcWindowsPipe&>(m_hProcess->pipe()).m_HandleOut;
  CcWString wsWorkingDir = m_hProcess->getWorkingDirectory();
  CcWString wsCommand = commandline.getWString();
  const wchar_t* pWDir = nullptr;
  if(wsWorkingDir.length() > 0)
    pWDir = wsWorkingDir.getWcharString();
  // Start the child process.
  if (!CreateProcessW(nullptr,   // No module name (use command line)
    wsCommand.getLPWSTR(),        // Command line
    nullptr,           // Process handle not inheritable
    nullptr,           // Thread handle not inheritable
    TRUE,          // Set handle inheritance to FALSE
    CREATE_NO_WINDOW,              // No creation flags
    nullptr,           // Use parent's environment block
    pWDir,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &m_pPrivate->pi)           // Pointer to PROCESS_INFORMATION structure
    )
  {
    CcStatus oStatus;
    oStatus.setSystemError(GetLastError());
    setExitCode(oStatus);
    m_hProcess->setExitCode(oStatus);
    CCDEBUG("CreateProcess failed: " + CcString::fromNumber(oStatus.getErrorUint(), 16));
  }
  else
  {
    m_bProcessStarted = true;
    // Wait until child process exits.
    while (WAIT_TIMEOUT == WaitForSingleObject(m_pPrivate->pi.hProcess, 100))
    {
      //static_cast<CcWindowsPipe&>(m_hProcess->pipe()).readCache();
    }
    DWORD uiExitCode;
    GetExitCodeProcess(m_pPrivate->pi.hProcess, &uiExitCode);

    static_cast<CcWindowsPipe&>(m_hProcess->pipe()).readCache();
    m_hProcess->pipe().close();

    // Close process and thread handles.
    if (m_pPrivate->pi.hProcess != INVALID_HANDLE_VALUE)
    {
      CloseHandle(m_pPrivate->pi.hProcess);
      m_pPrivate->pi.hProcess = INVALID_HANDLE_VALUE;
    }
    if (m_pPrivate->pi.hThread != INVALID_HANDLE_VALUE)
    {
      CloseHandle(m_pPrivate->pi.hThread);
      m_pPrivate->pi.hThread = INVALID_HANDLE_VALUE;
    }

    setExitCode(uiExitCode);
    CCDEBUG("ExitCode: " + CcString::fromNumber(uiExitCode));
    m_hProcess->setExitCode(uiExitCode);

    m_bProcessStarted = false;
  }
  CCDEBUG("Process finished");
}

void CcWindowsProcessThread::kill()
{
  if (m_pPrivate->pi.hProcess != INVALID_HANDLE_VALUE)
  {
    if (TerminateProcess(m_pPrivate->pi.hProcess, getExitCode()))
    {
      size_t uiTimeout = 100;
      while (m_bProcessStarted && uiTimeout-- > 0 && CcKernel::sleep(10));
    }
  }
}

void CcWindowsProcessThread::term()
{
  kill();
}
