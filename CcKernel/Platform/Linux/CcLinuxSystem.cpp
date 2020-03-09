/**
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @brief     Implementation of Class CcSystem
 */
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcStringList.h"
#include "CcGroupList.h"
#include "CcLinuxTimer.h"
#include "CcLinuxTouch.h"
#include "CcLinuxFile.h"
#include "CcLinuxFilesystem.h"
#include "CcLinuxSocketTcp.h"
#include "CcLinuxSocketUdp.h"
#include "CcLinuxLed.h"
#include "CcLinuxGpioPort.h"
#include "CcLinuxProcessThread.h"
#include "CcLinuxUser.h"
#include "CcLinuxPipe.h"
#include "CcLinuxSharedMemory.h"
#include "CcLinuxNetworkStack.h"
#include "CcLinuxModule.h"
#include "CcDirectory.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcUserList.h"
#include "CcMapCommon.h"
#include "CcStringUtil.h"
#include "IModule.h"

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <csignal>
#include <cstdlib>

class CcSystem::CPrivate
{
public:
  void initSystem();
  void initTimer();
  void initDisplay();
  void initNetworkStack();

  CcLinuxNetworkStack* pNetworkStack;
  IFileSystem *pFilesystem;
  CcDeviceList m_cDeviceList;
  utsname oSysInfo;
  CcList<CcLinuxModule> m_oModules;

  static CcStringMap m_oEnvValues;
};
CcStringMap CcSystem::CPrivate::m_oEnvValues;

[[noreturn]] void CcSystemSignalHanlder(int s)
{
  switch(s)
  {
    case SIGINT:
      CCFALLTHROUGH;
    case SIGABRT:
      CCFALLTHROUGH;
    case SIGTERM:
      CcKernel::shutdown();
      break;
  }
  exit(1);
}

CcSystem::CcSystem()
{
  CCNEW(m_pPrivateData, CPrivate);
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = CcSystemSignalHanlder;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  if( 0 == sigaction(SIGINT, &sigIntHandler, nullptr))
  {
    CCVERBOSE("SIGINT handler successfully set");
    if( 0 == sigaction(SIGABRT, &sigIntHandler, nullptr))
    {
      CCVERBOSE("SIGABRT handler successfully set");
      if( 0 == sigaction(SIGTERM, &sigIntHandler, nullptr))
      {
        CCVERBOSE("SIGTERM handler successfully set");
      }
      else
      {
        CCERROR("Faild to set SIGTERM handler");
      }
    }
    else
    {
      CCERROR("Faild to set SIGABRT handler");
    }
  }
  else
  {
    CCERROR("Faild to set SIGINT handler");
  }
}

CcSystem::~CcSystem()
{
  for(CcLinuxModule& oModule : m_pPrivateData->m_oModules)
  {
    oModule.unloadModule();
  }
  for( ; 0 < m_pPrivateData->m_cDeviceList.size(); )
  {
    m_pPrivateData->m_cDeviceList.remove(0);
  }
  CCDELETE(m_pPrivateData->pFilesystem);
  CCDELETE(m_pPrivateData->pNetworkStack);
  CCDELETE(m_pPrivateData);
}

void CcSystem::init()
{
  signal(SIGPIPE, SIG_IGN);
  m_pPrivateData->initSystem();
  m_pPrivateData->initNetworkStack();
}

bool CcSystem::initGUI()
{
  m_pPrivateData->initDisplay();
  return true;
}

bool CcSystem::initCLI()
{
  return false;
}

bool CcSystem::deinitCLI()
{
  return false;
}

int CcSystem::initService()
{
  return fork();
}

bool CcSystem::isAdmin()
{
  int user;
  user = getuid();
  return user == 0;
}

void CcSystem::CPrivate::initSystem()
{
  CCNEW(pFilesystem, CcLinuxFilesystem);
  CcFileSystem::addMountPoint("/", pFilesystem);
  if(0!=uname(&oSysInfo))
  {
    CCDEBUG("Failed to retreive system info");
  }
}

void CcSystem::CPrivate::initNetworkStack()
{
  CCNEW(pNetworkStack, CcLinuxNetworkStack);
}

void CcSystem::CPrivate::initDisplay()
{
#if defined(CCOS_GUI) && (CCOS_GUI > 0)
  CCNEW(m_Display, CcX11SubSystem, 500, 500);
  CcKernel::addDevice(m_Display, EDeviceType::Display);
  m_Display->open();
#endif
}

void *CcSystem_threadFunction(void *Param)
{
  IThread *pThreadObject = static_cast<IThread *>(Param);
  pThreadObject->startOnThread();
  void* pReturn = reinterpret_cast<void*>(pThreadObject->getExitCode().getErrorInt());
  return pReturn;
}

