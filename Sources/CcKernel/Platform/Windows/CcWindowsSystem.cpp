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
 * @brief     implementations for Class CcSystem
 **/

#include "CcWindowsGlobals.h"
#include "CcSystem.h"
#include "CcGroupList.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcStringUtil.h"
#include "CcWString.h"
#include "CcUserList.h"
#include "CcVersion.h"
#include "CcFileSystem.h"
#include "CcWindowsTouch.h"
#include "CcWindowsService.h"
#include "CcWindowsBoardSupport.h"
#include "CcWindowsServiceControl.h"
#include "CcWindowsPipe.h"
#include "CcWindowsFile.h"
#include "CcWindowsFilesystem.h"
#include "CcWindowsRegistryFilesystem.h"
#include "CcWindowsProcessThread.h"
#include "CcWindowsUser.h"
#include "CcWindowsSharedMemory.h"
#include "CcWindowsNetworkStack.h"
#include "Network/Stack/CcNetworkStack.h"
#include "CcWindowsModule.h"
#include "CcThreadManager.h"

CCEXTERNC_BEGIN
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <lm.h>
#include <lmcons.h>
#include <shlobj.h>
#include <direct.h>
#include <signal.h>
#include <knownfolders.h>
CCEXTERNC_END
#include <ctime>

//! Code is from http://msdn.microsoft.com/de-de/library/xcb2z8hs.aspx
#define MS_VC_EXCEPTION 0x406d1388
#ifndef CC_AVOID_UWP
  //! Avoid UWP dll usage, they are not installed everywhere
  #define CC_AVOID_UWP    1
#endif
#pragma pack(push,8)
//! Thread name info as defined in Windows
typedef struct tagTHREADNAME_INFO
{
  DWORD dwType;     //!< Must be 0x1000.
  LPCSTR szName;    //!< Pointer to name (in user addr space).
  DWORD dwThreadID; //!< Thread ID (-1=caller thread).
  DWORD dwFlags;    //!< Reserved for future use, must be zero.
} THREADNAME_INFO; //!< Type for tagTHREADNAME_INFO
#pragma pack(pop)

