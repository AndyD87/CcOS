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
#include "CcLog.h"
#include "CcDeviceList.h"
#include "CcAppList.h"
#include "CcMapCommon.h"
#include "CcConsole.h"
#include "CcMemoryMonitor.h"
#include "CcMemoryManager.h"
#include "CcStdOut.h"
#include "IKernel.h"
#include "CcVersion.h"

#ifdef LINUX
#include <unistd.h>
#endif

class CcKernelPrivate
{
public:
  CcKernelPrivate() :
    m_oKernelVersion(CCOS_VERSION_MAJOR, CCOS_VERSION_MINOR, CCOS_VERSION_PATCH, CCOS_VERSION_BUILD),
    addDevice(CcKernel::addDevice),
    removeDevice(CcKernel::removeDevice),
    opNew(malloc),
    opDel(free),
    oMemoryInterface({
    #ifdef MEMORYMONITOR_ENABLED
      CcMemoryMonitor::remove,
      CcMemoryMonitor::insert
    #else
      nullptr,
      nullptr
    #endif
      }),
    m_oInterfaceModule(this)
  {}
  CcVersion                     m_oKernelVersion;
  CcSystem*                     pSystem = nullptr;
  void (*addDevice)(CcDeviceHandle Device);     //!< Pointer to CcKernel::addDevice
  void (*removeDevice)(CcDeviceHandle Device);  //!< Pointer to CcKernel::removeDevice
  void*(*opNew)(size_t uiSize);                 //!< Pointer to new operator in Kernel space
  void (*opDel)(void*) = nullptr;               //!< Pointer to delete operator in Kernel space
  CcMemoryMonitor::SInterface   oMemoryInterface;
  CcAppList                     m_AppList;
  CcDeviceList                  m_DeviceList;
  CcDriverLoad                  m_oDriverList;
  CcEventHandler                m_oInputEventHandler;
  CcEventHandler                m_oShutdownHandler;
  CcEventHandleMap<EDeviceType> m_oDeviceEventHandler;
  IKernel                       m_oInterfaceModule;
  bool                          m_SystemStarted = false;
  bool                          m_bInitialized  = false;
  #ifdef DEBUG
    bool                        m_bDebug = true;
  #else
    bool                        m_bDebug = false;
  #endif

  static CcKernelPrivate  oInstance;
  static CcKernelPrivate* pPrivate;
};

CcKernelPrivate     CcKernelPrivate::oInstance;
CcKernel            CcKernel::s_oKernel;
CcKernelPrivate*    CcKernelPrivate::pPrivate(nullptr);

#ifdef GENERIC
void CcKernel_Start()
{
  main(0,nullptr);
}
#endif

CcKernel::CcKernel()
{
  CcKernelPrivate::pPrivate = &CcKernelPrivate::oInstance;
#ifdef MEMORYMONITOR_ENABLED
  CcMemoryMonitor::init();
  #ifdef MEMORYMONITOR_CHECK_KERNEL
    CcMemoryMonitor::enable();
  #endif
#endif
  // Setup system
  CCNEW(CcKernelPrivate::oInstance.pSystem, CcSystem);

  // Initialize static classes
  CcConsole::init();
  CcFileSystem::init();

  // Start drivers
  CcKernelPrivate::oInstance.m_oDriverList.init(0);
  CcKernelPrivate::oInstance.pSystem->init();
  CcKernelPrivate::oInstance.m_oDriverList.init(1);
  CcKernelPrivate::oInstance.m_oDriverList.init(2);
  CcKernelPrivate::oInstance.m_oDriverList.init(3);

  // To ensure that user is running in separate context initialize userspace just now.
  CcMemoryManager::initUserSpace();
  CcKernelPrivate::oInstance.m_bInitialized = true;
}

CcKernel::~CcKernel()
{
  shutdown();

  if (CcKernelPrivate::oInstance.pSystem)
  {
    CCDELETE(CcKernelPrivate::oInstance.pSystem);
  }
  else
  {
    CCDEBUG("Error");
  }
#ifdef MEMORYMONITOR_ENABLED
#ifdef MEMORYMONITOR_CHECK_KERNEL
  // Wait for workers to be ended
  #ifdef WINDOWS
    sleep(20);
  #elif defined(LINUX)
    usleep(20000);
  #endif
  if (CcMemoryMonitor::getAllocationCount())
  {
    CcStdOut* pOut = CcConsole::getOutStream();
    if (pOut == nullptr)
    {
      pOut = CcConsole::getOutStream();
    }
    CcMemoryMonitor::printLeft(static_cast<IIo*>(pOut));
    exit(-1);
  }
  // Wait for all io is realy done and shutdown is realy complete
#ifdef WINDOWS
  sleep(20);
#elif defined(LINUX)
  usleep(20000);
#endif
  CcMemoryMonitor::disable();
#endif
  CcMemoryMonitor::deinit();
#endif
  CcKernelPrivate::pPrivate = nullptr;
}

void CcKernel::delayMs(uint32 uiDelay)
{
  CcSystem* pSystem = CcKernelPrivate::pPrivate->pSystem;
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
  return CcKernelPrivate::pPrivate->pSystem->initGUI();
}