bool CcSystem::createThread(IThread& oThread)
{
  pthread_t threadId;
  oThread.enterState(EThreadState::Starting);
  int iErr = pthread_create(&threadId, nullptr, CcSystem_threadFunction, static_cast<void*>(&oThread));
  if (0 == iErr)
  {
    pthread_detach(threadId);
    return true;
  }
  else
  {
    CCDEBUG("Failed to create thread: " + CcString::fromNumber(iErr));
    return false;
  }
}

bool CcSystem::createProcess(CcProcess& oProcessToStart)
{
  CCNEWTYPE(pPipe, CcLinuxPipe);
  oProcessToStart.setPipe(pPipe);
  CCNEWTYPE(pWorker, CcLinuxProcessThread, oProcessToStart);
  pWorker->start();
  oProcessToStart.setThreadHandle(pWorker);
  return true;
}

void CcSystem::error()
{
}

void CcSystem::warning()
{
}

ISocket* CcSystem::getSocket(ESocketType eType)
{
  ISocket* pNewSocket = nullptr;
  if(m_pPrivateData->pNetworkStack)
  {
    pNewSocket = m_pPrivateData->pNetworkStack->getSocket(eType);
  }
  return pNewSocket;
}

INetworkStack* CcSystem::getNetworkStack()
{
  return m_pPrivateData->pNetworkStack;
}

CcString CcSystem::getName()
{
  return CcString(m_pPrivateData->oSysInfo.sysname);
}

CcVersion CcSystem::getVersion()
{
  return CcVersion(m_pPrivateData->oSysInfo.release);
}

extern char **environ;
CcStringMap CcSystem::getEnvironmentVariables() const
{
  CcStringMap sRet;
  for(char **ppcCurrent = environ; *ppcCurrent != nullptr; ppcCurrent++)
  {
    char* pcCurrent = *ppcCurrent;
    char* pcFound   = CcStringUtil::strchr(pcCurrent, '=');
    if(pcFound != nullptr)
    {
      CcString sKey(pcCurrent, static_cast<size_t>(pcFound-pcCurrent));
      CcString sValue(pcFound+1);
      sRet.append(sKey, sValue);
    }
  }
  return sRet;
}

CcString CcSystem::getEnvironmentVariable(const CcString& sName) const
{
  CcString sValue;
  char* pcValue = getenv(sName.getCharString());
  if(pcValue != nullptr)
  {
    sValue.append(pcValue);
  }
  return sValue;
}

bool CcSystem::getEnvironmentVariableExists(const CcString& sName) const
{
  bool bRet = false;
  char* pcValue = getenv(sName.getCharString());
  if(pcValue != nullptr)
  {
    bRet = true;
  }
  return bRet;
}

bool CcSystem::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  CcString sSetString;
  int iResult = 0;
  sSetString << sName << "=" << sValue;
  if(CcSystem::CPrivate::m_oEnvValues.containsKey(sName))
  {
    CcString& rSetString = CcSystem::CPrivate::m_oEnvValues[sName];
    rSetString = sSetString;
    char* pcNewValue = rSetString.getCharString();
    iResult = putenv(pcNewValue);
  }
  else
  {
    CcSystem::CPrivate::m_oEnvValues.append(sName, sSetString);
    CcString& rSetString = CcSystem::CPrivate::m_oEnvValues.last().value();
    char* pcNewValue = rSetString.getCharString();
    iResult = putenv(pcNewValue);
  }
  if(iResult == 0)
  {
    return true;
  }
  CCDEBUG("setEnvironmentVariable failed: " + CcString::fromNumber(iResult, 16));
  return false;
}

bool CcSystem::removeEnvironmentVariable(const CcString& sName)
{
  if(0 == unsetenv(sName.getCharString()))
  {
    CcSystem::CPrivate::m_oEnvValues.removeKey(sName);
    return true;
  }
  return false;
}

CcDateTime CcSystem::getDateTime()
{
  timespec stClocktime;
  clock_gettime(CLOCK_REALTIME, &stClocktime);
  uint64 iMS = static_cast<uint64>(stClocktime.tv_nsec)/1000;
  uint64 iS  = static_cast<uint64>(stClocktime.tv_sec)*1000000;
  return CcDateTime(iMS + iS);
}

void CcSystem::sleep(uint32 timeoutMs)
{
  usleep(1000 * timeoutMs);
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, size_t uiNr)
{
  CCUNUSED(Type); CCUNUSED(uiNr); return nullptr;
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& sName)
{
  CcDeviceHandle pRet = nullptr;
  switch (Type) {
    case EDeviceType::Led:
    {
      CcString Path("/sys/class/leds/" + sName);
      CcFile ledFolder(Path);
      if(ledFolder.isDir())
      {
        CCNEWTYPE(pLed, CcLinuxLed, Path);
        pRet.set(pLed, EDeviceType::Led);
        m_pPrivateData->m_cDeviceList.append(pRet);
      }
      break;
    }
    case EDeviceType::GpioPort:
    {
      if(sName == "System")
      {
        pRet = m_pPrivateData->m_cDeviceList.getDevice(EDeviceType::GpioPort);
        if(pRet.isValid())
        {
          CCNEWTYPE(pPort, CcLinuxGpioPort);
          pRet.set(pPort, EDeviceType::GpioPort);
          m_pPrivateData->m_cDeviceList.append(pRet);
        }
      }
    }
    default:
      break;
  }
  return pRet;
}

