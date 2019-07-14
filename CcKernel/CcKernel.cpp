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
#include "CcEventHandleMap.h"
#include "CcInputEvent.h"
#include "IDevice.h"
#include "CcApp.h"
#include "CcThreadManager.h"
#include "CcLog.h"
#include "CcFileSystem.h"
#include "CcDeviceList.h"
#include "CcAppList.h"
#include "CcMapCommon.h"
#include "CcConsole.h"
#include "CcMemoryMonitor.h"

class CcKernelPrivate
{
public:
  static CcVersion            m_oKernelVersion;
  static CcSystem*            m_pSystem;      //!< Pointer to System witch is getting initialized when Kernel is created
  static time_t               m_SystemTime;           //!< System Time in UTC
  static CcEventHandler       m_oInputEventHandler; //!< Object Handler with all Event-Receiver
  static bool                 m_SystemStarted;        //!< Check if Target-System is started
  static bool                 m_bDebug;               //!< Set Debug-Mode on for debug messages
  static CcAppList            m_AppList;       //!< Applications currently registered to Kernel
  static CcThreadManager      m_Threads;       //!< Managing all created Threads
  static CcDriverLoad         m_oDriverList;   //!< Initialized drivers
  static CcDeviceList         m_DeviceList;    //!< List of Devices registered to Kernel for lowlevel access
  static CcLog                m_Log;           //!< Log-Manager to handle Kernel-Output messages
  static CcEventHandler       m_oShutdownHandler;
  static bool                 m_bRunning;
  static CcEventHandleMap<EDeviceType> m_oDeviceEventHandler;

};

CcVersion           CcKernelPrivate::m_oKernelVersion(CCOS_VERSION_MAJOR, CCOS_VERSION_MINOR, CCOS_VERSION_PATCH, CCOS_VERSION_BUILD);
CcSystem*           CcKernelPrivate::m_pSystem      = nullptr;
bool                CcKernelPrivate::m_SystemStarted = false;
#ifdef DEBUG
bool                CcKernelPrivate::m_bDebug = true;
#else
bool                CcKernelPrivate::m_bDebug = false;
#endif
CcAppList           CcKernelPrivate::m_AppList;
CcThreadManager     CcKernelPrivate::m_Threads;
CcDeviceList        CcKernelPrivate::m_DeviceList;
CcDriverLoad        CcKernelPrivate::m_oDriverList;
CcEventHandler      CcKernelPrivate::m_oInputEventHandler;
bool                CcKernelPrivate::m_bRunning = false;
CcEventHandler      CcKernelPrivate::m_oShutdownHandler;
CcEventHandleMap<EDeviceType> CcKernelPrivate::m_oDeviceEventHandler;
CcKernel            CcKernel::Kernel;

#ifdef GENERIC
void CcKernel_Start()
{
  main(0,nullptr);
}
#endif

CcKernel::CcKernel()
{
  CcKernelPrivate::m_oDriverList.init(0);
#ifdef MEMORYMONITOR_ENABLED
  CcMemoryMonitor::init();
#endif
  CcKernelPrivate::m_pSystem = new CcSystem();
  CCMONITORNEW(CcKernelPrivate::m_pSystem);
  CcKernelPrivate::m_pSystem->init();
  CcKernelPrivate::m_bRunning = true;
  CcKernelPrivate::m_oDriverList.init(1);
  CcKernelPrivate::m_oDriverList.init(2);
  CcKernelPrivate::m_oDriverList.init(3);
}

#if defined(DEBUG)
char CcKernelSHARED CCCHECKNULL(const void* pData)
{
  if( pData == nullptr)
  {
    CcKernel::message(EMessage::Warning);
    return 0;
  }
  return 1;
}
#endif

CcKernel::~CcKernel() 
{
  shutdown();
  CCDELETE(CcKernelPrivate::m_pSystem);
}

void CcKernel::delayMs(uint32 uiDelay)
{
  CcKernelPrivate::m_pSystem->sleep(uiDelay);
}

void CcKernel::delayS(uint32 uiDelay)
{
  if (uiDelay > 0)
    while (uiDelay > 0)
    {
      delayMs(1000);
      uiDelay--;
    }
  else
    delayMs(0);
}


bool CcKernel::initGUI()
{
  return CcKernelPrivate::m_pSystem->initGUI();
}

bool CcKernel::initCLI()
{
  return CcKernelPrivate::m_pSystem->initCLI();
}

int CcKernel::initService()
{
  return CcKernelPrivate::m_pSystem->initService();
}

void CcKernel::shutdown()
{
  if (CcKernelPrivate::m_bRunning)
  {
    CcKernelPrivate::m_bRunning = false;
    CcKernelPrivate::m_oShutdownHandler.call(nullptr);
    CcKernelPrivate::m_oDriverList.deinit();
  }
}

void CcKernel::addApp(const CcAppHandle& hApplication)
{
  CcKernelPrivate::m_AppList.append(hApplication);
}

const CcAppList &CcKernel::getAppList()
{
  return CcKernelPrivate::m_AppList;
}

bool CcKernel::createThread(IThread &Thread)
{
  if (CcKernelPrivate::m_pSystem->createThread(Thread))
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
    return CcKernelPrivate::m_pSystem->createProcess(processToStart);
  }
}

CcEventHandler& CcKernel::getInputEventHandler()
{
  return CcKernelPrivate::m_oInputEventHandler;
}

void CcKernel::emitInputEvent(CcInputEvent& InputEvent)
{
  CcKernelPrivate::m_oInputEventHandler.call(&InputEvent);
}

CcSystem& CcKernel::getSystem()
{
  return *CcKernelPrivate::m_pSystem;
}

