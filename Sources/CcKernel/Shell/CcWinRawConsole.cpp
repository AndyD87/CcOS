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
 * @brief     Class CcWinRawConsole
 */

#include "CcWinRawConsole.h"

#ifdef WINDOWS

#include "IIo.h"
#include "CcConsole.h"
#include "CcString.h"
#include "CcWString.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"
#include "CcKernel.h"
#include <windows.h>
#include <stdio.h>

CcWinRawConsole::CcWinRawConsole()
{
}

CcWinRawConsole::~CcWinRawConsole()
{
  close();
}

size_t CcWinRawConsole::read(void* pBuffer, size_t uSize)
{
  INPUT_RECORD irInBuf;
  bCancel = false;
  size_t uiRet = 0;
  DWORD dw;
  bool bFinishRead = false;
  CcString oStringBuffer;
  DWORD cNumRead = 1;
  while (uiRet < uSize && !bFinishRead && !bCancel)
  {
    if (PeekConsoleInputW(
      hStdin,      // input buffer handle
      &irInBuf,     // buffer to read into
      1,         // size of read buffer
      &cNumRead) && cNumRead)
    {
      // Wait for the events.
      if (!ReadConsoleInputW(
        hStdin,      // input buffer handle
        &irInBuf,     // buffer to read into
        1,         // size of read buffer
        &cNumRead)) // number of records read
      {
        CCDEBUG("ReadConsoleInput");
      }
      else if (!bCancel && cNumRead)
      {
        switch (irInBuf.EventType)
        {
          case KEY_EVENT: // keyboard input
            if (irInBuf.Event.KeyEvent.bKeyDown)
            {
              oStringBuffer.appendWchar(irInBuf.Event.KeyEvent.uChar.UnicodeChar);
            }
            else
            {
              WriteConsoleInputW(GetStdHandle(STD_OUTPUT_HANDLE), &irInBuf, 1, &dw);
            }
            break;

          case MOUSE_EVENT: // mouse input
          case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
          case FOCUS_EVENT:  // disregard focus events
          case MENU_EVENT:   // disregard menu events
            WriteConsoleInputW(GetStdHandle(STD_OUTPUT_HANDLE), &irInBuf, 1, &dw);
            break;
          default:
            //CCDEBUG("Unknown event type");
            break;
        }
      }
    }
    else if (oStringBuffer.size())
    {
      CcStatic::memcpy(pBuffer, oStringBuffer.getCharString(), CCMIN(oStringBuffer.length(), uSize));
      uiRet += CCMIN(oStringBuffer.length(), uSize);
      break;
    }
    else if (!cNumRead)
    {
      CcKernel::sleep(10);
    }
  }
  return uiRet;
}

size_t CcWinRawConsole::readHidden(void* pBuffer, size_t uSize)
{
  size_t uiRet = read(pBuffer, uSize);
  return uiRet;
}

size_t CcWinRawConsole::write(const void* pBuffer, size_t uSize)
{
  return CcConsole::write(pBuffer, uSize);
}

CcStatus CcWinRawConsole::open(EOpenFlags)
{
  CcStatus oRet = false;
  // Get the standard input handle.

  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdout == INVALID_HANDLE_VALUE)
  {
    CCDEBUG("GetStdHandle");
  }
  else
  {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
    {
      CCDEBUG("GetStdHandle");
    }
    else
    {
      // Save the current input mode, to be restored on exit.
      if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
      {
        CCDEBUG("GetConsoleMode");
      }
      else
      {
        // Enable the window and mouse input events.
        //uint32_t fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
        //if (!SetConsoleMode(hStdin, fdwMode))
        //{
        //  CCDEBUG("SetConsoleMode");
        //}
        //else
        //{
          oRet = true;
        //}
      }
    }
  }
  return oRet;
}

CcStatus CcWinRawConsole::close()
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

CcStatus CcWinRawConsole::cancel()
{
  CcStatus oRet = EStatus::AllOk;
  bCancel = true;
  INPUT_RECORD ir;
  ir.Event.FocusEvent.bSetFocus = TRUE;
  DWORD dw;
  WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &dw);
  return oRet;
}

CcStatus CcWinRawConsole::ioControl(uint32 , const void *, size_t , void *, size_t , size_t* )
{
  CcStatus oRet = EStatus::NotSupported;
  return oRet;
}

void* CcWinRawConsole::getStdFile()
{
  return hStdin;
}

CcStatus CcWinRawConsole::flush()
{
  CcStatus oRet = EStatus::NotSupported;
  return oRet;
}

#endif // WINDOWS