#ifdef CcKernel_EXPORTS
BOOL WINAPI DllMain
(
  HINSTANCE CCUNUSED_PARAM(hinstDLL),   // handle to DLL module
  DWORD fdwReason,                      // reason for calling function
  LPVOID CCUNUSED_PARAM(lpReserved)     // reserved
)
{
  // Perform actions based on the reason for calling.
  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      // Perform any necessary cleanup.
      CcKernel::shutdown();
      break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif

class CStaticBarrier
{
#ifdef CC_STATIC
  public:
    CStaticBarrier(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel) :
      pKernelInstance(pKernelInstance),
      pTargetKernel(pTargetKernel)
    {
      DWORD uiProc = GetCurrentProcessId();
      {
        CcString sName = "CcOS_" + CcString::fromSize(static_cast<size_t>(uiProc));
        CCNEW(pProcMemory, CcWindowsSharedMemory, sName, sizeof(CcKernelPrivate*));
      }
      if (pProcMemory->exists())
      {
        if (pProcMemory->claim(EOpenFlags::Read))
        {
          *pTargetKernel = *static_cast<CcKernelPrivate**>(pProcMemory->getBuffer());
        }
      }
      else
      {
        if (pProcMemory->open(EOpenFlags::ReadWrite))
        {
          bProcMemoryCreated = true;
          *pTargetKernel = pKernelInstance;
          CcStatic::memcpy(pProcMemory->getBuffer(), &pKernelInstance, sizeof(CcKernelPrivate*));
        }
      }
    }

    ~CStaticBarrier()
    {
      *pTargetKernel = pKernelInstance;
      if (pProcMemory)
      {
        pProcMemory->close();
        CCDELETE(pProcMemory);
      }
    }

  private:
    CcWindowsSharedMemory*  pProcMemory = nullptr;
    bool                    bProcMemoryCreated = false;
#else
  public:
    CStaticBarrier(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel) :
      pKernelInstance(pKernelInstance),
      pTargetKernel(pTargetKernel)
    {
      *pTargetKernel = pKernelInstance;
    }
    ~CStaticBarrier()
    {
      *pTargetKernel = pKernelInstance;
    }

#endif
private:
  CcKernelPrivate* pKernelInstance;
  CcKernelPrivate** pTargetKernel;
};

class CcSystem::CPrivate : CStaticBarrier
{
public:
  CPrivate(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel) :
    CStaticBarrier(pKernelInstance, pTargetKernel)
  {

  }

  void initSystem();
  void initFilesystem();
  void initNetworkStack();

  void deinitSystem();
  void deinitFilesystem();
  void deinitNetworkStack();

  static BOOL CtrlHandler(DWORD fdwCtrlType)
  {
    BOOL bStopShutdown = FALSE;
    switch (fdwCtrlType)
    {
      case CTRL_C_EVENT:
        bStopShutdown = CcKernel::shutdown(CcKernelShutdownEvent::EReason::UserRequest)? FALSE : TRUE;
        if(!bStopShutdown) exit(CcSystem::CPrivate::s_oCurrentExitCode);
        break;
      case CTRL_CLOSE_EVENT:
      case CTRL_BREAK_EVENT:
      case CTRL_SHUTDOWN_EVENT:
        bStopShutdown = CcKernel::shutdown(CcKernelShutdownEvent::EReason::SystemShutdown) ? FALSE : TRUE;
        if (!bStopShutdown) exit(CcSystem::CPrivate::s_oCurrentExitCode);
      case CTRL_LOGOFF_EVENT:
      default:
        break;
    }
    return bStopShutdown;
  }

  static void SetThreadName(const char* threadName)
  {

#ifndef __GNUC__
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
#else
    CCUNUSED(threadName);
#endif
  }

  /**
   * @brief Function to start the ThreadObject
   * @param Param: Param containing pointer to ThreadObject
   * @return alway returns 0, todo: get success of threads as return value;
   */
  static DWORD WINAPI threadFunction(void *Param)
  {
    IThread *pThreadObject = static_cast<IThread *>(Param);
#ifdef DEBUG
    // Just set Name only on debug ( save system ressources )
    SetThreadName(pThreadObject->getName().getCharString());
#endif
    if (s_pThreadManager) s_pThreadManager->addThread(pThreadObject);
    // Do net create threads wich are not in starting state
    CcSystem::CPrivate::s_oCurrentExitCode = pThreadObject->startOnThread();
    DWORD dwReturn = static_cast<DWORD>(CcSystem::CPrivate::s_oCurrentExitCode.getErrorUint());
    if (s_pThreadManager) s_pThreadManager->removeThread(pThreadObject);
    return dwReturn;
  }

  CcVector<IDevice*>          oIdleList;
  CcVector<IDevice*>          oDeviceList;
  CcVector<CcWindowsModule*>  oModules;

  CcSharedPointer<CcWindowsFilesystem>   pFilesystem;
  //CcSharedPointer<CcWindowsRegistryFilesystem>  pRegistryFilesystem;
  CcSharedPointer<CcWindowsNetworkStack> pNetworkStack;

  CcThreadManager           oThreadManager;
  CcWindowsBoardSupport     m_oBoardSupport;
  static FILE*              s_pConsoleFile;
  static CcThreadManager*   s_pThreadManager;
  static CcStatus           s_oCurrentExitCode;
};

FILE*             CcSystem::CPrivate::s_pConsoleFile;
CcThreadManager*  CcSystem::CPrivate::s_pThreadManager;
CcStatus          CcSystem::CPrivate::s_oCurrentExitCode;


CcSystem::CcSystem(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel)
{
  CCNEW(m_pPrivate, CPrivate, pKernelInstance, pTargetKernel);
  CcSystem::CPrivate::s_pThreadManager = &m_pPrivate->oThreadManager;

  if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) CcSystem::CPrivate::CtrlHandler, TRUE))
  {
  }
}