bool CcKernel::initCLI()
{
  return CcKernelPrivate::pPrivate->pSystem->initCLI();
}

bool CcKernel::deinitCLI()
{
  return CcKernelPrivate::pPrivate->pSystem->deinitCLI();
}

int CcKernel::initService()
{
  return CcKernelPrivate::pPrivate->pSystem->initService();
}

bool CcKernel::isAdmin()
{
  return CcKernelPrivate::pPrivate->pSystem->isAdmin();
}

void CcKernel::shutdown()
{
  if (CcKernelPrivate::oInstance.m_bInitialized)
  {
    CcKernelPrivate::oInstance.m_bInitialized = false;
    CcKernelPrivate::oInstance.m_oShutdownHandler.call(nullptr);
    CcKernelPrivate::oInstance.m_oDriverList.deinit();
    CcKernelPrivate::oInstance.m_oDeviceEventHandler.clear();
    CcKernelPrivate::oInstance.m_DeviceList.clear();

    CcKernelPrivate::oInstance.pSystem->deinit();

    CcFileSystem::deinit();
    CcConsole::deinit();
  }
}

void CcKernel::addApp(const CcAppHandle& hApplication)
{
  CcKernelPrivate::pPrivate->m_AppList.append(hApplication);
}

const CcAppList &CcKernel::getAppList()
{
  return CcKernelPrivate::pPrivate->m_AppList;
}

bool CcKernel::createThread(IThread &Thread)
{
  if (CcKernelPrivate::pPrivate->pSystem)
  {
    if (CcKernelPrivate::pPrivate->pSystem->createThread(Thread))
    {
      return true;
    }
  }
  return false;
}

bool CcKernel::createProcess(CcProcess &processToStart)
{
  bool bKernelAppFound = false;
  for (size_t i = 0; i < CcKernelPrivate::pPrivate->m_AppList.size(); i++)
  {
    if (CcKernelPrivate::pPrivate->m_AppList.at(i).getApp()->getName() == processToStart.getApplication())
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
    return CcKernelPrivate::pPrivate->pSystem->createProcess(processToStart);
  }
}

CcEventHandler& CcKernel::getInputEventHandler()
{
  return CcKernelPrivate::pPrivate->m_oInputEventHandler;
}

void CcKernel::emitInputEvent(CcInputEvent& InputEvent)
{
  CcKernelPrivate::pPrivate->m_oInputEventHandler.call(&InputEvent);
}

CcSystem& CcKernel::getSystem()
{
  return *CcKernelPrivate::pPrivate->pSystem;
}

CcDateTime CcKernel::getDateTime()
{
  return CcKernelPrivate::pPrivate->pSystem->getDateTime();
}

CcDateTime CcKernel::getUpTime()
{
  return CcKernelPrivate::pPrivate->pSystem->getUpTime();
}

CcUserList CcKernel::getUserList()
{
  return CcKernelPrivate::pPrivate->pSystem->getUserList();
}

CcGroupList CcKernel::getGroupList()
{
  return CcKernelPrivate::pPrivate->pSystem->getGroupList();
}

void CcKernel::setDebug(bool bOnOff)
{
  CcKernelPrivate::pPrivate->m_bDebug = bOnOff;
}

bool CcKernel::getDebug()
{
  return CcKernelPrivate::pPrivate->m_bDebug;
}

CcDeviceHandle CcKernel::getDevice(EDeviceType Type, size_t nr)
{
#ifdef GENERIC
  // because nummerated devices are only in Kernel no system is requested
  return CcKernelPrivate::pPrivate->m_DeviceList.getDevice(Type, nr);
#else
  CcDeviceHandle oHandle = CcKernelPrivate::pPrivate->m_DeviceList.getDevice(Type, nr);
  if (oHandle.isValid() == false)
  {
    oHandle = CcKernelPrivate::pPrivate->pSystem->getDevice(Type, nr);
  }
  if (oHandle.isValid() == false)
  {
    CcKernelPrivate::pPrivate->m_oDriverList.load(Type);
    oHandle = CcKernelPrivate::pPrivate->m_DeviceList.getDevice(Type, nr);
  }
  return oHandle;
#endif
}

CcDeviceList CcKernel::getDevices(EDeviceType Type)
{
  CcDeviceList oDeviceList;
  for(CcDeviceHandle& oHandle : CcKernelPrivate::pPrivate->m_DeviceList)
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
  cRet = CcKernelPrivate::pPrivate->pSystem->getDevice(Type, Name);
  return cRet;
}

const IKernel& CcKernel::getInterface()
{
  return CcKernelPrivate::oInstance.m_oInterfaceModule;
}

void CcKernel::setInterface(IKernel* pInterface)
{
  if(pInterface)
  {
    CcKernelPrivate::pPrivate = pInterface->pContext;
  }
  else
  {
    CcKernelPrivate::pPrivate = &CcKernelPrivate::oInstance;
  }
  #ifdef MEMORYMONITOR_ENABLED
    CcMemoryMonitor::setInterface(&CcKernelPrivate::pPrivate->oMemoryInterface);
  #endif
}

