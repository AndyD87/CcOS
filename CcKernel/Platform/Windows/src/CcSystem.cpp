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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSystem
 **/

#include "WindowsGlobals.h"

#include "CcSystem.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcWString.h"
#include "CcUserList.h"
#include "CcFileSystem.h"
#include "WindowsTimer.h"
#include "WindowsFilesystem.h"
#include "WindowsSocket.h"
#include "WindowsPipeIn.h"
#include "WindowsPipeOut.h"
#include "WindowsUser.h"
#include <io.h>
#include <fcntl.h>
#include <LM.h>
#include <Lmcons.h>
#include <Shlobj.h>
#include <direct.h>
#include <ctime>


CcSystem::CcSystem() :
  m_GuiInitialized(false)
{
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_ProgramData, 0, nullptr, &programdata))
  {
    m_sConfigDir.fromUnicode(programdata, wcslen(programdata));
    m_sConfigDir.normalizePath();
    m_sConfigDir << "/CcOS";
    m_sDataDir = programdata;
    m_sDataDir.normalizePath();
    m_sDataDir << "/CcOS";
  }
  else
  {
    m_sConfigDir = "";
    m_sDataDir = "";
  }
  if (S_OK == SHGetKnownFolderPath(FOLDERID_ProgramFilesCommon, 0, nullptr, &programdata))
  {
    m_sBinaryDir.fromUnicode(programdata, wcslen(programdata));
    m_sBinaryDir.normalizePath();
    m_sBinaryDir << "/CcOS";
  }
  else
  {
    m_sBinaryDir = "";
  }

  CcWString sTempString(MAX_PATH);
  DWORD uiLength = GetTempPathW(static_cast<DWORD>(sTempString.length()), sTempString.getWcharString());
  if (uiLength > 0)
  {
    sTempString.resize(uiLength);
    m_sTempDir = sTempString.getString().getOsPath().replace('\\', '/');
    if (m_sTempDir.last() == '/')
      m_sTempDir.remove(m_sTempDir.length() - 1);
  }
  else
  {
    m_sTempDir = "";
  }

  char cCurrentPath[FILENAME_MAX];
  if (_getcwd(cCurrentPath, sizeof(cCurrentPath)))
  {
    m_sWorking.fromUnicode(programdata, wcslen(programdata));
    m_sWorking.normalizePath();
  }
  m_Display = nullptr;
  m_Timer   = nullptr;
}

CcSystem::~CcSystem() {
}

void CcSystem::init(void)
{
  initSystem();
  initFilesystem();
}

bool CcSystem::initGUI(void)
{
  initDisplay();
  m_GuiInitialized = true;
  return true; // YES we have a gui
}

bool CcSystem::initCLI(void)
{
  HWND consoleWnd = GetConsoleWindow();
  DWORD dwProcessId;
  GetWindowThreadProcessId(consoleWnd, &dwProcessId);
  if (GetCurrentProcessId() != dwProcessId)
  {
    AllocConsole();
    FILE* out;
    freopen_s(&out, "conin$", "r", stdin);
    freopen_s(&out, "conout$", "w", stdout);
    freopen_s(&out, "conout$", "w", stderr);
  }
  else
  {
    // We are in a console
  }
  return true; // YES we have a cli
}

int CcSystem::initService()
{
  // @todo implementation with Windows Service Manager
  //       currently we simulate the started child process, but we are still on parent.
  return 1;
}

void CcSystem::initFilesystem()
{
  CcFileSystem::init();
  m_Filesystem = new WindowsFilesystem(); CCMONITORNEW(m_Filesystem.get());
  if (m_Filesystem->getFile(m_sConfigDir)->isDir() == false)
  {
    m_Filesystem->mkdir(m_sConfigDir);
  }
  if (m_Filesystem->getFile(m_sDataDir)->isDir() == false)
  {
    m_Filesystem->mkdir(m_sDataDir);
  }
  // append root mount point to CcFileSystem
  CcFileSystem::addMountPoint("/", m_Filesystem);
}

