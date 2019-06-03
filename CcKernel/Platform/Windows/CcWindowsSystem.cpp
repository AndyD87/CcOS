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
 * @brief     implementations for Class CcSystem
 **/

#include "CcWindowsGlobals.h"
#include <stdio.h>

#include "CcSystem.h"
#include "CcGroupList.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcWString.h"
#include "CcUserList.h"
#include "CcFileSystem.h"
#include "CcWindowsTouch.h"
#include "CcWindowsService.h"
#include "CcWindowsTimer.h"
#include "CcWindowsPipe.h"
#include "CcWindowsFilesystem.h"
#include "CcWindowsRegistryFilesystem.h"
#include "CcWindowsSocketUdp.h"
#include "CcWindowsSocketTcp.h"
#include "CcWindowsProcessThread.h"
#include "CcWindowsUser.h"
#include "CcWindowsSharedMemory.h"
#include "Network/CcNetworkStack.h"
#include <io.h>
#include <fcntl.h>
#include <LM.h>
#include <Lmcons.h>
#include <Shlobj.h>
#include <direct.h>
#include <ctime>
#include <signal.h>

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

class CcSystem::CPrivate
{
public:
  void initSystem();
  void initTimer();
  void initFilesystem();
#ifdef WINDOWS_NETWORK_STACK
  void initNetworkStack();
#endif

  CcList<IDevice*> m_oDeviceList;

  CcSharedPointer<CcWindowsFilesystem>            pFilesystem;
  //CcSharedPointer<CcWindowsRegistryFilesystem>  pRegistryFilesystem;
  bool bCliInitialized = false;
#ifdef WINDOWS_NETWORK_STACK
  CcSharedPointer<CcNetworkStack> pNetworkStack;
#endif
  static CcStatus s_oCurrentExitCode;

  static BOOL CtrlHandler(DWORD fdwCtrlType)
  {
    switch (fdwCtrlType)
    {
      case CTRL_C_EVENT:
      case CTRL_CLOSE_EVENT:
      case CTRL_BREAK_EVENT:
      case CTRL_SHUTDOWN_EVENT:
        CcKernel::shutdown();
        exit(CcSystem::CPrivate::s_oCurrentExitCode);
      case CTRL_LOGOFF_EVENT:
        return FALSE;
      default:
        return FALSE;
    }
  }

  static void SetThreadName(const char* threadName)
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
  static DWORD WINAPI threadFunction(void *Param)
  {
    // Just set Name only on debug ( save system ressources )
    IThread *pThreadObject = static_cast<IThread *>(Param);
    // Do net create threads wich are not in starting state
    if (pThreadObject->getThreadState() == EThreadState::Starting)
    {
#ifdef DEBUG
      SetThreadName(pThreadObject->getName().getCharString());
#endif
      pThreadObject->enterState(EThreadState::Running);
    }
    
    CcSystem::CPrivate::s_oCurrentExitCode = pThreadObject->enterState(EThreadState::Stopped);
    return 0;
  }

};

CcStatus CcSystem::CPrivate::s_oCurrentExitCode;

CcSystem::CcSystem()
{
  m_pPrivateData = new CPrivate();
  CCMONITORNEW(m_pPrivateData);
}

CcSystem::~CcSystem() 
{
  m_pPrivateData->m_oDeviceList.clear();
  CCDELETE(m_pPrivateData);
}

void CcSystem::init()
{
  m_pPrivateData->initSystem();
  m_pPrivateData->initFilesystem();
#ifdef WINDOWS_NETWORK_STACK
  m_pPrivateData->initNetworkStack();
#endif
  HWND hConsoleWnd = GetConsoleWindow();
  if (hConsoleWnd != NULL)
  {
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) CcSystem::CPrivate::CtrlHandler, TRUE))
    {
      CCVERBOSE("Console handler set");
    }
    else
    {
      CCERROR("failed to set console handler routine: %08x" + CcString::fromNumber( GetLastError()));
    }
  }
}

void CcSystem::deinit()
{
  for (IDevice* pDevice : m_pPrivateData->m_oDeviceList)
  {
    CCDELETE( pDevice);
  }
}

bool CcSystem::initGUI()
{
  if (m_pPrivateData->bCliInitialized == false)
    FreeConsole();
  return true; // YES we have a gui
} 

bool CcSystem::initCLI()
{
  bool bRet = false;
  HWND hConsoleWnd = GetConsoleWindow();
  if (hConsoleWnd != NULL)
  {
    // console window found
    m_pPrivateData->bCliInitialized = true;
    bRet = true; // YES we have a cli
  }
  else
  {
    CCDEBUG("GetConsoleWindow not found");
    AllocConsole();
    FILE* out;
    freopen_s(&out, "conin$", "r", stdin);
    freopen_s(&out, "conout$", "w", stdout);
    freopen_s(&out, "conout$", "w", stderr);
    bRet = true;
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) CcSystem::CPrivate::CtrlHandler, TRUE))
    {
    }
  }
  return bRet;
}