void CcKernel::addDevice(CcDeviceHandle Device)
{
  CcKernelPrivate::pPrivate->m_DeviceList.append(Device);
  for (CcPair<EDeviceType, CcEvent>& oEntry : CcKernelPrivate::pPrivate->m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.value().call(Device.ptr());
    }
  }
}

void CcKernel::removeDevice(CcDeviceHandle Device)
{
  for (CcPair<EDeviceType, CcEvent>& oEntry : CcKernelPrivate::pPrivate->m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.value().call(Device.ptr());
    }
  }
  CcKernelPrivate::pPrivate->m_DeviceList.removeItem(Device);
}

const CcDeviceList &CcKernel::getDeviceList()
{
  return CcKernelPrivate::pPrivate->m_DeviceList;
}

ISocket* CcKernel::getSocket(ESocketType eType)
{
  // @todo create a networkmanager for socket managment.
  return CcKernelPrivate::pPrivate->pSystem->getSocket(eType);
}

INetworkStack* CcKernel::getNetworkStack()
{
  return CcKernelPrivate::pPrivate->pSystem->getNetworkStack();
}

ISharedMemory* CcKernel::getSharedMemory(const CcString& sName, size_t uiSize)
{
  return CcKernelPrivate::pPrivate->pSystem->getSharedMemory(sName, uiSize);
}

CcStringMap CcKernel::getEnvironmentVariables()
{
  return CcKernelPrivate::pPrivate->pSystem->getEnvironmentVariables();
}

CcString CcKernel::getEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::pPrivate->pSystem->getEnvironmentVariable(sName);
}

bool CcKernel::getEnvironmentVariableExists(const CcString& sName)
{
  return CcKernelPrivate::pPrivate->pSystem->getEnvironmentVariableExists(sName);
}

bool CcKernel::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  return CcKernelPrivate::pPrivate->pSystem->setEnvironmentVariable(sName, sValue);
}

bool CcKernel::removeEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::pPrivate->pSystem->removeEnvironmentVariable(sName);
}

void CcKernel::registerOnDevicePnpEvent(EDeviceType eType, CcEvent pEventHandle)
{
  CcKernelPrivate::pPrivate->m_oDeviceEventHandler.append(eType, pEventHandle);
}

void CcKernel::deregisterOnDevicePnpEvent(EDeviceType eType, CcObject* pHandler)
{
  CcKernelPrivate::pPrivate->m_oDeviceEventHandler.removeObject(eType, pHandler);
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
  return CcKernelPrivate::pPrivate->m_oShutdownHandler;
}

const CcVersion& CcKernel::getVersion()
{
  return CcKernelPrivate::pPrivate->m_oKernelVersion;
}

CcString CcKernel::getConfigDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getConfigDir();
}

CcString CcKernel::getDataDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getDataDir();
}

CcString CcKernel::getBinaryDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getBinaryDir();
}

CcString CcKernel::getWorkingDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getWorkingDir();
}

CcString CcKernel::getTempDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getTemporaryDir();
}

CcString CcKernel::getUserDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getUserDir();
}

CcString CcKernel::getUserDataDir()
{
  return CcKernelPrivate::pPrivate->pSystem->getUserDataDir();
}

CcStatus CcKernel::setWorkingDir(const CcString& sPath)
{
  return CcKernelPrivate::pPrivate->pSystem->setWorkingDir(sPath);
}

CcStatus CcKernel::loadModule(const CcString& sPath)
{
  return CcKernelPrivate::pPrivate->pSystem->loadModule(sPath, getInterface());
}

void CcKernel::message(EMessage eType)
{
  switch(eType)
  {
    case EMessage::Warning:
      CcKernelPrivate::pPrivate->pSystem->warning();
      break;
    case EMessage::Error:
    default:
      CcKernelPrivate::pPrivate->pSystem->error();
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
  if (CcKernelPrivate::pPrivate)
    return CcKernelPrivate::pPrivate->opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

void* operator new[](std::size_t uiSize) _GLIBCXX_THROW(std::bad_alloc)
{
  if (CcKernelPrivate::pPrivate)
    return CcKernelPrivate::pPrivate->opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

void operator delete(void* pBuffer) _GLIBCXX_USE_NOEXCEPT
{
  if (CcKernelPrivate::pPrivate)
    CcKernelPrivate::pPrivate->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete[](void* pBuffer) _GLIBCXX_USE_NOEXCEPT
{
  if (CcKernelPrivate::pPrivate)
    CcKernelPrivate::pPrivate->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

// Do not on mingw
#if defined(WINDOWS) && !defined(__GNUC__)
void operator delete(void* pBuffer, size_t uiSize) _GLIBCXX_USE_NOEXCEPT
{
  CCUNUSED(uiSize);
  if (CcKernelPrivate::pPrivate)
    CcKernelPrivate::pPrivate->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete[](void* pBuffer, size_t uiSize) _GLIBCXX_USE_NOEXCEPT
{
  CCUNUSED(uiSize);
  if (CcKernelPrivate::pPrivate)
    CcKernelPrivate::pPrivate->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}
#endif

