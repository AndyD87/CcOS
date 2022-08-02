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
 * @brief     Class CcWinPseudoConsole
 */

#include "IIo.h"
#include "CcWinPseudoConsole.h"
#include "CcConsole.h"
#include "CcString.h"
#include "CcWString.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"
#include "CcByteArray.h"
#include "IThread.h"
#include <stdio.h>

class IWinPseudoConsolePassthroughThread : public IThread
{
public:
  IWinPseudoConsolePassthroughThread(CcWinPseudoConsole* pProcess, IIo* pStream) :
    m_pProcess(pProcess),
    m_pStream(pStream)
  {
  }

  virtual void run() override
  {
    CcByteArray oReadBuffer;
    oReadBuffer.resize(1024);
    size_t uiRead;
    while (m_pStream && m_pProcess)// m_pProcess->getCurrentState() != EThreadState::Stopped)
    {
      uiRead = m_pProcess->readArray(oReadBuffer, false);
      if (uiRead && m_pStream)
        m_pStream->writeArray(oReadBuffer, uiRead);
    }
    m_pStream = nullptr;
    m_pProcess = nullptr;
  }

  virtual void onStop() override
  {
    m_pStream = nullptr;
    if (m_pProcess)
    {
      m_pProcess->cancel();
      m_pProcess = nullptr;
    }
  }

  CcWinPseudoConsole*  m_pProcess;
  IIo*        m_pStream;
};

CcWinPseudoConsole::~CcWinPseudoConsole()
{
  close();
  ClosePseudoConsole(hConsole);
  if (pi.hProcess) CloseHandle(pi.hProcess);
}

size_t CcWinPseudoConsole::read(void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  DWORD uiReadSize = 0;
  if (ReadFile(hStdout, pBuffer, static_cast<DWORD>(uSize), &uiReadSize, nullptr))
  {
    uiRet = uiReadSize;
  }
  return uiRet;
}

size_t CcWinPseudoConsole::readHidden(void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  DWORD uiReadSize = 0;
  if (ReadFile(hStdout, pBuffer, static_cast<DWORD>(uSize), &uiReadSize, nullptr))
  {
    uiRet = uiReadSize;
  }
  return uiRet;
}

size_t CcWinPseudoConsole::write(const void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  DWORD uiReadSize = 0;
  if (WriteFile(hStdin, pBuffer, static_cast<DWORD>(uSize), &uiReadSize, nullptr))
  {
    uiRet = uiReadSize;
  }
  return uiRet;
}

CcStatus CcWinPseudoConsole::open(EOpenFlags)
{
  CcStatus oRet = false;
  // Get the standard input handle.

  if (!CreateConsole())
  {
    printf("Can't create console: %u\n", GetLastError());
  }
  else
  {
    STARTUPINFOEX si;
    if (!PrepareStartupInformation(&si))
    {
      printf("Can't prepare startup info: %u\n", GetLastError());
    }
    else
    {
      memset(&pi, 0, sizeof(pi));

      // Call CreateProcess
      m_sCommandLine.replace(CcGlobalStrings::Seperators::Path, CcGlobalStrings::Seperators::BackSlash);
      if (!CreateProcessW(NULL, m_sCommandLine.getLPWSTR(), NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &si.StartupInfo, &pi))
      {
        oRet = false;
      }
      else
      {
        CloseHandle(pi.hThread);
        oRet = true;
        CCNEW(pThread, IWinPseudoConsolePassthroughThread, this, pIoStream);
        pThread->start();
      }
    }
  }

  return oRet;
}

CcStatus CcWinPseudoConsole::close()
{
  CcStatus oRet = false;
  hConsole = nullptr;
  if (pThread)
  {
    pThread->stop();
    CCDELETE(pThread);
  }
  CloseHandle(inRead);
  CloseHandle(outWrite);
  inRead = nullptr;
  outWrite = nullptr;
  return oRet;
}

