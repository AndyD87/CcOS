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
#include "IModuleBase.h"
#include "CcDevice.h"
#include "CcServiceSystem.h"
#include "Devices/CcBoardSupport.h"

#ifdef LINUX
  #include <unistd.h>
#endif
#ifndef GENERIC
  #include <cstdlib>
#endif

CcKernel            s_oKernel;
IKernel             CcKernel::s_oInterfaceModule;
bool                CcKernel::s_bShutdownInProgress;
CcVersion           CcKernel::s_oKernelVersion(CCOS_VERSION_MAJOR, CCOS_VERSION_MINOR, CCOS_VERSION_PATCH, CCOS_VERSION_BUILD);

class CcKernelPrivate
{
public:
  CcKernelPrivate() :
    addDevice(CcKernel::addDevice),
    removeDevice(CcKernel::removeDevice),
    opNew(malloc),
    opDel(free),
    oMemoryInterface({
      CcMemoryMonitor::remove,
      CcMemoryMonitor::insert
    })
  {}

  const CcVersion& getVersion()
  {
    return s_oKernel.getVersion();
  }


  //---------------------- Static Barrier start ( now new operation allowed before System
  static CcKernelPrivate*       pInstance;
  const CcDevice& (*addDevice)(const CcDevice& Device); //!< Pointer to CcKernel::addDevice
  bool (*removeDevice)(const CcDevice& Device);         //!< Pointer to CcKernel::removeDevice
  void*(*opNew)(size_t uiSize);                         //!< Pointer to new operator in Kernel space
  void (*opDel)(void*) = nullptr;                       //!< Pointer to delete operator in Kernel space
  CcMemoryMonitor::SInterface   oMemoryInterface;
  CcSystem                      oSystem;
  //---------------------- Static Barrier end

  CcServiceSystem               oServiceSystem;
  CcAppList                     m_AppList;
  CcDeviceList                  m_DeviceList;
  CcDriverLoad                  m_oDriverList;
  CcEventHandler                m_oInputEventHandler;
  CcEventHandler                m_oShutdownHandler;
  CcEventHandleMap<EDeviceType> m_oDeviceEventHandler;
  bool                          m_SystemStarted = false;
  bool                          m_bInitialized  = false;
  CcVector<void(*)(void)>       m_oExitFunctions;
  #ifdef DEBUG
    bool                        m_bDebug = true;
  #else
    bool                        m_bDebug = false;
  #endif

};

CcKernelPrivate*    CcKernelPrivate::pInstance(nullptr);

#ifdef GENERIC
/**
 * @brief This method is available for systems without main or without any
 *        option to implement main.
 *        CcKernel_Start can be called from such systems to create a call to main.
 */
void CcKernel_Start()
{
  main(0,nullptr);
}
#endif
CcKernel::CcKernel()
{
  CcKernel::s_bShutdownInProgress = false;
  #ifndef GENERIC
    // Setup system
    atexit(CcKernel::shutdown);
  #endif
  CCNEW(m_pPrivate, CcKernelPrivate);
  if (CcKernelPrivate::pInstance == nullptr) 
    CcKernelPrivate::pInstance = m_pPrivate;
  CcMemoryMonitor::init();
  #ifdef MEMORYMONITOR_CHECK_KERNEL
    CcMemoryMonitor::enable();
  #endif

  CcConsole::init();
  CcFileSystem::init();

  // Start drivers
  CcKernelPrivate::pInstance->m_oDriverList.init(0);
  CcKernelPrivate::pInstance->oSystem.init();
  CcKernelPrivate::pInstance->m_oDriverList.init(1);
  CcKernelPrivate::pInstance->m_oDriverList.init(2);
  CcKernelPrivate::pInstance->m_oDriverList.init(3);

  // To ensure that user is running in separate context initialize userspace just now.
  CcMemoryManager::initUserSpace();
  CcKernelPrivate::pInstance->m_bInitialized = true;
}