int CcSystem::initService()
{
  if (FreeConsole() == 0)
    return 0;
  else
    return -1;
}

void CcSystem::CPrivate::initFilesystem()
{
  CcFileSystem::init();
  pFilesystem = new CcWindowsFilesystem(); 
  CCMONITORNEW(pFilesystem.ptr());
  // append root mount point to CcFileSystem
  CcFileSystem::addMountPoint("/", pFilesystem.handleCasted<IFileSystem>());
  //pRegistryFilesystem = new CcWindowsRegistryFilesystem();
  //CCMONITORNEW(pRegistryFilesystem.ptr());
  //CcFileSystem::addMountPoint("/reg", pRegistryFilesystem.handleCasted<IFileSystem>());
}

#ifdef WINDOWS_NETWORK_STACK
void CcSystem::CPrivate::initNetworkStack()
{
  pNetworkStack = new CcNetworkStack();
  pNetworkStack->init();
}
#endif


void CcSystem::CPrivate::initSystem()
{
  initTimer();
}

void CcSystem::CPrivate::initTimer()
{
  CcWindowsTimer* pTimer = new CcWindowsTimer();
  CCMONITORNEW((void*) pTimer);
  m_oDeviceList.append(static_cast<IDevice*>(pTimer));
  CcKernel::addDevice(CcDeviceHandle(pTimer, EDeviceType::Timer));
}

bool CcSystem::createThread(IThread &Thread)
{
  DWORD threadId;
  Thread.enterState(EThreadState::Starting);
  if (nullptr == CreateThread(0, Thread.getStackSize(), CcSystem::CPrivate::threadFunction, (void*)&Thread, 0, &threadId))
    return false;
  else
    return true;
}

bool CcSystem::createProcess(CcProcess &processToStart)
{
  CcWindowsPipe* pPipe = new CcWindowsPipe();
  processToStart.setPipe(pPipe);
  CcWindowsProcessThread* pWorker = new CcWindowsProcessThread(processToStart);
  CCMONITORNEW(pWorker);
  pWorker->start();
  processToStart.setThreadHandle(pWorker);
  return true;
}

typedef bool(*KernelEntry)(CcKernel*);

//void CcSystem::loadModule(const CcString& Path)
//{
//  HINSTANCE hinstLib = LoadLibraryW((wchar_t*)Path.getWString().getWcharString());
//  KernelEntry ProcAdd;
//  BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
//  if (hinstLib != nullptr)
//  {
//    ProcAdd = (KernelEntry)GetProcAddress(hinstLib, "KernelEntry");
//
//    // If the function address is valid, call the function.
//
//    if (nullptr != ProcAdd)
//    {
//      fRunTimeLinkSuccess = TRUE;
//    }
//    else{
//      CCDEBUG("%d" + CcString::fromNumber(GetLastError()));
//    }
//    // Free the DLL module.
//
//    fFreeResult = FreeLibrary(hinstLib);
//  }
//  // If unable to call the DLL function, use an alternative.
//  if (!fRunTimeLinkSuccess)
//    CCDEBUG("Message printed from executable");
//}

CcStringMap CcSystem::getEnvironmentVariables() const
{
  CcStringMap oRet;
  wchar_t* pwcAllEnv = GetEnvironmentStringsW();
  if (pwcAllEnv != nullptr)
  {
    size_t uiOffset = 0;
    bool bDone = false;
    while (bDone == false)
    {
      if (pwcAllEnv[uiOffset] == 0)
      {
        break;
      }
      CcString sName;
      CcString sValue;
      while (pwcAllEnv[uiOffset] != '=')
      {
        sName.appendWchar(pwcAllEnv[uiOffset]);
        uiOffset++;
      }
      uiOffset++;
      while (pwcAllEnv[uiOffset] != 0)
      {
        sValue.appendWchar(pwcAllEnv[uiOffset]);
        uiOffset++;
      }
      uiOffset++;
      oRet.append(sName, sValue);
    }
    FreeEnvironmentStringsW(pwcAllEnv);
  }
  return oRet;
}