bool CcSystem::start( void )
{
  m_bSystemState = true; // We are done
  CcKernel::systemReady();
  MSG msg;
  while (m_bSystemState == true)
  {
    if (m_GuiInitialized)
    {
      for (;;)
      {
        BOOL bRet = GetMessage(&msg, nullptr, 0, 0);

        if (bRet > 0)  // (bRet > 0 indicates a message that must be processed.)
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        else if (bRet < 0)  // (bRet == -1 indicates an error.)
        {
          CCDEBUG("ERROR in main-loop");
        }
        else  // (bRet == 0 indicates "exit program".)
        {
          if (WindowsDisplay::hasOpenedWindows() == false)
            return false;
        }
      }
    }
    else
    {
      Sleep(100);
    }
  }
  return false;
}

void CcSystem::stop(void)
{
  PostQuitMessage(0);
  m_bSystemState = false;
}

// Code is from http://msdn.microsoft.com/de-de/library/xcb2z8hs.aspx
#define MS_VC_EXCEPTION 0x406d1388

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
  DWORD dwType;     // Must be 0x1000.
  LPCSTR szName;    // Pointer to name (in user addr space).
  DWORD dwThreadID; // Thread ID (-1=caller thread).
  DWORD dwFlags;    // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName(const char* threadName)
{
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = threadName;
  info.dwThreadID = ~static_cast<DWORD>(0);
  info.dwFlags = 0;

  __try
  {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*) &info);
  }
  __except (EXCEPTION_EXECUTE_HANDLER)
  {
  }
}
/**
 * @brief Function to start the ThreadObject
 * @param Param: Param containing pointer to ThreadObject
 * @return alway returns 0, todo: get success of threads as return value;
 */
DWORD WINAPI threadFunction(void *Param)
{
  // Just set Name only on debug ( save system ressources )
  CcThreadObject *pThreadObject = static_cast<CcThreadObject *>(Param);
#ifdef DEBUG
  SetThreadName(pThreadObject->getName().getCharString());
#endif
  pThreadObject->enterState(EThreadState::Running);
  pThreadObject->run();
  pThreadObject->enterState(EThreadState::Stopped);
  pThreadObject->onStopped();
  return 0;
}

bool CcSystem::createThread(CcThreadObject &Thread)
{
  DWORD threadId;
  Thread.enterState(EThreadState::Starting);
  if (nullptr == CreateThread(0, 0, threadFunction, (void*)&Thread, 0, &threadId))
    return false;
  else
    return true;
}

bool CcSystem::createProcess(CcProcess &processToStart)
{
  CcString commandline("\"" + processToStart.getApplication() + "\" " + processToStart.getArguments().collapseList(" "));
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  WindowsPipeIn  *pipeIn = 0;
  WindowsPipeOut *pipeOut = 0;
  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&pi, sizeof(pi));
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags |= STARTF_USESTDHANDLES;
  if (processToStart.getInput() != nullptr)
  {
    pipeIn = new WindowsPipeIn(processToStart.getInput()); CCMONITORNEW(pipeIn);
    si.hStdInput = pipeIn->m_Handle;
    pipeIn->start();
  }
  if (processToStart.getOutput() != nullptr)
  {
    pipeOut = new WindowsPipeOut(processToStart.getOutput()); CCMONITORNEW(pipeOut);
    si.hStdOutput = pipeOut->m_Handle;
    si.hStdError = pipeOut->m_Handle;
    pipeOut->start();
  }
  // Start the child process. 
  if (!CreateProcessW( nullptr,   // No module name (use command line)
                      commandline.getUnicode().getLPWSTR(),        // Command line
                      nullptr,           // Process handle not inheritable
                      nullptr,           // Thread handle not inheritable
                      TRUE,          // Set handle inheritance to FALSE
                      CREATE_NO_WINDOW,              // No creation flags
                      nullptr,           // Use parent's environment block
                      nullptr,           // Use parent's starting directory 
                      &si,            // Pointer to STARTUPINFO structure
                      &pi)           // Pointer to PROCESS_INFORMATION structure
    )
  {
    CCDEBUG("CreateProcess failed: " + CcString::fromNumber(GetLastError()));
  }
  else
  {
    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  // Close process and thread handles.
  if (pipeIn != 0)
  {
    CCMONITORDELETE(pipeIn); delete pipeIn;
  }
  if (pipeOut != 0)
  {
    CCMONITORDELETE(pipeOut); delete pipeOut;
  }
  return true;
}

typedef bool(*KernelEntry)(CcKernel*);