CcSystem::~CcSystem()
{
  CcSystem::CPrivate::s_pThreadManager = nullptr;
  m_pPrivate->oDeviceList.clear();
  CCDELETE(m_pPrivate);
}

void CcSystem::init()
{
  CcSystem::CPrivate::s_pConsoleFile   = nullptr;
  m_pPrivate->initSystem();
  m_pPrivate->initFilesystem();

#if CC_AVOID_UWP
#else
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
#endif
}

void CcSystem::deinit()
{
  deinitCLI();
  m_pPrivate->deinitNetworkStack();
  m_pPrivate->deinitFilesystem();
  m_pPrivate->deinitSystem();
  for (IDevice* pDevice : m_pPrivate->oDeviceList)
    CCDELETE( pDevice);
  m_pPrivate->oDeviceList.clear();
  for (CcWindowsModule* pModule : m_pPrivate->oModules)
  {
    CCDELETE(pModule);
  }
  m_pPrivate->oModules.clear();
}

bool CcSystem::initGUI()
{
#if CC_AVOID_UWP
#else
  FreeConsole();
#endif
  return true; // YES we have a gui
}

bool CcSystem::initCLI()
{
  bool bRet = false;

#if CC_AVOID_UWP
#else
  HWND hConsoleWnd = GetConsoleWindow();
  if (hConsoleWnd != NULL)
  {
    bRet = true; // YES we have a cli
  }
  else
  {
    if (AllocConsole())
    {
      #ifndef __GNUC__
        freopen_s(&CcSystem::CPrivate::s_pConsoleFile, "conin$", "r", stdin);
        freopen_s(&CcSystem::CPrivate::s_pConsoleFile, "conout$", "w", stdout);
        freopen_s(&CcSystem::CPrivate::s_pConsoleFile, "conout$", "w", stderr);
      #else
        CcSystem::CPrivate::s_pConsoleFile = freopen("conin$", "r", stdin);
        CcSystem::CPrivate::s_pConsoleFile = freopen("conout$", "w", stdout);
        CcSystem::CPrivate::s_pConsoleFile = freopen("conout$", "w", stderr);
      #endif
    }
    bRet = true;
  }
#endif
  return bRet;
}

bool CcSystem::deinitCLI()
{
  bool bRet = false;
#if CC_AVOID_UWP
#else
  if (CcSystem::CPrivate::s_pConsoleFile)
  {
    FreeConsole();
    CcSystem::CPrivate::s_pConsoleFile = nullptr;
  }
#endif
  return bRet;
}

bool CcSystem::isAdmin()
{
  bool bIsAdmin = false;
  SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
  PSID AdministratorsGroup; 
  bIsAdmin = AllocateAndInitializeSid(
      &NtAuthority,
      2,
      SECURITY_BUILTIN_DOMAIN_RID,
      DOMAIN_ALIAS_RID_ADMINS,
      0, 0, 0, 0, 0, 0,
      &AdministratorsGroup) != FALSE; 
  if(bIsAdmin) 
  {
    BOOL b;
    if (CheckTokenMembership( NULL, AdministratorsGroup, &b)) 
    {
      bIsAdmin = b != FALSE;
    }
    FreeSid(AdministratorsGroup); 
  }
  return bIsAdmin;
}

void CcSystem::CPrivate::initSystem()
{
  CcKernel::addDevice(CcDevice(&m_oBoardSupport, EDeviceType::BoardSupport));
}

void CcSystem::CPrivate::initFilesystem()
{
  // append root mount point to CcFileSystem
  CCNEW(pFilesystem, CcWindowsFilesystem);
  CcFileSystem::addMountPoint("/", pFilesystem.handleCasted<IFileSystem>());
}

void CcSystem::CPrivate::initNetworkStack()
{
#ifdef GENERIC_NETWORK_STACK
  CCNEW(pNetworkStack, CcNetworkStack);
#else
  CCNEW(pNetworkStack, CcWindowsNetworkStack);
#endif
  pNetworkStack->init();
}

void CcSystem::CPrivate::deinitSystem()
{
  oThreadManager.closeAll();
}

