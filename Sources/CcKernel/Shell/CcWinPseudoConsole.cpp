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
#include <windows.h>
#include <stdio.h>

CcWinPseudoConsole::~CcWinPseudoConsole()
{
  close();
}

size_t CcWinPseudoConsole::read(void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  DWORD uiReadSize = 0;
  if (ReadFile(hStdout, pBuffer, uSize, &uiReadSize, nullptr))
  {
    uiRet = uiReadSize;
  }
  return uiRet;
}

size_t CcWinPseudoConsole::readHidden(void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  DWORD uiReadSize = 0;
  if (WriteFile(hStdin, pBuffer, uSize, &uiReadSize, nullptr))
  {
    uiRet = uiReadSize;
  }
  return uiRet;
}

size_t CcWinPseudoConsole::write(const void* pBuffer, size_t uSize)
{
  return CcConsole::write(pBuffer, uSize);
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

      PROCESS_INFORMATION pi;
      memset(&pi, 0, sizeof(pi));

      // Call CreateProcess
      if (!CreateProcessW(NULL, m_sCommandLine.getLPWSTR(), NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &si.StartupInfo, &pi))
      {
        oRet = false;
      }
      else
      {
        oRet = true;
      }
    }
  }

  return oRet;
}

CcStatus CcWinPseudoConsole::close()
{
  CcStatus oRet = false;
  // Enable the window and mouse input events.
  if (hStdin != INVALID_HANDLE_VALUE)
  {
    if (!SetConsoleMode(hStdin, fdwSaveOldMode))
    {
      CCDEBUG("SetConsoleMode");
    }
    else
    {
      oRet = true;
    }
  }
  return oRet;
}

CcStatus CcWinPseudoConsole::cancel()
{
  CcStatus oRet = EStatus::AllOk;
  bCancel = true;
  INPUT_RECORD ir[2];
  for (int i = 0; i < 2; i++)
  {
    KEY_EVENT_RECORD *kev = &ir[i].Event.KeyEvent;
    ir[i].EventType = KEY_EVENT;
    kev->bKeyDown = i == 0;    //<-true, than false
    kev->dwControlKeyState = 0;
    kev->wRepeatCount = 1;
    kev->uChar.UnicodeChar = VK_RETURN;
    kev->wVirtualKeyCode = VK_RETURN;
    kev->wVirtualScanCode = static_cast<WORD>(MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC));
  }
  DWORD dw;
  WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ir, 2, &dw);
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

CcStatus CcWinPseudoConsole::CreateConsole()
{
  CcStatus oStatus = false;
  HANDLE inRead = NULL;
  HANDLE outWrite = NULL;

  if (!CreatePipe(&inRead, &hStdin, NULL, 0))
  {
    if (inRead) CloseHandle(inRead);
    if (hStdout) CloseHandle(hStdin);
  }
  else
  {
    if (!CreatePipe(&hStdout, &outWrite, NULL, 0))
    {
      if (inRead) CloseHandle(inRead);
      if (hStdout) CloseHandle(hStdout);
      if (hStdin) CloseHandle(hStdin);
      if (outWrite) CloseHandle(outWrite);
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
    return FALSE;
  }

  // Initialize the list memory location
  if (!InitializeProcThreadAttributeList(psi->lpAttributeList, 1, 0, &bytesRequired))
  {
    HeapFree(GetProcessHeap(), 0, psi->lpAttributeList);
    return FALSE;
  }

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
    return FALSE;
  }

  return oStatus;
}
