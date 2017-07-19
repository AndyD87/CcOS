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
#include "CcString.h"
#include "CcSystem.h"
#include "CcProcess.h"
#include "CcDateTime.h"
#include "Driver/CcDriverLoad.h"
#include "CcFileSystem.h"

EKernelState        CcKernel::s_eState      = EKernelState::Initializing;
CcSystem*           CcKernel::m_System      = nullptr;
time_t              CcKernel::m_SystemTime  = 0;
int                 CcKernel::m_argc        = 0;
char**              CcKernel::m_argv        = nullptr;
bool                CcKernel::m_SystemStarted = false;
#ifdef DEBUG
bool                CcKernel::m_bDebug = true;
#else
bool                CcKernel::m_bDebug = false;
#endif
CcAppList           CcKernel::m_AppList;
CcThreadManager     CcKernel::m_Threads;
CcDeviceList        CcKernel::m_DeviceList;
CcUserList          CcKernel::m_UserList;
CcEventHandler      CcKernel::m_EventHandler;
CcKernel            CcKernel::Kernel;

CcKernel::CcKernel()
{
#ifdef MEMORYMONITOR_ENABLED
  CcMemoryMonitor::initLists();
#endif
  init();
}

CcKernel::~CcKernel() 
{
  s_eState = EKernelState::Stopped;
}

void CcKernel::init(void)
{
  m_System = new CcSystem();
  CCMONITORNEW(m_System);
  m_System->init();
  m_UserList = m_System->getUserList();

  CcDriverLoad::init();
#ifdef MEMORYMONITOR_ENABLED
  // MemoryMonitor requires Threads from System to start it's thread
  CcMemoryMonitor::initThread();
#endif
}

void CcKernel::start(void)
{
  if (s_eState < EKernelState::Starting)
  {
    s_eState = EKernelState::Starting;
    if (m_SystemStarted == false && m_System != nullptr)
    {
      s_eState = EKernelState::Running;
      m_SystemStarted = true;
      m_System->start();
    }
    CcKernel::stop();
  }
}

void CcKernel::stop(void)
{
  if (s_eState < EKernelState::Stopping)
  {
    s_eState = EKernelState::Stopping;
    m_AppList.clear();
    m_Threads.closeAll();
    m_System->stop();
    CCMONITORDELETE(m_System); 
    delete m_System;
#ifdef MEMORYMONITOR_ENABLED
    CcMemoryMonitor::deinit();
#endif
  }
}

void CcKernel::systemReady()
{

}

void CcKernel::delayMs(uint32 uiDelay)
{
  m_System->sleep(uiDelay);
}

void CcKernel::delayS(uint32 uiDelay)
{
  delayMs(uiDelay * 1000);
}


bool CcKernel::initGUI()
{
  return m_System->initGUI();
}

bool CcKernel::initCLI()
{
  return m_System->initCLI();
}

int CcKernel::initService()
{
  return m_System->initService();
}

void CcKernel::addApp(const CcAppHandle& hApplication)
{
  m_AppList.append(hApplication);
}

const CcAppList &CcKernel::getAppList(void)
{
  return m_AppList;
}

void CcKernel::systemTick( void )
{
  m_SystemTime+=10;
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
  if (m_System->createThread(Thread))
  {
    m_Threads.addThread(Thread);
    return true;
  }
  return false;
}

bool CcKernel::createProcess(CcProcess &processToStart)
{
  bool bKernelAppFound = false;
  for (size_t i = 0; i < m_AppList.size(); i++)
  {
    if (m_AppList.at(i).getApp()->getName() == processToStart.getApplication())
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
    return m_System->createProcess(processToStart);
  }
}

CcEventHandler& CcKernel::getInputEventHandler()
{
  return m_EventHandler;
}

void CcKernel::emitEvent(CcInputEvent& InputEvent)
{
  m_EventHandler.call(&InputEvent);
}

const CcSystem& CcKernel::getSystem(void)
{
  return *m_System;
}

CcDateTime CcKernel::getDateTime(void)
{
  return m_System->getDateTime();
}

const CcUserList& CcKernel::getUserList()
{
  return m_UserList;
}

void CcKernel::setArg(int argc, char **argv)
{
  CCMONITORDELETE(m_argv); delete m_argv;
  m_argc = argc;
  m_argv = argv;
}

void CcKernel::setDebug(bool bOnOff)
{
  m_bDebug = bOnOff;
}

bool CcKernel::getDebug()
{
  return m_bDebug;
}

CcHandle<CcDevice> CcKernel::getDevice(EDeviceType Type, uint16 nr)
{
  CcHandle<CcDevice> cRet;
  // because nummerated devices are only in Kernel no system is requested
  cRet = m_DeviceList.getDevice(Type, nr);
  return cRet;
}

CcHandle<CcDevice> CcKernel::getDevice(EDeviceType Type, const CcString& Name)
{
  CcHandle<CcDevice> cRet;
  // @todo: because name devices are only in System no kernel request is done 
  cRet = m_System->getDevice(Type, Name);
  return cRet;
}

void CcKernel::addDevice(CcHandle<CcDevice> Device, EDeviceType Type)
{
  m_DeviceList.append(Type, Device);
}

const CcDeviceList &CcKernel::getDeviceList(void)
{
  return m_DeviceList;
}

CcSocket* CcKernel::getSocket(ESocketType eType)
{
  // @todo create a networkmanager for socket managment.
  return m_System->getSocket(eType);
}

const CcString& CcKernel::getConfigDir()
{
  return m_System->getConfigDir();
}

const CcString& CcKernel::getDataDir()
{
  return m_System->getDataDir();
}

const CcString& CcKernel::getBinaryDir()
{
  return m_System->getBinaryDir();
}

const CcString& CcKernel::getWorkingDir(void)
{
  return m_System->getWorkingDir();
}

const CcString& CcKernel::getTempDir()
{
  return m_System->getTemporaryDir();
}