void CcSystem::CPrivate::deinitFilesystem()
{
  CcFileSystem::removeMountPoint("/");
  pFilesystem.clear();
}

void CcSystem::CPrivate::deinitNetworkStack()
{
  if (pNetworkStack.isValid())
  {
    pNetworkStack->deinit();
    pNetworkStack.clear();
  }
}

bool CcSystem::createThread(IThread &Thread)
{
  DWORD threadId;
  if (nullptr == CreateThread(0, Thread.getStackSize(), CcSystem::CPrivate::threadFunction, (void*)&Thread, 0, &threadId))
    return false;
  else
  {
    return true;
  }
}

bool CcSystem::createProcess(CcProcess &processToStart)
{
  CCVERBOSE("CcSystem::createProcess");
  CCNEWTYPE(pPipe, CcWindowsPipe);
  processToStart.setPipe(pPipe);
  CCNEWTYPE(pWorker, CcWindowsProcessThread, processToStart);
  pWorker->start();
  processToStart.setThreadHandle(pWorker);
  return true;
}

void CcSystem::error()
{
}

void CcSystem::warning()
{
}

CcString CcSystem::getName()
{
  return CcString("Windows");
}

CcVersion CcSystem::getVersion()
{
  CcVersion oRet;
  DWORD dwLen, dwDummy;
  CcWString oKernelDll(L"Kernel32.dll");
  dwLen = GetFileVersionInfoSizeW(oKernelDll.getWcharString(), &dwDummy);
  if (dwLen > 0)
  {
    CCNEWARRAYTYPE(pcData, char, dwLen);
    if (GetFileVersionInfoW(oKernelDll.getWcharString(), 0, dwLen, pcData))
    {
      VS_FIXEDFILEINFO* pBuffer;
      UINT uiLen;
      if (VerQueryValueW(pcData, L"\\", (LPVOID*) &pBuffer, &uiLen) &&
          uiLen)
      {
        oRet.setMajor(pBuffer->dwProductVersionMS >> 16);
        oRet.setMinor(pBuffer->dwProductVersionMS & 0xffff);
        oRet.setBuild(pBuffer->dwProductVersionLS >> 16);
        oRet.setRevision(pBuffer->dwProductVersionLS & 0xffff);
      }
    }
    CCDELETEARR(pcData);
  }
  return oRet;
}

//! @return Get path to current running executable
CcString CcSystem::getCurrentExecutablePath() const
{
  CcString sPath;
  DWORD dwSize = 0;
  wchar_t* pwcBaseDir = nullptr;
  do
  {
    dwSize += MAX_PATH;
    CCDELETEARRAY(pwcBaseDir);
    CCNEWARRAY(pwcBaseDir, wchar_t, dwSize);
  } while (GetLastError() == ERROR_INSUFFICIENT_BUFFER);

  if (pwcBaseDir)
  {
    if (dwSize > GetModuleFileNameW(nullptr, pwcBaseDir, dwSize) &&
        GetLastError() == ERROR_SUCCESS)
    {
      CcWString wsPath(pwcBaseDir);
      sPath = wsPath.getString();
      sPath.normalizePath();
    }
    CCDELETEARRAY(pwcBaseDir);
  }
  return sPath;
}

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
  // Buffer in String class is one higher than length because of \0 at the end
  DWORD dwSize = GetEnvironmentVariableW(wsName.getWcharString(), nullptr, 0);
  if (GetLastError() != ERROR_ENVVAR_NOT_FOUND)
  {
    wsValue.reserve(dwSize-1);
    GetEnvironmentVariableW(wsName.getWcharString(), wsValue.getWcharString(), static_cast<DWORD>(wsValue.length() + 1));
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

CcDateTime CcSystem::getUpTime()
{
  return CcDateTimeFromMSeconds(GetTickCount64());
}

void CcSystem::sleep(uint32 timeoutMs)
{
  DWORD dwTemp =  timeoutMs;
  Sleep(CCMAX(dwTemp,1));
}

const CcDevice& CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{
  CCUNUSED(Type);
  CCUNUSED(Name);
  return CcDevice::NullDevice;
}

ISocket* CcSystem::getSocket(ESocketType type)
{
  ISocket* newSocket = nullptr;
  if (getNetworkStack() != nullptr)
  {
    newSocket = m_pPrivate->pNetworkStack->getSocket(type);
  }
  return newSocket;
}

INetworkStack* CcSystem::getNetworkStack()
{
  if(!m_pPrivate->pNetworkStack.isValid())
    m_pPrivate->initNetworkStack();
  return m_pPrivate->pNetworkStack;
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
          CCNEWTYPE(User, CcWindowsUser, sTemp);
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
    CCNEWTYPE(User, CcWindowsUser, pcCurUser);
    UserList.append(User);
    UserList.setCurrentUser(pcCurUser);
  }
  return UserList;
}

