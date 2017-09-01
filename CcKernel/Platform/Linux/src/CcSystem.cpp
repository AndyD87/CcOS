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
 * @file      CcSystem
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcSystem
 */
#include "CcSystem.h"
#include "LinuxTimer.h"
#include "LinuxDisplay.h"
#include "LinuxTouch.h"
#include "LinuxFile.h"
#include "LinuxFilesystem.h"
#include "LinuxSocket.h"
#include "LinuxLed.h"
#include "LinuxGPIOPort.h"
#include "LinuxUser.h"
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
  static CcStringMap m_oEnvValues;
};
CcStringMap CcSystemPrivate::m_oEnvValues;

CcSystem::CcSystem() :
  m_sConfigDir("/etc"),
  m_sDataDir("/var/lib"),
  m_sBinaryDir("/bin")
{
}

CcSystem::~CcSystem()
{
  for( ; 0 < m_cDeviceList.size(); )
  {
    m_cDeviceList.remove(0);
  }
}

void CcSystem::init(void)
{
  signal(SIGPIPE, SIG_IGN);
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {
    m_sWorkingDir = cwd;
  }
  if(!LinuxFile(m_sConfigDir).isDir())
  {
    ::mkdir(  m_sConfigDir.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  if(!LinuxFile(m_sDataDir).isDir())
  {
    ::mkdir(  m_sDataDir.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
  }
  const char* pcTempDirPath = getenv("TMP");
  if (pcTempDirPath != nullptr)
  {
    m_sTempDir = pcTempDirPath;
  }
  else
  {
    const char* pcTempDirPath = getenv("TEMP");
    if (pcTempDirPath != nullptr)
    {
      m_sTempDir = pcTempDirPath;
    }
    else
    {
      m_sTempDir = "/tmp";
      if (!LinuxFile(m_sTempDir).isDir())
      {
        ::mkdir(m_sTempDir.getCharString(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
      }
    }
  }
  initSystem();
}

bool CcSystem::start( void )
{
  m_bSystemState=true;
  //start the main loop
  while(m_bSystemState){
    sleep(1);
  }
  return true;
}

bool CcSystem::stop( void )
{
  m_bSystemState=false;
}

bool CcSystem::initGUI(void)
{
  initDisplay();
  initTouch();
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

void CcSystem::initSystem(void)
{
  CcFileSystem::init();
  m_Filesystem = new LinuxFilesystem();
  CcFileSystem::addMountPoint("/", m_Filesystem);
}

void CcSystem::initDisplay( void )
{
#if (CCOS_GUI > 0)
  m_Display = new LinuxDisplay(500, 500);
  CcKernel::addDevice(m_Display, EDeviceType::Display);
  m_Display->open();
#endif
}

void CcSystem::initTouch( void )
{
}

void CcSystem::systemTick( void )
{
  CcKernel::systemTick();
}

void *threadFunction(void *Param)
{
  CcThreadObject *pThreadObject = static_cast<CcThreadObject *>(Param);
  pThreadObject->enterState(EThreadState::Running);
  pThreadObject->run();
  pThreadObject->enterState(EThreadState::Stopped);
  return 0;
}

bool CcSystem::createThread(CcThreadObject& Object)
{
  pthread_t threadId;
  if (0 == pthread_create(&threadId, 0, threadFunction, (void*)&Object))
    return true;
  else
    return false;
}

bool CcSystem::createProcess(CcProcess& oProcessToStart)
{
  return false;
}

CcSocket* CcSystem::getSocket(ESocketType type)
{
  CcSocket *temp= new LinuxSocket(type);
  return temp;
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
    return true;
  }
  return false;
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

CcDateTime CcSystem::getDateTime( void )
{
  timespec stClocktime;
  clock_gettime(CLOCK_REALTIME, &stClocktime);
  int64 iTime = (static_cast<int64>(stClocktime.tv_nsec)/1000)+(static_cast<int64>(stClocktime.tv_sec)*1000000);
  return CcDateTimeFromSeconds(iTime);
}

void CcSystem::sleep(uint32 timeoutMs){
  usleep(1000 * timeoutMs);
}

CcHandle<CcDevice> CcSystem::getDevice(EDeviceType Type, const CcString& Name){
  CcHandle<CcDevice> pRet = NULL;
  switch (Type) {
    case EDeviceType::Led:
    {
      CcString Path("/sys/class/leds/" + Name);
      CcFile ledFolder(Path);
      if(ledFolder.isDir())
      {
        pRet = new LinuxLed(Path);
        m_cDeviceList.append(EDeviceType::Led, pRet);
      }
      break;
    }
    case EDeviceType::GPIOPort:
    {
      if(Name == "System")
      {
        pRet = m_cDeviceList.getDevice(EDeviceType::GPIOPort);
        if(pRet == NULL)
        {
          pRet = new LinuxGPIOPort();
          m_cDeviceList.append(EDeviceType::GPIOPort, pRet);
        }
      }
    }
    default:
      break;
  }
  return pRet;
}

CcUserList CcSystem::getUserList(void)
{
  CcUserList UserList;
  passwd *retTemp;
  do
  {
    retTemp = getpwent();
    if(retTemp != NULL)
    {
      CcString sUsername(retTemp->pw_name);
      CcString sHomeDir(retTemp->pw_dir);
      CcUser *newUser=new LinuxUser(sUsername, sHomeDir);
      UserList.append(newUser);
    }
  } while(retTemp != NULL);
  retTemp = getpwuid(getuid());
  CcString sCurUser(retTemp->pw_name);
  UserList.setCurrentUser(sCurUser);
  endpwent();
  return UserList;
}
