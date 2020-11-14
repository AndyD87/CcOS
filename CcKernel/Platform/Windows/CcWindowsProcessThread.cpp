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
 * @brief     Implemtation of class CcWindowsProcessThread
 */
#include "CcWindowsProcessThread.h"
#include "CcWindowsGlobals.h"
#include "CcWindowsPipe.h"
#include "CcWString.h"

class CcWindowsProcessThread::CPrivate
{
public:
  HANDLE hProcess = INVALID_HANDLE_VALUE;
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
  CcString commandline("\"" + m_hProcess->getApplication() + "\" " + m_hProcess->getArguments().collapse(" \"", "\""));
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&pi, sizeof(pi));
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
    &pi)           // Pointer to PROCESS_INFORMATION structure
    )
  {
    CCDEBUG("CreateProcess failed: " + CcString::fromNumber(GetLastError()));
  }
  else
  {
    m_bProcessStarted = true;
    if (m_pPrivate != nullptr)
    {
      m_pPrivate->hProcess = pi.hProcess;
    }
    // Wait until child process exits.
    while (WAIT_TIMEOUT == WaitForSingleObject(pi.hProcess, 100))
    {
      static_cast<CcWindowsPipe&>(m_hProcess->pipe()).readCache();
    }
    m_bProcessStarted = false;
    // Check if m_pPrivate was deleted in desonstructor
    if (m_pPrivate != nullptr)
    {
      m_pPrivate->hProcess = INVALID_HANDLE_VALUE;
    }
    DWORD uiExitCode;
    GetExitCodeProcess(pi.hProcess, &uiExitCode);
    this->setExitCode(uiExitCode);
    m_hProcess->setExitCode(uiExitCode);
    CloseHandle(si.hStdInput);
    CloseHandle(si.hStdOutput);
    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    static_cast<CcWindowsPipe&>(m_hProcess->pipe()).m_HandleIn = INVALID_HANDLE_VALUE;
    static_cast<CcWindowsPipe&>(m_hProcess->pipe()).m_HandleOut = INVALID_HANDLE_VALUE;
  }
}

void CcWindowsProcessThread::kill()
{
  if (m_pPrivate->hProcess != INVALID_HANDLE_VALUE)
  {
    if (TerminateProcess(m_pPrivate->hProcess, getExitCode()))
    {
      m_pPrivate->hProcess = INVALID_HANDLE_VALUE;
    }
  }
}

void CcWindowsProcessThread::term()
{
  if (m_pPrivate->hProcess != INVALID_HANDLE_VALUE)
  {
    if (TerminateProcess(m_pPrivate->hProcess, getExitCode()))
    {
      m_pPrivate->hProcess = INVALID_HANDLE_VALUE;
    }
  }
}