ISharedMemory* CcSystem::getSharedMemory(const CcString &sName, size_t uiSize)
{
  CCNEWTYPE(pSharedMem, CcWindowsSharedMemory, sName, uiSize);
  return static_cast<ISharedMemory*>(pSharedMem);
}

CcGroupList CcSystem::getGroupList()
{
  return CcGroupList();
}

CcString CcSystem::getConfigDir() const
{
  CcString sRet;
#if !defined(__GNUG__) || __GNUG__ > 4
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_ProgramData, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, CcStringUtil::strlen(programdata));
    sRet.normalizePath();
  }
#else
  wchar_t szPath[MAX_PATH];
  if(SUCCEEDED(SHGetFolderPathW(NULL,
                             CSIDL_COMMON_APPDATA,
                             NULL,
                             0,
                             szPath)))
  {
    sRet.fromUnicode(szPath, CcStringUtil::strlen(szPath));
    sRet.normalizePath();
  }
#endif
  return sRet;
}

CcString CcSystem::getDataDir() const
{
  return getConfigDir();
}

CcString CcSystem::getBinaryDir() const
{
  CcString sRet;
#if !defined(__GNUG__) || __GNUG__ > 4
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_ProgramFilesCommon, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, CcStringUtil::strlen(programdata));
    sRet.normalizePath();
  }
#else
  wchar_t szPath[MAX_PATH];
  if(SUCCEEDED(SHGetFolderPathW(NULL,
                             CSIDL_PROGRAM_FILES_COMMON,
                             NULL,
                             0,
                             szPath)))
  {
    sRet.fromUnicode(szPath, CcStringUtil::strlen(szPath));
    sRet.normalizePath();
  }
#endif
  return sRet;
}

CcString CcSystem::getWorkingDir() const
{
  CcString sRet;
  wchar_t programdata[FILENAME_MAX];
  if (_wgetcwd(programdata, FILENAME_MAX))
  {
    sRet.fromUnicode(programdata, CcStringUtil::strlen(programdata));
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
    sRet = sTempString.substr(0, uiLength).getString().getOsPath().replace('\\', '/');
    if (sRet.last() == '/')
      sRet.remove(sRet.length() - 1);
  }
  return sRet;
}

CcString CcSystem::getUserDir() const
{
  CcString sRet;
#if !defined(__GNUG__) || __GNUG__ > 4
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, CcStringUtil::strlen(programdata));
    sRet.normalizePath();
  }
#else
  wchar_t szPath[MAX_PATH];
  if(SUCCEEDED(SHGetFolderPathW(NULL,
                             CSIDL_MYDOCUMENTS,
                             NULL,
                             0,
                             szPath)))
  {
    sRet.fromUnicode(szPath, CcStringUtil::strlen(szPath));
    sRet.normalizePath();
  }
#endif
  return sRet;
}

CcString CcSystem::getUserDataDir() const
{
  CcString sRet;
#if !defined(__GNUG__) || __GNUG__ > 4
  PWSTR programdata;
  if (S_OK == SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &programdata))
  {
    sRet.fromUnicode(programdata, CcStringUtil::strlen(programdata));
    sRet.normalizePath();
  }
