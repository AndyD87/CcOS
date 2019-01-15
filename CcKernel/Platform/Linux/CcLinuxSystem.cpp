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
#include "CcStringList.h"
#include "CcGroupList.h"
#include "CcLinuxTimer.h"
#include "CcLinuxTouch.h"
#include "CcLinuxFile.h"
#include "CcLinuxFilesystem.h"
#include "CcLinuxSocketTcp.h"
#include "CcLinuxSocketUdp.h"
#include "CcLinuxLed.h"
#include "CcLinuxGPIOPort.h"
#include "CcLinuxProcessThread.h"
#include "CcLinuxUser.h"
#include "CcLinuxPipe.h"
#include "CcLinuxSharedMemory.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcUserList.h"
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/stat.h>
#include <csignal>
#include <cstdlib>
#include "CcMapCommon.h"
#include "CcStringUtil.h"


class CcSystemPrivate
{
public:
  void initSystem(void);
  void initTimer(void );
  void initDisplay(void );
  void initTouch(void );

  void systemTick(void );

  CcFileSystemAbstract *m_Filesystem;
  CcDeviceList m_cDeviceList;

  static CcStringMap m_oEnvValues;
};
CcStringMap CcSystemPrivate::m_oEnvValues;

void CcSystemSignalHanlder(int s)
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
  m_pPrivateData = new CcSystemPrivate();
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = CcSystemSignalHanlder;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  if( 0 == sigaction(SIGINT, &sigIntHandler, NULL))
  {
    CCVERBOSE("SIGINT handler successfully set");
    if( 0 == sigaction(SIGABRT, &sigIntHandler, NULL))
    {
      CCVERBOSE("SIGABRT handler successfully set");
      if( 0 == sigaction(SIGTERM, &sigIntHandler, NULL))
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
  for( ; 0 < m_pPrivateData->m_cDeviceList.size(); )
  {
    m_pPrivateData->m_cDeviceList.remove(0);
  }
  delete m_pPrivateData;
}

void CcSystem::init(void)
{
  signal(SIGPIPE, SIG_IGN);
  m_pPrivateData->initSystem();
}

bool CcSystem::initGUI(void)
{
  m_pPrivateData->initDisplay();
  m_pPrivateData->initTouch();
  return true;
}

bool CcSystem::initCLI(void)
{
  return false;
}

int CcSystem::initService()
{
  return fork();
}

void CcSystemPrivate::initSystem(void)
{
  CcFileSystem::init();
  m_Filesystem = new CcLinuxFilesystem();
  CcFileSystem::addMountPoint("/", m_Filesystem);
}

void CcSystemPrivate::initDisplay(void )
{
#if (CCOS_GUI > 0)
  m_Display = new CcX11SubSystem(500, 500);
  CcKernel::addDevice(m_Display, EDeviceType::Display);
  m_Display->open();
#endif
}

void CcSystemPrivate::initTouch(void )
{
}

void CcSystemPrivate::systemTick(void )
{
  CcKernel::systemTick();
}

void *threadFunction(void *Param)
{
  CcThreadObject *pThreadObject = static_cast<CcThreadObject *>(Param);
  if (pThreadObject->getThreadState() == EThreadState::Starting)
  {
    pThreadObject->enterState(EThreadState::Running);
    pThreadObject->run();
    pThreadObject->enterState(EThreadState::Stopped);
    pThreadObject->onStopped();
  }
  else
  {
    // Do net create threads wich are not in starting state
    pThreadObject->enterState(EThreadState::Stopped);
  }
  return 0;
}

bool CcSystem::createThread(CcThreadObject& Object)
{
  pthread_t threadId;
  int iErr = pthread_create(&threadId, 0, threadFunction, (void*)&Object);
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
  CcLinuxPipe* pPipe = new CcLinuxPipe();
  oProcessToStart.setPipe(pPipe);
  CcLinuxProcessThread* pWorker = new CcLinuxProcessThread(oProcessToStart);
  CCMONITORNEW(pWorker);
  pWorker->start();
  oProcessToStart.setThreadHandle(pWorker);
  return true;
}

CcSocketAbstract* CcSystem::getSocket(ESocketType type)
{
  CcSocketAbstract* pNewSocket;
  switch(type)
  {
    case ESocketType::TCP:
      pNewSocket = new CcLinuxSocketTcp();
      break;
    case ESocketType::UDP:
      pNewSocket = new CcLinuxSocketUdp();
      break;
    default:
      pNewSocket = nullptr;
      break;
  }
  return pNewSocket;
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
      CcString sKey(pcCurrent, pcFound-pcCurrent);
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
  if(CcSystemPrivate::m_oEnvValues.containsKey(sName))
  {
    CcString& rSetString = CcSystemPrivate::m_oEnvValues[sName];
    rSetString = sSetString;
    char* pcNewValue = rSetString.getCharString();
    iResult = putenv(pcNewValue);
  }
  else
  {
    CcSystemPrivate::m_oEnvValues.append(sName, sSetString);
    CcString& rSetString = CcSystemPrivate::m_oEnvValues.last().value();
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
    CcSystemPrivate::m_oEnvValues.removeKey(sName);
    return true;
  }
  return false;
}

CcDateTime CcSystem::getDateTime(void )
{
  timespec stClocktime;
  clock_gettime(CLOCK_REALTIME, &stClocktime);
  int64 iMS = static_cast<int64>(stClocktime.tv_nsec)/1000;
  int64 iS  = static_cast<int64>(stClocktime.tv_sec)*1000000;
  return CcDateTime(iMS + iS);
}

void CcSystem::sleep(uint32 timeoutMs)
{
  usleep(1000 * timeoutMs);
}

CcHandle<CcDevice> CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{
  CcHandle<CcDevice> pRet = NULL;
  switch (Type) {
    case EDeviceType::Led:
    {
      CcString Path("/sys/class/leds/" + Name);
      CcFile ledFolder(Path);
      if(ledFolder.isDir())
      {
        pRet = new CcLinuxLed(Path);
        m_pPrivateData->m_cDeviceList.append(EDeviceType::Led, pRet);
      }
      break;
    }
    case EDeviceType::GPIOPort:
    {
      if(Name == "System")
      {
        pRet = m_pPrivateData->m_cDeviceList.getDevice(EDeviceType::GPIOPort);
        if(pRet == NULL)
        {
          pRet = new CcLinuxGPIOPort();
          m_pPrivateData->m_cDeviceList.append(EDeviceType::GPIOPort, pRet);
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

CcString CcSystem::getWorkingDir(void) const
{
  CcString sRet;
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {
    sRet = cwd;
  }
  return sRet;
}

CcString CcSystem::getTemporaryDir(void) const
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

CcUserList CcSystem::getUserList(void)
{
  CcUserList UserList;
  passwd *retTemp;
  retTemp = getpwuid(getuid());
  CcString sCurUser(retTemp->pw_name);
  do
  {
    retTemp = getpwent();
    if(retTemp != NULL)
    {
      CcString sUsername(retTemp->pw_name);
      CcString sHomeDir(retTemp->pw_dir);
      if(sUsername != sCurUser)
      {
        CcLinuxUser *newUser=new CcLinuxUser(sUsername, sHomeDir, retTemp->pw_uid, false);
        UserList.append(newUser);
      }
      else
      {
        CcLinuxUser *newUser=new CcLinuxUser(sUsername, sHomeDir, retTemp->pw_uid, true);
        UserList.append(newUser);
        UserList.setCurrentUser(sCurUser);
      }
    }
  } while(retTemp != NULL);
  endpwent();
  return UserList;
}

CcSharedMemoryAbstract* CcSystem::getSharedMemory(const CcString &sName, size_t uiSize)
{
  return static_cast<CcSharedMemoryAbstract*>(new CcLinuxSharedMemory(sName, uiSize));
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
