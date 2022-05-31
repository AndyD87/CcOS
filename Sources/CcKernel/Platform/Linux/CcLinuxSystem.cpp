/**
 * @copyright  Andreas Dirmeier (C) 2022
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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcSystem
 */
#include "CcSystem.h"
#include "CcBaseSettings.h"
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
#include "CcLinuxBoardSupport.h"
#include "CcDirectory.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcUserList.h"
#include "CcMapCommon.h"
#include "CcStringUtil.h"
#include "IModuleBase.h"
#include "CcThreadManager.h"
#include "CcService.h"

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <csignal>
#include <cstdlib>
#include <libgen.h>         // dirname
#include <linux/limits.h>   // PATH_MAX
#include <sys/reboot.h>
#include <errno.h>

class CcSystem::CPrivate
{
public:
  CPrivate(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel)
  {
    *pTargetKernel = pKernelInstance;

  }
  void initSystem();
  void initTimer();
  void initDisplay();

  CcLinuxNetworkStack* getNetworkStack();
  void deinitNetworkStack();

  static void *ThreadFunction(void *Param)
  {
    IThread *pThreadObject = static_cast<IThread *>(Param);
    if(s_pThreadManager) s_pThreadManager->addThread(pThreadObject);
    pThreadObject->startOnThread();
    void* pReturn = reinterpret_cast<void*>(pThreadObject->getExitCode().getErrorInt());
    if(s_pThreadManager) s_pThreadManager->removeThread(pThreadObject);
    return pReturn;
  }

  CcMutex                   oDataLock;
  CcLinuxBoardSupport       oBoardSupport;
  IFileSystem*              pFilesystem;
  CcDeviceList              oDeviceList;
  utsname                   oSysInfo;
  CcList<CcLinuxModule*>    oModules;
  CcVector<IDevice*>        oIdleList;
  
  CcThreadManager           oThreadManager;
  static CcThreadManager*   s_pThreadManager;
  static CcStringMap        m_oEnvValues;

private:
  CcLinuxNetworkStack*      pNetworkStack;
};

CcThreadManager*  CcSystem::CPrivate::s_pThreadManager(nullptr);
CcStringMap       CcSystem::CPrivate::m_oEnvValues;

CcSharedPointer<CcService>  m_pService;

static void *FailbackExit(void *Param)
{
  CcKernel::stop();
  usleep(1000);
  CcKernel::shutdown();
  // 1 second for application to finish main, otherwise kill
  sleep(1);
  exit(1);
  return Param;
}

/**
 * @brief Signal handler for os transmitted signals
 * @param s: signal to handle
 */
void CcSystemSignalHandler(int s)
{
  CCDEBUG("Signal received: " + CcString::fromInt(s));
  switch(s)
  {
    case SIGINT:
      CCFALLTHROUGH;
    case SIGABRT:
      CCFALLTHROUGH;
    case SIGTERM:
      pthread_t threadId;
      pthread_create(&threadId, nullptr, FailbackExit, nullptr);
      break;
    default:
      exit(1);
  }
}

CcSystem::CcSystem(CcKernelPrivate* pKernelInstance, CcKernelPrivate** pTargetKernel)
{
  CCNEW(m_pPrivate, CPrivate, pKernelInstance, pTargetKernel);
  CcSystem::CPrivate::s_pThreadManager = &m_pPrivate->oThreadManager;
}

CcSystem::~CcSystem()
{
  CcSystem::CPrivate::s_pThreadManager = nullptr;
  CCDELETE(m_pPrivate);
}

void CcSystem::init()
{
  signal (SIGINT, CcSystemSignalHandler);
  if( SIG_ERR != signal(SIGINT, CcSystemSignalHandler))
  {
    CCVERBOSE("SIGINT handler successfully set");
    if( SIG_ERR != signal(SIGTERM, CcSystemSignalHandler))
    {
      CCVERBOSE("SIGTERM handler successfully set");
      if( SIG_ERR != signal(SIGABRT, CcSystemSignalHandler))
      {
        CCVERBOSE("SIGABRT handler successfully set");
      }
      else
      {
        CCERROR("Faild to set SIGABRT handler");
      }
    }
    else
    {
      CCERROR("Faild to set SIGTERM handler");
    }
  }
  else
  {
    CCERROR("Faild to set SIGINT handler");
  }
  signal(SIGPIPE, SIG_IGN);
  m_pPrivate->initSystem();
  CcKernel::addDevice(CcDevice(&m_pPrivate->oBoardSupport, EDeviceType::BoardSupport));
}

void CcSystem::deinit()
{
  CcKernel::removeDevice(CcDevice(&m_pPrivate->oBoardSupport, EDeviceType::BoardSupport));
  m_pPrivate->oThreadManager.closeAll();
  CcFileSystem::removeMountPoint("/");
  
  CCDELETE(m_pPrivate->pFilesystem);
  m_pPrivate->deinitNetworkStack();

  for (IDevice* pDevice : m_pPrivate->oDeviceList)
    CCDELETE( pDevice);
  m_pPrivate->oDeviceList.clear();
  for (CcLinuxModule* pModule : m_pPrivate->oModules)
    CCDELETE(pModule);
  m_pPrivate->oModules.clear();
}

