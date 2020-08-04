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
#include "CcWindowsTimer.h"
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
  void initFilesystem();
  void initNetworkStack();

  void deinitSystem();
  void deinitFilesystem();
  void deinitNetworkStack();

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
    // Do net create threads wich are not in starting state
    CcSystem::CPrivate::s_oCurrentExitCode = pThreadObject->startOnThread();
    s_oThreadManager.removeThread(pThreadObject);
    DWORD dwReturn = static_cast<DWORD>(CcSystem::CPrivate::s_oCurrentExitCode.getErrorUint());
    return dwReturn;
  }

  CcVector<IDevice*>        oIdleList;
#ifdef CC_STATIC
  CcWindowsSharedMemory     *pProcMemory;
  bool bProcMemoryCreated = false;
#endif

  CcVector<IDevice*> m_oDeviceList;
  CcList<CcWindowsModule> m_oModules;

  CcSharedPointer<CcWindowsFilesystem>            pFilesystem;
  //CcSharedPointer<CcWindowsRegistryFilesystem>  pRegistryFilesystem;
  CcSharedPointer<INetworkStack> pNetworkStack;

  static CcThreadManager s_oThreadManager;
  static CcStatus s_oCurrentExitCode;
};

CcThreadManager CcSystem::CPrivate::s_oThreadManager;
CcStatus        CcSystem::CPrivate::s_oCurrentExitCode;

CcSystem::CcSystem()
{
  CCNEW(m_pPrivate, CPrivate);
  #ifdef CC_STATIC
    DWORD uiProc = GetCurrentProcessId();
    {
      CcString sName = "CcOS_" + CcString::fromSize(static_cast<size_t>(uiProc));
      CCNEW(m_pPrivate->pProcMemory, CcWindowsSharedMemory, sName, sizeof(IKernel));
    }
    if (m_pPrivate->pProcMemory->exists())
    {
      if (m_pPrivate->pProcMemory->claim(EOpenFlags::Read))
      {
        CcKernel::setInterface(static_cast<IKernel*>(m_pPrivate->pProcMemory->getBuffer())->pBaseObject);
      }
    }
    else
    {
      if (m_pPrivate->pProcMemory->open(EOpenFlags::ReadWrite))
      {
        m_pPrivate->bProcMemoryCreated = true;
        CcStatic::memcpy(m_pPrivate->pProcMemory->getBuffer(), &CcKernel::getInterface(), sizeof(IKernel));
      }
    }
  #endif
}

CcSystem::~CcSystem()
{
  m_pPrivate->m_oDeviceList.clear();
  #ifdef CC_STATIC
    if (m_pPrivate->pProcMemory)
    {
      if(m_pPrivate->bProcMemoryCreated)
        m_pPrivate->pProcMemory->close();
      // Keep the order it is matching with initializing
      CcKernel::setInterface(nullptr);
      CCDELETE(m_pPrivate->pProcMemory);
    }
  #endif
  CCDELETE(m_pPrivate);
}

void CcSystem::init()
{
  m_pPrivate->initSystem();
  m_pPrivate->initFilesystem();
  m_pPrivate->initNetworkStack();
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
  m_pPrivate->deinitNetworkStack();
  m_pPrivate->deinitFilesystem();
  m_pPrivate->deinitSystem();
  for (IDevice* pDevice : m_pPrivate->m_oDeviceList)
  {
    CCDELETE( pDevice);
  }
  for (CcWindowsModule& oModule : m_pPrivate->m_oModules)
  {
    oModule.unloadModule();
  }
  m_pPrivate->m_oModules.clear();
}

bool CcSystem::initGUI()
{
  FreeConsole();
  return true; // YES we have a gui
}