CcKernel::~CcKernel()
{
  if (s_bShutdownInProgress == false)
  {
    // Shutdown will be done by OS`s atexit function wich will call shutdown
    // Just on generic we have to call it here.
    shutdown();

    #ifdef MEMORYMONITOR_CHECK_KERNEL
      // Wait for workers to be ended
    #ifdef WINDOWS
      Sleep(20);
    #elif defined(LINUX)
      usleep(20000);
    #endif
      if (CcMemoryMonitor::getAllocationCount())
      {
        CCDEBUG("Buffer left");
        CcStdOut oOut;
        CcMemoryMonitor::printLeft(static_cast<IIo*>(&oOut));
        exit(-1);
      }
      // Wait for all io is realy done and shutdown is realy complete
    #ifdef WINDOWS
      Sleep(20);
    #elif defined(LINUX)
      usleep(20000);
    #endif
      CcMemoryMonitor::disable();
    #endif
    CcMemoryMonitor::deinit();
  }
  CCDELETE(m_pPrivate);
}

void CcKernel::delayMs(uint32 uiDelay)
{
  CcKernelPrivate::pInstance->oSystem.sleep(uiDelay);
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
  return CcKernelPrivate::pInstance->oSystem.initGUI();
}

bool CcKernel::initCLI()
{
  return CcKernelPrivate::pInstance->oSystem.initCLI();
}

bool CcKernel::deinitCLI()
{
  return CcKernelPrivate::pInstance->oSystem.deinitCLI();
}

bool CcKernel::isAdmin()
{
  return CcKernelPrivate::pInstance->oSystem.isAdmin();
}

void CcKernel::stop()
{
  CcKernelPrivate::pInstance->m_oShutdownHandler.call(nullptr);
  CcKernelPrivate::pInstance->oServiceSystem.stop();
}

void CcKernel::shutdown()
{
  if (s_bShutdownInProgress == false)
  {
    s_bShutdownInProgress = true;
    if (CcKernelPrivate::pInstance->m_bInitialized)
    {
      CcKernelPrivate::pInstance->m_bInitialized = false;

      // Stop all applications
      stop();

      CcKernelPrivate::pInstance->m_oDriverList.deinit();
      CcKernelPrivate::pInstance->m_oDeviceEventHandler.clear();
      CcKernelPrivate::pInstance->m_DeviceList.clear();

      while (CcKernelPrivate::pInstance->m_oExitFunctions.size() > 0)
      {
        void(*fExitFunction)(void) = CcKernelPrivate::pInstance->m_oExitFunctions[0];
        CcKernelPrivate::pInstance->m_oExitFunctions.remove(0);
        fExitFunction();
      }

      CcKernelPrivate::pInstance->oSystem.deinit();
      CcFileSystem::deinit();

      CCDEBUG("Kernel shutdown complete\r\n");
      CcConsole::deinit();
    }
    CcKernelPrivate::pInstance = nullptr;
  }
  else
  {
    CCDEBUG("CcKernel::shutdown called twice");
  }
}

void CcKernel::terminate()
{
  CCDEBUG("Kernel terminate");
  exit(-1);
}

void CcKernel::addApp(const CcAppHandle& hApplication)
{
  CcKernelPrivate::pInstance->m_AppList.append(hApplication);
}

const CcAppList &CcKernel::getAppList()
{
  return CcKernelPrivate::pInstance->m_AppList;
}

bool CcKernel::createThread(IThread &Thread)
{
  return CcKernelPrivate::pInstance->oSystem.createThread(Thread);
}

bool CcKernel::createProcess(CcProcess &processToStart)
{
  CCDEBUG("CcKernel::createProcess");
  bool bKernelAppFound = false;
  for (size_t i = 0; i < CcKernelPrivate::pInstance->m_AppList.size(); i++)
  {
    if (CcKernelPrivate::pInstance->m_AppList.at(i).getApp()->getName() == processToStart.getApplication())
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
    return CcKernelPrivate::pInstance->oSystem.createProcess(processToStart);
  }
}

CcEventHandler& CcKernel::getInputEventHandler()
{
  return CcKernelPrivate::pInstance->m_oInputEventHandler;
}