void CcSystem::loadModule(const CcString& Path)
{
  HINSTANCE hinstLib = LoadLibraryW((wchar_t*)Path.getUnicode().getWcharString());
  KernelEntry ProcAdd;
  BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
  if (hinstLib != nullptr)
  {
    ProcAdd = (KernelEntry)GetProcAddress(hinstLib, "KernelEntry");

    // If the function address is valid, call the function.

    if (nullptr != ProcAdd)
    {
      fRunTimeLinkSuccess = TRUE;
    }
    else{
      CCDEBUG("%d" + CcString::fromNumber(GetLastError()));
    }
    // Free the DLL module.

    fFreeResult = FreeLibrary(hinstLib);
  }
  // If unable to call the DLL function, use an alternative.
  if (!fRunTimeLinkSuccess)
    CCDEBUG("Message printed from executable");
}

CcDateTime CcSystem::getDateTime(void)
{
  CcDateTime oRet;
  FILETIME oFileTime;
  GetSystemTimeAsFileTime(&oFileTime);
  oRet.setFiletime((LONGLONG) oFileTime.dwLowDateTime + (((LONGLONG) oFileTime.dwHighDateTime) << 32));
  return oRet;
}

void CcSystem::sleep(uint32 timeoutMs)
{
  DWORD dwTemp =  timeoutMs;
  Sleep(dwTemp);
}

CcSocket* CcSystem::getSocket(ESocketType type)
{
  CcSocket* newSocket = new WindowsSocket(type); CCMONITORNEW(newSocket);
  return newSocket;
}

CcUserList CcSystem::getUserList()
{
  CcUserList UserList;
  LPUSER_INFO_1 pBuf = nullptr;
  LPUSER_INFO_1 pTmpBuf;
  DWORD dwLevel = 1;
  DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
  DWORD dwEntriesRead = 0;
  DWORD dwTotalEntries = 0;
  DWORD dwResumeHandle = 0;
  DWORD i;
  DWORD dwTotalCount = 0;
  NET_API_STATUS nStatus;

  do // begin do
  {
    nStatus = NetUserEnum(
      nullptr,
      dwLevel,
      FILTER_NORMAL_ACCOUNT, // global users
      (LPBYTE*)&pBuf,
      dwPrefMaxLen,
      &dwEntriesRead,
      &dwTotalEntries,
      &dwResumeHandle);
    //
    // If the call succeeds,
    //
    if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
    {
      if ((pTmpBuf = pBuf) != nullptr)
      {
        //
        // Loop through the entries.
        //
        for (i = 0; (i < dwEntriesRead); i++)
        {
          if (pTmpBuf == nullptr)
          {
            CCERROR("An access violation has occurred\n");
            break;
          }
          CcString sTemp(pTmpBuf->usri1_name);
          WindowsUser *User = new WindowsUser(sTemp); CCMONITORNEW(User);
          User->setWindowsHomeDir(pTmpBuf->usri1_home_dir);
          User->setWindowsPassword(pTmpBuf->usri1_password);
          UserList.append(User);
          pTmpBuf++;
          dwTotalCount++;
        }
      }
    }
    // Free the allocated buffer.
    if (pBuf != nullptr)
    {
      NetApiBufferFree(pBuf);
      pBuf = nullptr;
    }
  } while (nStatus == ERROR_MORE_DATA);

  wchar_t pcCurUser[UNLEN + 1];
  DWORD dwCurUserLen = UNLEN + 1;
  // It's possible that current user is in a domain and not listed in NetUserEnum
  GetUserNameW(pcCurUser, &dwCurUserLen);

  if (!UserList.setCurrentUser(pcCurUser))
  {
    WindowsUser *User = new WindowsUser(pcCurUser); CCMONITORNEW(User);
    UserList.append(User);
    UserList.setCurrentUser(pcCurUser);
  }
  return UserList;
}

void CcSystem::initSystem(void)
{
  initTimer();
}

void CcSystem::initTimer( void )
{
  m_Timer = new WindowsTimer();CCMONITORNEW((void*)m_Timer.get());
  CcKernel::addDevice(m_Timer.cast<CcDevice>(), EDeviceType::Timer); 
}

void CcSystem::initDisplay( void )
{
  m_Display = new WindowsDisplay(); CCMONITORNEW(m_Display.get());
  CcKernel::addDevice(m_Display.cast<CcDevice>(), EDeviceType::Display);
  m_Display->open();
}

void CcSystem::systemTick( void )
{
  CcKernel::systemTick();
}