bool CcSystem::initGUI()
{
  m_pPrivate->initDisplay();
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

bool CcSystem::isAdmin()
{
  int user;
  user = getuid();
  return user == 0;
}

void CcSystem::CPrivate::initSystem()
{
  CCNEW(pFilesystem, CcLinuxFilesystem);
  CcFileSystem::addMountPoint(CcGlobalStrings::Seperators::Slash, pFilesystem);
  if(0!=uname(&oSysInfo))
  {
    CCDEBUG("Failed to retreive system info");
  }
}

CcLinuxNetworkStack *CcSystem::CPrivate::getNetworkStack()
{
  oDataLock.lock();
  if(!pNetworkStack)
  {
    CCNEW(pNetworkStack, CcLinuxNetworkStack);
    pNetworkStack->init();
  }
  oDataLock.unlock();
  return pNetworkStack;
}

void CcSystem::CPrivate::deinitNetworkStack()
{
  oDataLock.lock();
  if(pNetworkStack)
  {
    pNetworkStack->deinit();
    CCDELETE(pNetworkStack);
  }
  oDataLock.unlock();
}

void CcSystem::CPrivate::initDisplay()
{
#if defined(CCOS_GUI) && (CCOS_GUI > 0)
  CCNEW(m_Display, CcX11SubSystem, 500, 500);
  CcKernel::addDevice(m_Display, EDeviceType::Display);
  m_Display->open();
#endif
}

bool CcSystem::createThread(IThread& oThread)
{
  pthread_t threadId;
  int iErr = pthread_create(&threadId, nullptr, CcSystem::CPrivate::ThreadFunction, static_cast<void*>(&oThread));
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
  if(getNetworkStack())
  {
    pNewSocket = getNetworkStack()->getSocket(eType);
  }
  return pNewSocket;
}

INetworkStack* CcSystem::getNetworkStack()
{
  return m_pPrivate->getNetworkStack();
}

CcString CcSystem::getName()
{
  return CcString(m_pPrivate->oSysInfo.sysname);
}

CcVersion CcSystem::getVersion()
{
  return CcVersion(m_pPrivate->oSysInfo.release);
}

//! Linux defined variable to access environment variables.ß´ß
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
    CcString& rSetString = CcSystem::CPrivate::m_oEnvValues.last().getValue();
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

CcDateTime CcSystem::getUpTime()
{
  timespec stClocktime;
  clock_gettime(CLOCK_MONOTONIC, &stClocktime);
  uint64 iMS = static_cast<uint64>(stClocktime.tv_nsec)/1000;
  uint64 iS  = static_cast<uint64>(stClocktime.tv_sec)*1000000;
  return CcDateTime(iMS + iS);
}

void CcSystem::sleep(uint32 timeoutMs)
{
  usleep(1000 * timeoutMs);
}

const CcDevice& CcSystem::getDevice(EDeviceType Type, const CcString& sName)
{
  switch (Type) {
    case EDeviceType::Led:
    {
      CcString Path("/sys/class/leds/" + sName);
      CcFile ledFolder(Path);
      if(ledFolder.isDir())
      {
        CCNEWTYPE(pLed, CcLinuxLed, Path);
        return m_pPrivate->oDeviceList.append(CcDevice(pLed, EDeviceType::Led)).last();
      }
      break;
    }
    case EDeviceType::GpioPort:
    {
      if(sName == "System")
      {
        const CcDevice& rDevice = m_pPrivate->oDeviceList.getDevice(EDeviceType::GpioPort);
        if(rDevice.isValid())
        {
          return rDevice;
        }
        else
        {
          CCNEWTYPE(pPort, CcLinuxGpioPort);
          return m_pPrivate->oDeviceList.append(CcDevice(pPort, EDeviceType::GpioPort)).last();
        }
      }
    }
    default:
      break;
  }
  return CcDevice::NullDevice;
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
  char cwd[CCOS_DEFAULT_MAX_PATH];
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

CcString CcSystem::getCurrentExecutablePath() const
{
  CcString sRet;
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  if (count >= 0 && count < PATH_MAX) {
      sRet.set(result, count);
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

ISharedMemory* CcSystem::getSharedMemory(const CcString& sName, size_t uiSize)
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

CcStatus CcSystem::loadModule(const CcString& sPath, const IKernel& oKernel)
{
  CCNEWTYPE(pModule, CcLinuxModule);
  CcStatus oStatus(false);
  bool bFound = false;
  for (CcLinuxModule* rModule : m_pPrivate->oModules)
    if (rModule->getName() == sPath)
      bFound = true;
  if (bFound == false)
  {
    oStatus = pModule->loadModule(sPath, oKernel);
    if (oStatus)
    {
      m_pPrivate->oModules.append(pModule);
    }
    else
      CCDELETE(pModule);
  }
  else
  {
    oStatus = true;
    CCDELETE(pModule);
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
  CcStatus oOk(true);
  CCUNUSED(sMessage);
  CCUNUSED(bForce);
  int iResult = reboot(RB_POWER_OFF);
  if(iResult != 0) oOk.setSystemError(errno);
  return oOk;
}

CcStatus CcSystem::restart(const CcString& sMessage, bool bForce)
{
  CcStatus oOk(true);
  CCUNUSED(sMessage);
  CCUNUSED(bForce);
  int iResult = reboot(RB_AUTOBOOT);
  if(iResult != 0) oOk.setSystemError(errno);
  return oOk;
}