void CcKernel::emitInputEvent(CcInputEvent& InputEvent)
{
  CcKernelPrivate::pInstance->m_oInputEventHandler.call(&InputEvent);
}

CcSystem& CcKernel::getSystem()
{
  return CcKernelPrivate::pInstance->oSystem;
}

CcServiceSystem& CcKernel::getServiceSystem()
{
  return CcKernelPrivate::pInstance->oServiceSystem;
}

CcDateTime CcKernel::getDateTime()
{
  return CcKernelPrivate::pInstance->oSystem.getDateTime();
}

CcDateTime CcKernel::getUpTime()
{
  return CcKernelPrivate::pInstance->oSystem.getUpTime();
}

CcUserList CcKernel::getUserList()
{
  return CcKernelPrivate::pInstance->oSystem.getUserList();
}

CcGroupList CcKernel::getGroupList()
{
  return CcKernelPrivate::pInstance->oSystem.getGroupList();
}

void CcKernel::setDebug(bool bOnOff)
{
  CcKernelPrivate::pInstance->m_bDebug = bOnOff;
}

bool CcKernel::getDebug()
{
  return CcKernelPrivate::pInstance->m_bDebug;
}

const CcDevice& CcKernel::getDevice(EDeviceType eType, size_t nr)
{
  CcDevice& oHandle = CcKernelPrivate::pInstance->m_DeviceList.getDevice(eType, nr);
  if (oHandle.isValid() == false && eType != EDeviceType::BoardSupport)
  {
    CcBoardSupport oBoardSupport = CcKernel::getDevice(EDeviceType::BoardSupport);
    if(oBoardSupport.isValid())
    {
      oHandle = oBoardSupport.getDevice()->createDevice(eType, nr);
    }
  }
  if (oHandle.isValid() == false)
  {
    CcKernelPrivate::pInstance->m_oDriverList.load(eType);
    oHandle = CcKernelPrivate::pInstance->m_DeviceList.getDevice(eType, nr);
  }
  return oHandle;
}

CcDeviceList CcKernel::getDevices(EDeviceType Type)
{
  CcDeviceList oDeviceList;
  for(CcDevice& oHandle : CcKernelPrivate::pInstance->m_DeviceList)
  {
    if(oHandle.getType() == Type)
    {
      oDeviceList.append(oHandle);
    }
  }
  return oDeviceList;
}

const CcDevice& CcKernel::getDevice(EDeviceType Type, const CcString& Name)
{
  // @todo: because name devices are only in System no kernel request is done
  return CcKernelPrivate::pInstance->oSystem.getDevice(Type, Name);
}

IKernel& CcKernel::getInterface()
{
  return s_oInterfaceModule;
}

void CcKernel::setInterface(IKernel* pInterface)
{
  if(pInterface)
  {
    if (pInterface->pContext->getVersion() == getVersion())
    {
      CcKernelPrivate::pInstance = pInterface->pContext;
    }
    else
      CCERROR("Unable to load interface, KernelVersion not matching");
  }
  else
  {
    CcKernelPrivate::pInstance = nullptr;
  }
}

const CcDevice& CcKernel::addDevice(const CcDevice& Device)
{
  CcKernelPrivate::pInstance->m_DeviceList.lock();
  CcKernelPrivate::pInstance->m_DeviceList.append(Device);
  const CcDevice& oDevice = CcKernelPrivate::pInstance->m_DeviceList.last();
  for (CcPair<EDeviceType, CcEvent>& oEntry : CcKernelPrivate::pInstance->m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.getValue().call(Device.ptr());
    }
  }
  CcKernelPrivate::pInstance->m_DeviceList.unlock();
  return oDevice;
}

bool CcKernel::removeDevice(const CcDevice& Device)
{
  for (CcPair<EDeviceType, CcEvent>& oEntry : CcKernelPrivate::pInstance->m_oDeviceEventHandler)
  {
    if (oEntry.getKey() == Device.getType())
    {
      oEntry.getValue().call(Device.ptr());
    }
  }
  return CcKernelPrivate::pInstance->m_DeviceList.removeItem(Device);
}