bool CcSystem::initCLI()
{
  bool bRet = false;
  HWND hConsoleWnd = GetConsoleWindow();
  if (hConsoleWnd != NULL)
  {
    bRet = true; // YES we have a cli
  }
  else
  {
    if (AllocConsole())
    {
      FILE* out;
#ifndef __GNUC__
      freopen_s(&out, "conin$", "r", stdin);
      freopen_s(&out, "conout$", "w", stdout);
      freopen_s(&out, "conout$", "w", stderr);
#else
      out = freopen("conin$", "r", stdin);
      out = freopen("conout$", "w", stdout);
      out = freopen("conout$", "w", stderr);
#endif
      CCUNUSED(out);
    }
    bRet = true;
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) CcSystem::CPrivate::CtrlHandler, TRUE))
    {
    }
  }
  return bRet;
}

bool CcSystem::deinitCLI()
{
  bool bRet = false;
  HWND hConsoleWnd = GetConsoleWindow();
  if (hConsoleWnd != NULL)
  {
    bRet = FALSE != FreeConsole();
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

bool CcSystem::isAdmin()
{
  return IsUserAnAdmin() != FALSE;
}

void CcSystem::CPrivate::initSystem()
{
}

void CcSystem::CPrivate::initFilesystem()
{
  // append root mount point to CcFileSystem
  CCNEW(pFilesystem, CcWindowsFilesystem);
  CcFileSystem::addMountPoint("/", pFilesystem.handleCasted<IFileSystem>());
}

void CcSystem::CPrivate::initNetworkStack()
{
#ifdef WINDOWS_NETWORK_STACK
  CCNEW(pNetworkStack, CcNetworkStack);
#else
  CCNEW(pNetworkStack, CcWindowsNetworkStack);
#endif
  pNetworkStack->init();
}

void CcSystem::CPrivate::deinitSystem()
{
  s_oThreadManager.closeAll();
}

void CcSystem::CPrivate::deinitFilesystem()
{
  CcFileSystem::removeMountPoint("/");
  pFilesystem.clear();
}

void CcSystem::CPrivate::deinitNetworkStack()
{
  pNetworkStack.clear();
}

bool CcSystem::createThread(IThread &Thread)
{
  DWORD threadId;
  Thread.enterState(EThreadState::Starting);
  if (nullptr == CreateThread(0, Thread.getStackSize(), CcSystem::CPrivate::threadFunction, (void*)&Thread, 0, &threadId))
    return false;
  else
  {
    m_pPrivate->s_oThreadManager.addThread(&Thread);
    return true;
  }
}

bool CcSystem::createProcess(CcProcess &processToStart)
{
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

typedef bool(*KernelEntry)(CcKernel*);

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

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, size_t uiNr)
{
  CcDeviceHandle oDevice;
  CCUNUSED(uiNr);
  switch (Type)
  {
    case EDeviceType::WlanClient:

      break;
    case EDeviceType::Timer:
    {
      if (uiNr == 0)
      {
        CCNEWTYPE(pTimer, CcWindowsTimer);
        m_pPrivate->m_oDeviceList.append(static_cast<IDevice*>(pTimer));
        oDevice = CcDeviceHandle(pTimer, EDeviceType::Timer);
        CcKernel::addDevice(oDevice);
      }
      break;
    }
    default:
      break;
  }
  return oDevice;
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{
  CCUNUSED(Type); CCUNUSED(Name); return nullptr;
}

ISocket* CcSystem::getSocket(ESocketType type)
{
  ISocket* newSocket = nullptr;
  if (m_pPrivate->pNetworkStack != nullptr)
  {
    newSocket = m_pPrivate->pNetworkStack->getSocket(type);
  }
  return newSocket;
}

INetworkStack* CcSystem::getNetworkStack()
{
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
  CcWindowsModule oModule;
  CcStatus oStatus(false);
  bool bFound = false;
  for (CcWindowsModule& rModule : m_pPrivate->m_oModules)
    if (rModule.getName() == sPath)
      bFound = true;
  if (bFound == false)
  {
    oStatus = oModule.loadModule(sPath, oKernel);
    if (oStatus)
    {
      m_pPrivate->m_oModules.append(oModule);
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

void CcSystem::registerForIdle(IDevice* pDevice)
{
  m_pPrivate->oIdleList.append(pDevice);
}

void CcSystem::deregisterForIdle(IDevice* pDevice)
{
  m_pPrivate->oIdleList.removeItem(pDevice);
}