CcString CcSystem::getConfigDir() const
{
  CcString sRet("/etc");
  if(!CcLinuxFile(sRet).isDir())
  {
    ::mkdir(sRet.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  return sRet;
}

CcString CcSystem::getDataDir() const
{
  CcString sRet("/var/lib");
  if(!CcLinuxFile(sRet).isDir())
  {
    ::mkdir(sRet.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  return sRet;
}

CcString CcSystem::getBinaryDir() const
{
  CcString sRet("/bin");
  if(!CcLinuxFile(sRet).isDir())
  {
    ::mkdir(sRet.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  return sRet;
}

CcString CcSystem::getWorkingDir() const
{
  CcString sRet;
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != nullptr)
  {
    sRet = cwd;
  }
  return sRet;
}

CcString CcSystem::getTemporaryDir() const
{
  CcString sRet;
  const char* pcTempDirPath = getenv("TMP");
  if (pcTempDirPath != nullptr)
  {
    sRet = pcTempDirPath;
  }
  else
  {
    const char* pcTempDirPath = getenv("TEMP");
    if (pcTempDirPath != nullptr)
    {
      sRet = pcTempDirPath;
    }
    else
    {
      sRet = "/tmp";
      if (!CcLinuxFile(sRet).isDir())
      {
        ::mkdir(sRet.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
      }
    }
  }
  return sRet;
}

CcString CcSystem::getUserDir() const
{
  CcString sRet(getpwuid(getuid())->pw_dir);
  if(!CcLinuxFile(sRet).isDir())
  {
    ::mkdir(sRet.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  return sRet;
}

CcString CcSystem::getUserDataDir() const
{
  CcString sRet(getpwuid(getuid())->pw_dir);
  sRet.appendPath(".CcOS");
  if(!CcLinuxFile(sRet).isDir())
  {
    ::mkdir(sRet.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  return sRet;
}

CcUserList CcSystem::getUserList()
{
  CcUserList UserList;
  passwd *retTemp;
  retTemp = getpwuid(getuid());
  CcString sCurUser(retTemp->pw_name);
  do
  {
    retTemp = getpwent();
    if(retTemp != nullptr)
    {
      CcString sUsername(retTemp->pw_name);
      CcString sHomeDir(retTemp->pw_dir);
      if(sUsername != sCurUser)
      {
        CCNEWTYPE(pNewUser, CcLinuxUser, sUsername, sHomeDir, retTemp->pw_uid, false);
        UserList.append(pNewUser);
      }
      else
      {
        CCNEWTYPE(pNewUser, CcLinuxUser, sUsername, sHomeDir, retTemp->pw_uid, true);
        UserList.append(pNewUser);
        UserList.setCurrentUser(sCurUser);
      }
    }
  } while(retTemp != nullptr);
  endpwent();
  return UserList;
}

ISharedMemory* CcSystem::getSharedMemory(const CcString &sName, size_t uiSize)
{
  CCNEWTYPE(pSharedMem, CcLinuxSharedMemory, sName, uiSize);
  return pSharedMem;
}

CcGroupList CcSystem::getGroupList()
{
  CcGroupList oGroups;
  CcFile oGroupFile("/etc/group");
  if(oGroupFile.open(EOpenFlags::Read))
  {
    CcString sGroupFile = oGroupFile.readAll();
    CcStringList slGroupLines = sGroupFile.splitLines();
    for(const CcString& sGroupLine : slGroupLines)
    {
      CcStringList slGroupData = sGroupLine.split(':');
      if(slGroupData.size() > 2)
      {
        CcGroup oGroup(slGroupData[0], slGroupData[2].toUint32());
        oGroups.append(oGroup);
      }
    }
  }
  return oGroups;
}

CcStatus CcSystem::loadModule(const CcString& sPath)
{
  CcLinuxModule oModule;
  CcStatus oStatus = oModule.loadModule(sPath);
  if(oStatus)
  {
    m_pPrivateData->m_oModules.append(oModule);
  }
  return oStatus;
}

CcStatus CcSystem::setWorkingDir(const CcString& sPath)
{
  CcStatus oOk(false);
  CcString sNewPath = sPath;
  oOk = 0 == chdir(sNewPath.normalizePath().getCharString());
  return oOk;
}