const CcDeviceList &CcKernel::getDeviceList()
{
  return CcKernelPrivate::pInstance->m_DeviceList;
}

ISocket* CcKernel::getSocket(ESocketType eType)
{
  // @todo create a networkmanager for socket managment.
  return CcKernelPrivate::pInstance->oSystem.getSocket(eType);
}

INetworkStack* CcKernel::getNetworkStack()
{
  return CcKernelPrivate::pInstance->oSystem.getNetworkStack();
}

ISharedMemory* CcKernel::getSharedMemory(const CcString& sName, size_t uiSize)
{
  return CcKernelPrivate::pInstance->oSystem.getSharedMemory(sName, uiSize);
}

CcStringMap CcKernel::getEnvironmentVariables()
{
  return CcKernelPrivate::pInstance->oSystem.getEnvironmentVariables();
}

CcString CcKernel::getEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::pInstance->oSystem.getEnvironmentVariable(sName);
}

bool CcKernel::getEnvironmentVariableExists(const CcString& sName)
{
  return CcKernelPrivate::pInstance->oSystem.getEnvironmentVariableExists(sName);
}

bool CcKernel::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  return CcKernelPrivate::pInstance->oSystem.setEnvironmentVariable(sName, sValue);
}

bool CcKernel::removeEnvironmentVariable(const CcString& sName)
{
  return CcKernelPrivate::pInstance->oSystem.removeEnvironmentVariable(sName);
}

void CcKernel::registerAtExit(void(*fAtExit)(void))
{
  CcKernelPrivate::pInstance->m_oExitFunctions.append(fAtExit);
}

void CcKernel::deregisterAtExit(void(*fAtExit)(void))
{
  CcKernelPrivate::pInstance->m_oExitFunctions.removeItem(fAtExit);
}

void CcKernel::registerOnDevicePnpEvent(EDeviceType eType, const CcEvent& pEventHandle)
{
  CcKernelPrivate::pInstance->m_oDeviceEventHandler.append(eType, pEventHandle);
}

