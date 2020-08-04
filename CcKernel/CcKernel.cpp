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
#include "CcDeviceList.h"
#include "CcAppList.h"
#include "CcMapCommon.h"
#include "CcConsole.h"
#include "CcMemoryMonitor.h"
#include "CcMemoryManager.h"
#include "CcStdOut.h"
#include "IKernel.h"

#ifdef LINUX
#include <unistd.h>
#endif

class CcKernelPrivate
{
public:
  static IKernel*             m_pInterface;
  static IKernel              m_oInterfaceModule;
  static CcVersion            m_oKernelVersion;
  static CcSystem*            pSystem;      //!< Pointer to System witch is getting initialized when Kernel is created
  static time_t               m_SystemTime;           //!< System Time in UTC
  static CcEventHandler       m_oInputEventHandler; //!< Object Handler with all Event-Receiver
  static bool                 m_SystemStarted;        //!< Check if Target-System is started
  static bool                 m_bDebug;               //!< Set Debug-Mode on for debug messages
  static CcAppList            m_AppList;       //!< Applications currently registered to Kernel
  static CcDriverLoad         m_oDriverList;   //!< Initialized drivers
  static CcDeviceList         m_DeviceList;    //!< List of Devices registered to Kernel for lowlevel access
  static CcLog                m_Log;           //!< Log-Manager to handle Kernel-Output messages
  static CcEventHandler       m_oShutdownHandler;
  static bool                 m_bInitialized;
  static CcEventHandleMap<EDeviceType> m_oDeviceEventHandler;
};

IKernel*            CcKernelPrivate::m_pInterface(nullptr);
IKernel             CcKernelPrivate::m_oInterfaceModule(
  CcKernel::addDevice,
  CcKernel::removeDevice,
  malloc,
  free,
#ifdef MEMORYMONITOR_ENABLED
  CcMemoryMonitor::remove,
  CcMemoryMonitor::insert
#else
  nullptr,
  nullptr
#endif
);
CcVersion           CcKernelPrivate::m_oKernelVersion(CCOS_VERSION_MAJOR, CCOS_VERSION_MINOR, CCOS_VERSION_PATCH, CCOS_VERSION_BUILD);
CcSystem*           CcKernelPrivate::pSystem      = nullptr;
bool                CcKernelPrivate::m_SystemStarted = false;
#ifdef DEBUG
bool                CcKernelPrivate::m_bDebug = true;
#else
bool                CcKernelPrivate::m_bDebug = false;
#endif
CcAppList           CcKernelPrivate::m_AppList;
CcDeviceList        CcKernelPrivate::m_DeviceList;
CcDriverLoad        CcKernelPrivate::m_oDriverList;
CcEventHandler      CcKernelPrivate::m_oInputEventHandler;
bool                CcKernelPrivate::m_bInitialized = false;
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
#ifdef MEMORYMONITOR_ENABLED
  CcMemoryMonitor::init();
  #ifdef MEMORYMONITOR_CHECK_KERNEL
    CcMemoryMonitor::enable();
  #endif
#endif
  // Setup system
  CCNEW(CcKernelPrivate::pSystem, CcSystem);

  // Initialize static classes
  CcConsole::init();
  CcFileSystem::init();

  // Start drivers
  CcKernelPrivate::m_oDriverList.init(0);
  CcKernelPrivate::pSystem->init();
  CcKernelPrivate::m_oDriverList.init(1);
  CcKernelPrivate::m_oDriverList.init(2);
  CcKernelPrivate::m_oDriverList.init(3);

  // To ensure that user is running in separate context initialize userspace just now.
  CcMemoryManager::initUserSpace();
  CcKernelPrivate::m_bInitialized = true;
}

CcKernel::~CcKernel()
{
  shutdown();

  if (CcKernelPrivate::pSystem)
  {
    CCDELETE(CcKernelPrivate::pSystem);
  }
  else
  {
    CCDEBUG("Error");
  }
#ifdef MEMORYMONITOR_ENABLED
#ifdef MEMORYMONITOR_CHECK_KERNEL
  if (CcMemoryMonitor::getAllocationCount())
  {
    CcStdOut* pOut = CcConsole::getOutStream();
    if (pOut == nullptr)
    {
      pOut = CcConsole::getOutStream();
    }
    CcMemoryMonitor::printLeft(static_cast<IIo*>(pOut));
    // Wait for all io is realy done and shutdown is realy complete
#ifdef WINDOWS
    sleep(10);
#elif defined(LINUX)
    usleep(10000);
#endif
    exit(-1);
  }
  CcMemoryMonitor::disable();
#endif
  CcMemoryMonitor::deinit();
#endif
}