CcStatus CcWinPseudoConsole::cancel()
{
  CcStatus oRet = EStatus::AllOk;
  if (!WriteFile(outWrite, "\n", 1, nullptr, nullptr))
  {
    DWORD dwError = GetLastError();
    CCDEBUG("cancel failed with " + CcString::fromNumber(dwError));
  }
  if (hStdout) CloseHandle(hStdout);
  return oRet;
}

CcStatus CcWinPseudoConsole::ioControl(uint32 , const void *, size_t , void *, size_t , size_t* )
{
  CcStatus oRet = EStatus::NotSupported;
  return oRet;
}

void* CcWinPseudoConsole::getStdFile()
{
  return hStdin;
}

CcStatus CcWinPseudoConsole::flush()
{
  CcStatus oRet = EStatus::NotSupported;
  return oRet;
}

CcStatus CcWinPseudoConsole::check()
{
  CcStatus oRet = EStatus::AlreadyStopped;
  if (WAIT_TIMEOUT == WaitForSingleObject(pi.hProcess, 100))
  {
    oRet = EStatus::AllOk;
  }
  return oRet;
}

CcStatus CcWinPseudoConsole::CreateConsole()
{
  CcStatus oStatus = false;
  if (!CreatePipe(&inRead, &hStdin, NULL, 0))
  {
    if (inRead) CloseHandle(inRead);
    if (hStdout) CloseHandle(hStdin);
    inRead = nullptr;
    hStdin = nullptr;
  }
  else
  {
    if (!CreatePipe(&hStdout, &outWrite, NULL, 0))
    {
      if (inRead) CloseHandle(inRead);
      if (hStdout) CloseHandle(hStdout);
      if (hStdin) CloseHandle(hStdin);
      if (outWrite) CloseHandle(outWrite);
      inRead = nullptr;
      hStdin = nullptr;
      hStdout = nullptr;
      outWrite = nullptr;
    }
    else
    {
      COORD size = {80, INT16_MAX};
      HRESULT hr = CreatePseudoConsole(size, inRead, outWrite, 0, &hConsole);
      if (FAILED(hr))
      {
        if (inRead) CloseHandle(inRead);
        if (hStdout) CloseHandle(hStdout);
        if (hStdin) CloseHandle(hStdin);
        if (outWrite) CloseHandle(outWrite);
        inRead = nullptr;
        hStdin = nullptr;
        hStdout = nullptr;
        outWrite = nullptr;
      }
      else
      {
        oStatus = true;
      }
    }
  }
  return oStatus;
}

CcStatus CcWinPseudoConsole::PrepareStartupInformation(void* pvsi)
{
  CcStatus oStatus = false;

  STARTUPINFOEX* psi = static_cast<STARTUPINFOEX*>(pvsi);
  memset(psi, 0, sizeof(*psi));
  psi->StartupInfo.cb = sizeof(STARTUPINFOEX);

  // Discover the size required for the list
  SIZE_T bytesRequired;
  InitializeProcThreadAttributeList(NULL, 1, 0, &bytesRequired);

  // Allocate memory to represent the list
  psi->lpAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST) HeapAlloc(GetProcessHeap(), 0, bytesRequired);
  if (!psi->lpAttributeList)
  {
  }
  else
  {
    // Initialize the list memory location
    if (!InitializeProcThreadAttributeList(psi->lpAttributeList, 1, 0, &bytesRequired))
    {
      HeapFree(GetProcessHeap(), 0, psi->lpAttributeList);
    }
    else
    {
      // Set the pseudoconsole information into the list
      if (!UpdateProcThreadAttribute(psi->lpAttributeList,
        0,
        PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
        hConsole,
        sizeof(hConsole),
        NULL,
        NULL))
      {
        HeapFree(GetProcessHeap(), 0, psi->lpAttributeList);
      }
      else
      {
        oStatus = EStatus::AllOk;
      }
    }
  }
  return oStatus;
}