void CcKernel::deregisterOnDevicePnpEvent(EDeviceType eType, CcObject* pHandler)
{
  CcKernelPrivate::pInstance->m_oDeviceEventHandler.removeObject(eType, pHandler);
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

void CcKernel::addShutdownHandler(const CcEvent& oEvent)
{
  if(CcKernelPrivate::pInstance)
    CcKernelPrivate::pInstance->m_oShutdownHandler.append(oEvent);
}

void CcKernel::removeShutdownHandler(CcObject* pObject)
{
  if(CcKernelPrivate::pInstance)
    CcKernelPrivate::pInstance->m_oShutdownHandler.removeObject(pObject);
}

const CcVersion& CcKernel::getVersion()
{
  return s_oKernelVersion;
}

CcString CcKernel::getConfigDir()
{
  return CcKernelPrivate::pInstance->oSystem.getConfigDir();
}

CcString CcKernel::getDataDir()
{
  return CcKernelPrivate::pInstance->oSystem.getDataDir();
}

CcString CcKernel::getBinaryDir()
{
  return CcKernelPrivate::pInstance->oSystem.getBinaryDir();
}

CcString CcKernel::getWorkingDir()
{
  return CcKernelPrivate::pInstance->oSystem.getWorkingDir();
}

CcString CcKernel::getTempDir()
{
  return CcKernelPrivate::pInstance->oSystem.getTemporaryDir();
}

CcString CcKernel::getUserDir()
{
  return CcKernelPrivate::pInstance->oSystem.getUserDir();
}

CcString CcKernel::getUserDataDir()
{
  return CcKernelPrivate::pInstance->oSystem.getUserDataDir();
}

CcString CcKernel::getCurrentExecutablePath()
{
  return CcKernelPrivate::pInstance->oSystem.getCurrentExecutablePath();
}

CcStatus CcKernel::setWorkingDir(const CcString& sPath)
{
  return CcKernelPrivate::pInstance->oSystem.setWorkingDir(sPath);
}

CcStatus CcKernel::loadModule(const CcString& sPath)
{
  return CcKernelPrivate::pInstance->oSystem.loadModule(sPath, getInterface());
}

void CcKernel::message(EMessage eType)
{
  switch(eType)
  {
    case EMessage::Warning:
      CcKernelPrivate::pInstance->oSystem.warning();
      break;
    case EMessage::Error:
      CcKernelPrivate::pInstance->oSystem.error();
#ifdef DEBUG
      // Stuck on error!
      while(1);
#endif
      break;
    case EMessage::Info:
    case EMessage::Debug:
    case EMessage::Verbose:
    default:
      break;
  }
}

void CcKernel::message(EMessage eType, const CcString& sMessage)
{
  if(!s_bShutdownInProgress)
  {
    switch(eType)
    {
      case EMessage::Warning:
        CcConsole::writeLine(CcLog::formatWarningMessage(sMessage));
        message(eType);
        break;
      case EMessage::Error:
        CcConsole::writeLine(CcLog::formatErrorMessage(sMessage));
        message(eType);
        break;
      case EMessage::Info:
        CcConsole::writeLine(CcLog::formatInfoMessage(sMessage));
        message(eType);
        break;
      case EMessage::Debug:
        CcConsole::writeLine(CcLog::formatDebugMessage(sMessage));
        message(eType);
        break;
      case EMessage::Verbose:
        CcConsole::writeLine(CcLog::formatVerboseMessage(sMessage));
        message(eType);
        break;
    }
  }
}

void CcKernel::forceBreakpoint()
{
  #if defined(__i386__) || defined(__x86_64__)
    __asm__ volatile("int $0x03");
  #elif defined(__thumb__)
    __asm__ volatile(".inst 0xde01");
  #elif defined(__arm__) && !defined(__thumb__)
    __asm__ volatile(".inst 0xe7f001f0");
  #elif defined(__aarch64__) && defined(__APPLE__)
    __builtin_debugtrap();
  #elif defined(__aarch64__)
    trap_instruction();
  #elif defined(__riscv)
    __asm__ volatile(".4byte 0x00100073");
  #elif defined(_MSC_VER)
    __debugbreak();
  #else
    bool bBreak = 0;
    while(bBreak == 0)
      CcKernel::sleep(1);
  #endif
}

/**
 * @brief Overloaded new method for CcOS to track allocations and
 *        to share buffer regions over modules
 * @param uiSize: Number of bytes to allocate
 * @return Pointer to allocated buffer or nullptr if error
 */
void* operator new(std::size_t uiSize) CCTHROW_BAD_ALLOC
{
  if (CcKernelPrivate::pInstance)
    return CcKernelPrivate::pInstance->opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

/**
 * @brief Overloaded new method for CcOS to track allocations and
 *        to share buffer regions over modules
 * @param uiSize: Number of bytes to allocate
 * @return Pointer to allocated buffer or nullptr if error
 */
void* operator new[](std::size_t uiSize) CCTHROW_BAD_ALLOC
{
  if (CcKernelPrivate::pInstance)
    return CcKernelPrivate::pInstance->opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 */
void operator delete(void* pBuffer) NOEXCEPT
{
  if (CcKernelPrivate::pInstance)
    CcKernelPrivate::pInstance->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 */
void operator delete[](void* pBuffer) NOEXCEPT
{
  if (CcKernelPrivate::pInstance)
    CcKernelPrivate::pInstance->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

// Do not on mingw
#if defined(WINDOWS) && !defined(__GNUC__)
/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 * @param uiSize:  Ignored param, pBuffer will be deleted completly
 */
void operator delete(void* pBuffer, size_t uiSize) NOEXCEPT
{
  CCUNUSED(uiSize);
  if (CcKernelPrivate::pInstance)
    CcKernelPrivate::pInstance->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 * @param uiSize:  Ignored param, pBuffer will be deleted completly
 */
void operator delete[](void* pBuffer, size_t uiSize) NOEXCEPT
{
  CCUNUSED(uiSize);
  if (CcKernelPrivate::pInstance)
    CcKernelPrivate::pInstance->opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}
#endif