CcDateTime CcKernel::getDateTime()
{
  return CcKernelPrivate::m_pSystem->getDateTime();
}

CcUserList CcKernel::getUserList()
{
  return CcKernelPrivate::m_pSystem->getUserList();
}

CcGroupList CcKernel::getGroupList()
{
  return CcKernelPrivate::m_pSystem->getGroupList();
}

void CcKernel::setDebug(bool bOnOff)
{
  CcKernelPrivate::m_bDebug = bOnOff;
}

bool CcKernel::getDebug()
{
  return CcKernelPrivate::m_bDebug;
}

CcDeviceHandle CcKernel::getDevice(EDeviceType Type, size_t nr)
{
  // because nummerated devices are only in Kernel no system is requested
  return CcKernelPrivate::m_DeviceList.getDevice(Type, nr);
}

CcDeviceList CcKernel::getDevices(EDeviceType Type)
{
  CcDeviceList oDeviceList;
  for(CcDeviceHandle& oHandle : CcKernelPrivate::m_DeviceList)
  {
    if(oHandle.getType() == Type)
    {
      oDeviceList.append(oHandle);
    }
  }
  return oDeviceList;
}

CcDeviceHandle CcKernel::getDevice(EDeviceType Type, const CcString& Name)
{
  CcDeviceHandle cRet;
  // @todo: because name devices are only in System no kernel request is done 
  cRet = CcKernelPrivate::m_pSystem->getDevice(Type, Name);
  return cRet;
}

void CcKernel::addDevice(CcDeviceHandle Device)
{
  CcKernelPrivate::m_DeviceList.append(Device);
  for (CcPair<EDeviceType, IEvent*>& oEntry : CcKernelPrivate::m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.getValue()->call(Device.ptr());
    }
  }
}

void CcKernel::removeDevice(CcDeviceHandle Device)
{
  for (CcPair<EDeviceType, IEvent*>& oEntry : CcKernelPrivate::m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.getValue()->call(Device.ptr());
    }
  }
  CcKernelPrivate::m_DeviceList.removeItem(Device);
}

const CcDeviceList &CcKernel::getDeviceList()
{
  return CcKernelPrivate::m_DeviceList;
}

ISocket* CcKernel::getSocket(ESocketType eType)
{
  // @todo create a networkmanager for socket managment.
  return CcKernelPrivate::m_pSystem->getSocket(eType);
}

ISharedMemory* CcKernel::getSharedMemory(const CcString& sName, size_t uiSize)
{
  return CcKernelPrivate::m_pSystem->getSharedMemory(sName, uiSize);
}

CcStringMap CcKernel::getEnvironmentVariables()
{
  return CcKernelPrivate::m_pSystem->getEnvironmentVariables();
}

CcString CcKernel::getEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::m_pSystem->getEnvironmentVariable(sName);
}

bool CcKernel::getEnvironmentVariableExists(const CcString& sName)
{
  return CcKernelPrivate::m_pSystem->getEnvironmentVariableExists(sName);
}

bool CcKernel::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  return CcKernelPrivate::m_pSystem->setEnvironmentVariable(sName, sValue);
}

bool CcKernel::removeEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::m_pSystem->removeEnvironmentVariable(sName);
}

void CcKernel::registerOnDevicePnpEvent(EDeviceType eType, IEvent* pEventHandle)
{
  CcKernelPrivate::m_oDeviceEventHandler.append(eType, pEventHandle);
}

void CcKernel::deregisterOnDevicePnpEvent(EDeviceType eType, CcObject* pHandler)
{
  CcKernelPrivate::m_oDeviceEventHandler.removeObject(eType, pHandler);
}

EPlatform CcKernel::getPlatform()
{
#ifdef GENERIC
  return EPlatform::Generic;
#elif defined(LINUX)
  return EPlatform::Linux;
#elif defined(WINDOWS)
  return EPlatform::Windows;
#else
  return EPlatform::Unknown;
#endif
}

CcEventHandler& CcKernel::getShutdownHandler()
{
  return CcKernelPrivate::m_oShutdownHandler;
}

const CcVersion& CcKernel::getVersion()
{
  return CcKernelPrivate::m_oKernelVersion;
}

CcString CcKernel::getConfigDir()
{
  return CcKernelPrivate::m_pSystem->getConfigDir();
}

CcString CcKernel::getDataDir()
{
  return CcKernelPrivate::m_pSystem->getDataDir();
}

CcString CcKernel::getBinaryDir()
{
  return CcKernelPrivate::m_pSystem->getBinaryDir();
}

CcString CcKernel::getWorkingDir()
{
  return CcKernelPrivate::m_pSystem->getWorkingDir();
}

CcString CcKernel::getTempDir()
{
  return CcKernelPrivate::m_pSystem->getTemporaryDir();
}

CcString CcKernel::getUserDir()
{
  return CcKernelPrivate::m_pSystem->getUserDir();
}

CcString CcKernel::getUserDataDir()
{
  return CcKernelPrivate::m_pSystem->getUserDataDir();
}

void CcKernel::message(EMessage eType)
{
  switch(eType)
  {
    case EMessage::Warning:
      CcKernelPrivate::m_pSystem->warning();
      break;
    case EMessage::Error:
    default:
      CcKernelPrivate::m_pSystem->error();
#ifdef DEBUG
      // Stuck on error!
      while(1);
#endif
      break;
  }
}

void CcKernel::message(EMessage eType, const CcString& sMessage)
{
  switch(eType)
  {
    case EMessage::Warning:
      CcConsole::writeLine(CcLog::formatWarningMessage(sMessage));
      message(eType);
      break;
    case EMessage::Error:
    default:
      CcConsole::writeLine(CcLog::formatErrorMessage(sMessage));
      message(eType);
      break;
  }
}
