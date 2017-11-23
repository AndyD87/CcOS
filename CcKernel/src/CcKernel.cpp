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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcKernel
 * @todo : - implement security attributes to avoid manipulation for Kernel Devices and Filesystem
 */

#include "CcKernel.h"
#include "CcVersion.h"
#include "CcOSVersion.h"
#include "CcString.h"
#include "CcSystem.h"
#include "CcProcess.h"
#include "CcDateTime.h"
#include "Driver/CcDriverLoad.h"
#include "CcFileSystem.h"
#include "CcGroupList.h"
#include "CcUserList.h"
#include "CcHandle.h"
#include "CcEventHandler.h"
#include "CcInputEvent.h"
#include "CcDevice.h"
#include "CcApp.h"
#include "CcThreadManager.h"
#include "CcLog.h"
#include "CcFileSystem.h"
#include "CcDeviceList.h"
#include "CcAppList.h"
#include "CcMapCommon.h"

class CcKernelPrivate
{
public:
  static void init();
  static void deinit();
  static CcVersion m_oKernelVersion;
  static CcSystem* m_System;      //!< Pointer to System wich is getting initialized when Kernel is created
  static time_t m_SystemTime;           //!< System Time in UTC
  static CcEventHandler m_EventHandler; //!< Object Handler with all Event-Receiver
  static int    m_argc;                 //!< Count of Startup Parameters todo: replace with StringList
  static char **m_argv;                 //!< Startup parameters todo: replace with StringList
  static bool   m_SystemStarted;        //!< Check if Target-System is started
  static bool   m_bDebug;               //!< Set Debug-Mode on for debug messages
  static CcAppList            m_AppList;       //!< Applications currently registered to Kernel
  static CcThreadManager      m_Threads;       //!< Managing all created Threads
  static CcDeviceList         m_DeviceList;    //!< List of Devices registered to Kernel for lowlevel access
  static CcLog                m_Log;           //!< Log-Manager to handle Kernel-Output messages
  static CcUserList           m_UserList;      //!< List of Users available on System
  static bool                 m_bUserListReceived;
  static CcGroupList          m_GroupList;     //!< List of Users available on System
  static bool                 m_bGroupListReceived;
};
CcVersion CcKernelPrivate::m_oKernelVersion(CCOS_VERSION_MAJOR, CCOS_VERSION_MINOR, CCOS_VERSION_PATCH, CCOS_VERSION_BUILD);

CcSystem*           CcKernelPrivate::m_System      = nullptr;
time_t              CcKernelPrivate::m_SystemTime  = 0;
int                 CcKernelPrivate::m_argc        = 0;
char**              CcKernelPrivate::m_argv        = nullptr;
bool                CcKernelPrivate::m_SystemStarted = false;
#ifdef DEBUG
bool                CcKernelPrivate::m_bDebug = true;
#else
bool                CcKernelPrivate::m_bDebug = false;
#endif
CcAppList           CcKernelPrivate::m_AppList;
CcThreadManager     CcKernelPrivate::m_Threads;
CcDeviceList        CcKernelPrivate::m_DeviceList;
bool                CcKernelPrivate::m_bUserListReceived = false;
CcUserList          CcKernelPrivate::m_UserList;
bool                CcKernelPrivate::m_bGroupListReceived = false;
CcGroupList          CcKernelPrivate::m_GroupList;
CcEventHandler      CcKernelPrivate::m_EventHandler;
CcKernel            CcKernel::Kernel;

void CcKernelPrivate::init()
{
  CcKernelPrivate::m_System = new CcSystem();
  CCMONITORNEW(CcKernelPrivate::m_System);
  CcKernelPrivate::m_System->init();
}

void CcKernelPrivate::deinit()
{
  if (m_System != nullptr)
  {
    CCMONITORDELETE(m_System);
    delete m_System;
    m_System = nullptr;
  }
}