CcString CcSystem::getEnvironmentVariable(const CcString& sName) const
{
  CcWString wsValue;
  CcWString wsName = sName;
  // retreive size
  DWORD uiResult = GetEnvironmentVariableW(wsName.getWcharString(), NULL, 0);
  if (uiResult == ERROR_ENVVAR_NOT_FOUND)
  {
    CCDEBUG("getEnvironmentVariable " + sName + " not found");
  }
  else
  {
    if (uiResult > 0)
    {
      // Windows returned required size of bother including \0, but this is not required for String class
      wsValue.resize(uiResult - 1);
      // Buffer in String class is one higher than length because of \0 at the end
      GetEnvironmentVariableW(wsName.getWcharString(), wsValue.getWcharString(), static_cast<DWORD>(wsValue.length() + 1));
    }
  }
  return wsValue.getString();
}

bool CcSystem::getEnvironmentVariableExists(const CcString& sName) const
{
  bool bRet = false;
  CcWString wsName = sName;
  // retreive size
  DWORD uiResult = GetEnvironmentVariableW(wsName.getWcharString(), NULL, 0);
  if (uiResult != ERROR_ENVVAR_NOT_FOUND)
  {
    bRet = true;
  }
  return bRet;
}

bool CcSystem::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  if (SetEnvironmentVariableW(sName.getWString().getWcharString(), sValue.getWString().getWcharString()))
  {
    return true;
  }
  else
  {
    CCDEBUG("setEnvironmentVariable failed: " + CcString::fromNumber(GetLastError(), 16));
  }
  return false;
}

bool CcSystem::removeEnvironmentVariable(const CcString& sName)
{
  // Seting Value to null will delete Variable
  BOOL bResult = SetEnvironmentVariableW(sName.getWString().getWcharString(), nullptr);
  if (bResult)
  {
    return true;
  }
  else
  {
    CCDEBUG("removeEnvironmentVariable failed: " + CcString::fromNumber(GetLastError(), 16));
  }
  return false;
}

CcDateTime CcSystem::getDateTime()
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
  Sleep(max(dwTemp,1));
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{
  CCUNUSED(Type); CCUNUSED(Name); return nullptr;
}

ISocket* CcSystem::getSocket(ESocketType type)
{
  ISocket* newSocket;
  if (CcNetworkStack::instance() != nullptr)
  {
    newSocket = CcNetworkStack::instance()->getSocket(type);
  }
  else
  {
    switch (type)
    {
    case ESocketType::TCP:
      newSocket = new CcWindowsSocketTcp();
      CCMONITORNEW(newSocket);
      break;
    case ESocketType::UDP:
      newSocket = new CcWindowsSocketUdp();
      CCMONITORNEW(newSocket);
      break;
    default:
      newSocket = nullptr;
    }
  }
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
          CcWindowsUser *User = new CcWindowsUser(sTemp); 
          CCMONITORNEW(User);
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
    CcWindowsUser *User = new CcWindowsUser(pcCurUser); 
    CCMONITORNEW(User);
    UserList.append(User);
    UserList.setCurrentUser(pcCurUser);
  }
  return UserList;
}


ISharedMemory* CcSystem::getSharedMemory(const CcString &sName, size_t uiSize)
{
  return static_cast<ISharedMemory*>(new CcWindowsSharedMemory(sName, uiSize));
}


CcGroupList CcSystem::getGroupList()
{
  return CcGroupList();
}

CcString CcSystem::getConfigDir() const
{
  CcString sRet;
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_ProgramData, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, wcslen(programdata));
    sRet.normalizePath();
  }
  return sRet;
}

CcString CcSystem::getDataDir() const
{
  return getConfigDir();
}

CcString CcSystem::getBinaryDir() const
{
  CcString sRet;
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_ProgramFilesCommon, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, wcslen(programdata));
    sRet.normalizePath();
  }
  return sRet;
}

CcString CcSystem::getWorkingDir() const
{
  CcString sRet;
  wchar_t programdata[FILENAME_MAX];
  if (_wgetcwd(programdata, FILENAME_MAX))
  {
    sRet.fromUnicode(programdata, wcslen(programdata));
    sRet.setOsPath(sRet);
  }
  return sRet;
}

CcString CcSystem::getTemporaryDir() const
{
  CcString sRet;
  CcWString sTempString(MAX_PATH);
  DWORD uiLength = GetTempPathW(static_cast<DWORD>(sTempString.length()), sTempString.getWcharString());
  if (uiLength > 0)
  {
    sTempString.resize(uiLength);
    sRet = sTempString.getString().getOsPath().replace('\\', '/');
    if (sRet.last() == '/')
      sRet.remove(sRet.length() - 1);
  }
  return sRet;
}

CcString CcSystem::getUserDir() const
{
  CcString sRet;
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, wcslen(programdata));
    sRet.normalizePath();
  }
  return sRet;
}

CcString CcSystem::getUserDataDir() const
{
  CcString sRet;
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, wcslen(programdata));
    sRet.normalizePath();
  }
  return sRet;
}

void CcSystem::warning()
{
}

void CcSystem::error()
{
}