#else
  wchar_t szPath[MAX_PATH];
  if(SUCCEEDED(SHGetFolderPathW(NULL,
                             CSIDL_APPDATA,
                             NULL,
                             0,
                             szPath)))
  {
    sRet.fromUnicode(szPath, CcStringUtil::strlen(szPath));
    sRet.normalizePath();
  }
#endif
  return sRet;
}

CcStatus CcSystem::loadModule(const CcString& sPath, const IKernel& oKernel)
{
  CCNEWTYPE(pModule, CcWindowsModule);
  CcStatus oStatus(false);
  bool bFound = false;
  for (CcWindowsModule* rModule : m_pPrivate->oModules)
  {
    if (rModule->getName() == sPath)
    {
      bFound = true;
    }
  }
  if (bFound == false)
  {
    oStatus = pModule->loadModule(sPath, oKernel);
    if (oStatus)
    {
      m_pPrivate->oModules.append(pModule);
    }
    else
    {
      CCDELETE(pModule);
    }
  }
  else
  {
    oStatus = true;
  }
  return oStatus;
}

CcStatus CcSystem::setWorkingDir(const CcString& sPath)
{
  CcStatus oOk(false);
  CcString sNewPath = sPath;
  CcWString oPath = CcWindowsFile::toWindowsPath(sNewPath.getOsPath().getWString());
  oOk = FALSE != SetCurrentDirectoryW(oPath.getWcharString());
  return oOk;
}

CcStatus CcSystem::setName(const CcString& sName)
{
  CcStatus oOk(false);
  CcString sNewPath = sName;
  CcWString oPath = sNewPath.getWString();
  if (FALSE == SetComputerNameExW(ComputerNamePhysicalDnsHostname, oPath.getWcharString()))
  {
    CCDEBUG(CcString::fromNumber(GetLastError()));
  }
  return oOk;
}

void CcSystem::registerForIdle(IDevice* pDevice)
{
  m_pPrivate->oIdleList.append(pDevice);
}

bool CcSystem::deregisterForIdle(IDevice* pDevice)
{
  return m_pPrivate->oIdleList.removeItem(pDevice);
}

CcStatus CcSystem::shutdown(const CcString& sMessage, bool bForce)
{
  CcStatus oOk(false);
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  CcStatic_memsetZeroObject(tkp);

  // Get a token for this process. 
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
  {

  }
  else
  {
    // Get the LUID for the shutdown privilege. 
    if (!LookupPrivilegeValueW(NULL, FORCE_UNICODE(SE_SHUTDOWN_NAME), &tkp.Privileges[0].Luid))
    {
      CCDEBUG(CcString::fromNumber(GetLastError()));
    }
    else
    {
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0))
      {
        CCDEBUG(CcString::fromNumber(GetLastError()));
      }
      else
      {
        if (!InitiateSystemShutdownExA(
          nullptr,
          sMessage.getLPSTR(),
          0,
          bForce,
          false,
          SHTDN_REASON_MINOR_MAINTENANCE
        ))
        {
          CCDEBUG(CcString::fromNumber(GetLastError()));
        }
        else
        {
          oOk = true;
        }
      }
    }
  }

  return oOk;
}

CcStatus CcSystem::restart(const CcString& sMessage, bool bForce)
{
  CcStatus oOk(false);
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  CcStatic_memsetZeroObject(tkp);

  // Get a token for this process. 
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
  {

  }
  else
  {
    // Get the LUID for the shutdown privilege. 
    if (!LookupPrivilegeValueW(NULL, FORCE_UNICODE(SE_SHUTDOWN_NAME), &tkp.Privileges[0].Luid))
    {
      CCDEBUG(CcString::fromNumber(GetLastError()));
    }
    else
    {
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0))
      {
        CCDEBUG(CcString::fromNumber(GetLastError()));
      }
      else
      {
        if(!InitiateSystemShutdownExA(
          nullptr,
          sMessage.getLPSTR(),
          0,
          bForce,
          true,
          SHTDN_REASON_MINOR_MAINTENANCE
        ))
        {
          CCDEBUG(CcString::fromNumber(GetLastError()));
        }
        else
        {
          oOk = true;
        }
      }
    }
  }

  return oOk;
}