CcKernel::CcKernel()
{
#ifdef MEMORYMONITOR_ENABLED
  CcMemoryMonitor::initLists();
#endif
  CcKernelPrivate::init();
  init();
}

CcKernel::~CcKernel() 
{
  CcKernelPrivate::deinit();
}

void CcKernel::init(void)
{
  CcDriverLoad::init();
#ifdef MEMORYMONITOR_ENABLED
  // MemoryMonitor requires Threads from System to start it's thread
  CcMemoryMonitor::initThread();
#endif
}

void CcKernel::delayMs(uint32 uiDelay)
{
  CcKernelPrivate::m_System->sleep(uiDelay);
}

void CcKernel::delayS(uint32 uiDelay)
{
  delayMs(uiDelay * 1000);
}


bool CcKernel::initGUI()
{
  return CcKernelPrivate::m_System->initGUI();
}

bool CcKernel::initCLI()
{
  return CcKernelPrivate::m_System->initCLI();
}

int CcKernel::initService()
{
  return CcKernelPrivate::m_System->initService();
}

void CcKernel::addApp(const CcAppHandle& hApplication)
{
  CcKernelPrivate::m_AppList.append(hApplication);
}

const CcAppList &CcKernel::getAppList(void)
{
  return CcKernelPrivate::m_AppList;
}

void CcKernel::systemTick( void )
{
  CcKernelPrivate::m_SystemTime+=10;
  //for(uint32 i=0; i<m_oTimerCallbackList.size(); i++)
  //{
  //  if(m_oTimerCallbackList[i].time == m_SystemTime)
  //  {
  //    m_oTimerCallbackList[i].OH.call();
  //    m_oTimerCallbackList.remove(i);
  //  }
  //}
}

bool CcKernel::createThread(CcThreadObject &Thread)
{
  if (CcKernelPrivate::m_System->createThread(Thread))
  {
    CcKernelPrivate::m_Threads.addThread(Thread);
    return true;
  }
  return false;
}

bool CcKernel::createProcess(CcProcess &processToStart)
{
  bool bKernelAppFound = false;
  for (size_t i = 0; i < CcKernelPrivate::m_AppList.size(); i++)
  {
    if (CcKernelPrivate::m_AppList.at(i).getApp()->getName() == processToStart.getApplication())
    {
      // TODO: Implementation of dynamic apploadings
      //CcApp* app = m_AppList.at(i));
      //createThread(*app);
      bKernelAppFound = true;
    }
  }
  if (bKernelAppFound)
  {
    return false;
  }
  else{
    return CcKernelPrivate::m_System->createProcess(processToStart);
  }
}

CcEventHandler& CcKernel::getInputEventHandler()
{
  return CcKernelPrivate::m_EventHandler;
}

void CcKernel::emitEvent(CcInputEvent& InputEvent)
{
  CcKernelPrivate::m_EventHandler.call(&InputEvent);
}

const CcSystem& CcKernel::getSystem(void)
{
  return *CcKernelPrivate::m_System;
}

CcDateTime CcKernel::getDateTime(void)
{
  return CcKernelPrivate::m_System->getDateTime();
}

const CcUserList& CcKernel::getUserList()
{
  if (CcKernelPrivate::m_bUserListReceived == false)
  {
    CcKernelPrivate::m_UserList = CcKernelPrivate::m_System->getUserList();
    CcKernelPrivate::m_bUserListReceived = true;
  }
  return CcKernelPrivate::m_UserList;
}

const CcGroupList& CcKernel::getGroupList()
{
  if (CcKernelPrivate::m_bGroupListReceived == false)
  {
    CcKernelPrivate::m_GroupList = CcKernelPrivate::m_System->getGroupList();
    CcKernelPrivate::m_bGroupListReceived = true;
  }
  return CcKernelPrivate::m_GroupList;
}