void CcKernel::delayMs(uint32 uiDelay)
{
  CcSystem* pSystem = CcKernelPrivate::pSystem;
  pSystem->sleep(uiDelay);
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
  return CcKernelPrivate::pSystem->initGUI();
}

bool CcKernel::initCLI()
{
  return CcKernelPrivate::pSystem->initCLI();
}

bool CcKernel::deinitCLI()
{
  return CcKernelPrivate::pSystem->deinitCLI();
}

int CcKernel::initService()
{
  return CcKernelPrivate::pSystem->initService();
}

bool CcKernel::isAdmin()
{
  return CcKernelPrivate::pSystem->isAdmin();
}

void CcKernel::shutdown()
{
  if (CcKernelPrivate::m_bInitialized)
  {
    CcKernelPrivate::m_bInitialized = false;
    CcKernelPrivate::m_oShutdownHandler.call(nullptr);
    CcKernelPrivate::m_oDriverList.deinit();
    CcKernelPrivate::m_oDeviceEventHandler.clear();
    CcKernelPrivate::m_DeviceList.clear();

    CcKernelPrivate::pSystem->deinit();

    CcFileSystem::deinit();
    CcConsole::deinit();
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
  if (CcKernelPrivate::pSystem)
  {
    if (CcKernelPrivate::pSystem->createThread(Thread))
    {
      return true;
    }
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
    return CcKernelPrivate::pSystem->createProcess(processToStart);
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
  return *CcKernelPrivate::pSystem;
}

CcDateTime CcKernel::getDateTime()
{
  return CcKernelPrivate::pSystem->getDateTime();
}

CcDateTime CcKernel::getUpTime()
{
  return CcKernelPrivate::pSystem->getUpTime();
}

CcUserList CcKernel::getUserList()
{
  return CcKernelPrivate::pSystem->getUserList();
}

CcGroupList CcKernel::getGroupList()
{
  return CcKernelPrivate::pSystem->getGroupList();
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
#ifdef GENERIC
  // because nummerated devices are only in Kernel no system is requested
  return CcKernelPrivate::m_DeviceList.getDevice(Type, nr);
#else
  CcDeviceHandle oHandle = CcKernelPrivate::m_DeviceList.getDevice(Type, nr);
  if (oHandle.isValid() == false)
  {
    oHandle = CcKernelPrivate::pSystem->getDevice(Type, nr);
  }
  if (oHandle.isValid() == false)
  {
    CcKernelPrivate::m_oDriverList.load(Type);
    oHandle = CcKernelPrivate::m_DeviceList.getDevice(Type, nr);
  }
  return oHandle;
#endif
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
  cRet = CcKernelPrivate::pSystem->getDevice(Type, Name);
  return cRet;
}

const IKernel& CcKernel::getInterface()
{
  return CcKernelPrivate::m_oInterfaceModule;
}

void CcKernel::setInterface(IKernel* pInterface)
{
  CcKernelPrivate::m_pInterface = pInterface;
  #ifdef MEMORYMONITOR_ENABLED
  if(
    pInterface != nullptr &&
    pInterface->pBaseObject != CcKernelPrivate::m_oInterfaceModule.pBaseObject
  )
    CcMemoryMonitor::setKernel(pInterface);
  else
    CcMemoryMonitor::setKernel(nullptr);
  #endif
}

void CcKernel::addDevice(CcDeviceHandle Device)
{
  CcKernelPrivate::m_DeviceList.append(Device);
  for (CcPair<EDeviceType, CcEvent>& oEntry : CcKernelPrivate::m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.value().call(Device.ptr());
    }
  }
}

void CcKernel::removeDevice(CcDeviceHandle Device)
{
  for (CcPair<EDeviceType, CcEvent>& oEntry : CcKernelPrivate::m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.value().call(Device.ptr());
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
  return CcKernelPrivate::pSystem->getSocket(eType);
}

INetworkStack* CcKernel::getNetworkStack()
{
  return CcKernelPrivate::pSystem->getNetworkStack();
}

ISharedMemory* CcKernel::getSharedMemory(const CcString& sName, size_t uiSize)
{
  return CcKernelPrivate::pSystem->getSharedMemory(sName, uiSize);
}

CcStringMap CcKernel::getEnvironmentVariables()
{
  return CcKernelPrivate::pSystem->getEnvironmentVariables();
}

CcString CcKernel::getEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::pSystem->getEnvironmentVariable(sName);
}

bool CcKernel::getEnvironmentVariableExists(const CcString& sName)
{
  return CcKernelPrivate::pSystem->getEnvironmentVariableExists(sName);
}

bool CcKernel::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  return CcKernelPrivate::pSystem->setEnvironmentVariable(sName, sValue);
}

bool CcKernel::removeEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::pSystem->removeEnvironmentVariable(sName);
}

void CcKernel::registerOnDevicePnpEvent(EDeviceType eType, CcEvent pEventHandle)
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
  return CcKernelPrivate::pSystem->getConfigDir();
}

CcString CcKernel::getDataDir()
{
  return CcKernelPrivate::pSystem->getDataDir();
}

CcString CcKernel::getBinaryDir()
{
  return CcKernelPrivate::pSystem->getBinaryDir();
}

CcString CcKernel::getWorkingDir()
{
  return CcKernelPrivate::pSystem->getWorkingDir();
}

CcString CcKernel::getTempDir()
{
  return CcKernelPrivate::pSystem->getTemporaryDir();
}

CcString CcKernel::getUserDir()
{
  return CcKernelPrivate::pSystem->getUserDir();
}

CcString CcKernel::getUserDataDir()
{
  return CcKernelPrivate::pSystem->getUserDataDir();
}

CcStatus CcKernel::setWorkingDir(const CcString& sPath)
{
  return CcKernelPrivate::pSystem->setWorkingDir(sPath);
}

CcStatus CcKernel::loadModule(const CcString& sPath)
{
  return CcKernelPrivate::pSystem->loadModule(sPath, getInterface());
}

void CcKernel::message(EMessage eType)
{
  switch(eType)
  {
    case EMessage::Warning:
      CcKernelPrivate::pSystem->warning();
      break;
    case EMessage::Error:
    default:
      CcKernelPrivate::pSystem->error();
#ifdef DEBUG
      // Stuck on error!
      while(1);
#endif
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

#include <new>

#ifndef _GLIBCXX_THROW
#define _GLIBCXX_THROW(BLAH)
#endif
#ifndef _GLIBCXX_USE_NOEXCEPT
#define _GLIBCXX_USE_NOEXCEPT
#endif

void* operator new(std::size_t uiSize) _GLIBCXX_THROW(std::bad_alloc)
{
  if (CcKernelPrivate::m_pInterface)
    return CcKernelPrivate::m_pInterface->opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

void* operator new[](std::size_t uiSize) _GLIBCXX_THROW(std::bad_alloc)
{
  if (CcKernelPrivate::m_pInterface)
    return CcKernelPrivate::m_pInterface->opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

void operator delete(void* pBuffer) _GLIBCXX_USE_NOEXCEPT
{
  if (CcKernelPrivate::m_pInterface)
    CcKernelPrivate::m_pInterface->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete[](void* pBuffer) _GLIBCXX_USE_NOEXCEPT
{
  if (CcKernelPrivate::m_pInterface)
    CcKernelPrivate::m_pInterface->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

// Do not on mingw
#if defined(WINDOWS) && !defined(__GNUC__)
void operator delete(void* pBuffer, size_t uiSize) _GLIBCXX_USE_NOEXCEPT
{
  CCUNUSED(uiSize);
  if (CcKernelPrivate::m_pInterface)
    CcKernelPrivate::m_pInterface->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete[](void* pBuffer, size_t uiSize) _GLIBCXX_USE_NOEXCEPT
{
  CCUNUSED(uiSize);
  if (CcKernelPrivate::m_pInterface)
    CcKernelPrivate::m_pInterface->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}
#endif