void CcKernel::setArg(int argc, char **argv)
{
  if (CcKernelPrivate::m_argv == nullptr)
  {
    CCMONITORDELETE(CcKernelPrivate::m_argv); delete CcKernelPrivate::m_argv;
  }
  CcKernelPrivate::m_argc = argc;
  CcKernelPrivate::m_argv = argv;
}

void CcKernel::setDebug(bool bOnOff)
{
  CcKernelPrivate::m_bDebug = bOnOff;
}

bool CcKernel::getDebug()
{
  return CcKernelPrivate::m_bDebug;
}

CcDeviceHandle CcKernel::getDevice(EDeviceType Type, uint16 nr)
{
  CcDeviceHandle cRet;
  // because nummerated devices are only in Kernel no system is requested
  cRet = CcKernelPrivate::m_DeviceList.getDevice(Type, nr);
  return cRet;
}

CcDeviceHandle CcKernel::getDevice(EDeviceType Type, const CcString& Name)
{
  CcDeviceHandle cRet;
  // @todo: because name devices are only in System no kernel request is done 
  cRet = CcKernelPrivate::m_System->getDevice(Type, Name);
  return cRet;
}

void CcKernel::addDevice(CcDeviceHandle Device, EDeviceType Type)
{
  CcKernelPrivate::m_DeviceList.append(Type, Device);
}

const CcDeviceList &CcKernel::getDeviceList(void)
{
  return CcKernelPrivate::m_DeviceList;
}

CcSocketAbstract* CcKernel::getSocket(ESocketType eType)
{
  // @todo create a networkmanager for socket managment.
  return CcKernelPrivate::m_System->getSocket(eType);
}

CcSharedMemoryAbstract* CcKernel::getSharedMemory(const CcString& sName, size_t uiSize)
{
  return CcKernelPrivate::m_System->getSharedMemory(sName, uiSize);
}

CcStringMap CcKernel::getEnvironmentVariables()
{
  if (CcKernelPrivate::m_System != nullptr)
  {
    return CcKernelPrivate::m_System->getEnvironmentVariables();
  }
  return CcStringMap();
}

CcString CcKernel::getEnvironmentVariable(const CcString& sName)
{
  if (CcKernelPrivate::m_System != nullptr)
  {
    return CcKernelPrivate::m_System->getEnvironmentVariable(sName);
  }
  return CcString();
}

bool CcKernel::getEnvironmentVariableExists(const CcString& sName)
{
  if (CcKernelPrivate::m_System != nullptr)
  {
    return CcKernelPrivate::m_System->getEnvironmentVariableExists(sName);
  }
  return false;
}

bool CcKernel::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  if (CcKernelPrivate::m_System != nullptr)
  {
    return CcKernelPrivate::m_System->setEnvironmentVariable(sName, sValue);
  }
  return false;
}

bool CcKernel::removeEnvironmentVariable(const CcString& sName)
{
  if (CcKernelPrivate::m_System != nullptr)
  {
    return CcKernelPrivate::m_System->removeEnvironmentVariable(sName);
  }
  return false;
}

CcString CcKernel::getConfigDir()
{
  return CcKernelPrivate::m_System->getConfigDir();
}

CcString CcKernel::getDataDir()
{
  return CcKernelPrivate::m_System->getDataDir();
}

CcString CcKernel::getBinaryDir()
{
  return CcKernelPrivate::m_System->getBinaryDir();
}

CcString CcKernel::getWorkingDir(void)
{
  return CcKernelPrivate::m_System->getWorkingDir();
}

CcString CcKernel::getTempDir()
{
  return CcKernelPrivate::m_System->getTemporaryDir();
}

CcString CcKernel::getUserDir()
{
  return CcKernelPrivate::m_System->getUserDir();
}

CcString CcKernel::getUserDataDir()
{
  return CcKernelPrivate::m_System->getUserDataDir();
}

const CcVersion& CcKernel::getVersion()
{
  return CcKernelPrivate::m_oKernelVersion;
}
