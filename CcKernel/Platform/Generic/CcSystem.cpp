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
 * @brief     Class CcSystem
 **/

#include "CcSystem.h"
#include "CcString.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcDateTime.h"
#include "CcFileSystem.h"
#include "IDevice.h"
#include "Network/CcSocket.h"
#include "CcGlobalStrings.h"
#include "CcUserList.h"
#include "CcObject.h"
#include "CcThreadManager.h"
#include "Devices/ITimer.h"
#include "Devices/ICpu.h"
#include "CcThreadContext.h"
#include "CcFileSystem.h"
#include "CcGenericFilesystem.h"

class CcSystemPrivate : public CcObject
{
public:
  CcSystemPrivate()
  {
    m_pCpu = CcKernel::getDevice(EDeviceType::Cpu).cast<ICpu>();
    if(m_pCpu != nullptr)
    {
      m_pCpu->setSystemTick(CcSystemPrivate::SystemTick);
      pCurrentThreadContext = m_pCpu->mainThread();
    }
  }

  static void SystemTick()
  {
    uiUpTime += 1000;
    uiThreadCount++;
    if(uiThreadCount >= 10)
    {
      ThreadTick();
    }
  }

  static void ThreadTick()
  {
    uiThreadCount = 0;
    if(pCurrentThreadContext != nullptr)
    {
      if(pCurrentThreadContext->pThreadObject->getThreadState() != EThreadState::Stopped)
      {
        m_oThreads.append(pCurrentThreadContext);
      }
      else
      {
        m_pCpu->deleteThread(pCurrentThreadContext);
      }
    }
    if(m_oThreads.size())
    {
      pCurrentThreadContext = m_oThreads[0];
      m_oThreads.remove(0);
      m_pCpu->loadThread(pCurrentThreadContext);
    }
  }

  void appendThread(IThread* pThread)
  {
    CcThreadContext* pThreadContext = m_pCpu->createThread(pThread);
    pThreadContext->pThreadObject->enterState(EThreadState::Starting);
    m_oThreads.prepend(pThreadContext);
  }

  static volatile uint64 uiUpTime;
  static volatile uint64 uiThreadCount;
  static CcStringMap oEnvVars;
  static CcGenericFilesystem oFileSystem;
  static CcHandle<ICpu> m_pCpu;
  static CcList<CcThreadContext*> m_oThreads;
  static CcThreadContext* pCurrentThreadContext;
};

CcStringMap CcSystemPrivate::oEnvVars;
volatile uint64 CcSystemPrivate::uiUpTime = 0;
volatile uint64 CcSystemPrivate::uiThreadCount = 0;
CcGenericFilesystem CcSystemPrivate::oFileSystem;
CcHandle<ICpu> CcSystemPrivate::m_pCpu = nullptr;
CcList<CcThreadContext*> CcSystemPrivate::m_oThreads;
CcThreadContext* CcSystemPrivate::pCurrentThreadContext = nullptr;

CcSystem::CcSystem()
{
  m_pPrivateData = new CcSystemPrivate();
  CcFileSystem::init();
  CcFileSystem::addMountPoint("/", &m_pPrivateData->oFileSystem);
}

CcSystem::~CcSystem()
{
  delete m_pPrivateData;
}

void CcSystem::init()
{
}

bool CcSystem::initGUI()
{
  return false; // we do not have a gui on a generic system
}

bool CcSystem::initCLI()
{
  return false; // we do not have a cli on a generic system
}

int CcSystem::initService()
{
  return 1;
}

bool CcSystem::createThread(IThread &oThread)
{
  m_pPrivateData->appendThread(&oThread);
  return false;
}

bool CcSystem::createProcess(CcProcess &oProcessToStart)
{
  CCUNUSED(oProcessToStart);
  return false;
}

CcDateTime CcSystem::getDateTime()
{
  return CcDateTime(CcSystemPrivate::uiUpTime);
}

#include "Devices/ILed.h"

void CcSystem::sleep(uint32 timeoutMs)
{
  uint64 uiSystemTime(CcSystemPrivate::uiUpTime);
  uiSystemTime += (timeoutMs*1000);
  while(uiSystemTime > CcSystemPrivate::uiUpTime)
  {
    // @todo Switch thread
  }
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{ 
  CCUNUSED(Type); 
  CCUNUSED(Name); 
  return nullptr; 
}

CcStringMap CcSystem::getEnvironmentVariables() const
{
  return CcSystemPrivate::oEnvVars;
}

CcString CcSystem::getEnvironmentVariable(const CcString& sName) const
{
  return CcSystemPrivate::oEnvVars[sName];
}

bool CcSystem::getEnvironmentVariableExists(const CcString& sName) const
{
  return CcSystemPrivate::oEnvVars.containsKey(sName);
}

bool CcSystem::removeEnvironmentVariable(const CcString& sName)
{
  return CcSystemPrivate::oEnvVars.removeKey(sName);
}

bool CcSystem::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  if(CcSystemPrivate::oEnvVars.containsKey(sName))
  {
    CcSystemPrivate::oEnvVars[sName] = sValue;
  }
  else
  {
    CcSystemPrivate::oEnvVars.add(sName, sValue);
  }
  return true;
}


ISocket* CcSystem::getSocket(ESocketType type)
{
  CCUNUSED(type);
  return nullptr;
}

CcString CcSystem::getConfigDir() const
{
  return CcGlobalStrings::Seperators::Slash;
}

CcString CcSystem::getDataDir() const
{
  return CcGlobalStrings::Seperators::Slash;
}

CcString CcSystem::getBinaryDir() const
{
  return CcGlobalStrings::Seperators::Slash;
}

CcString CcSystem::getWorkingDir() const
{
  return CcGlobalStrings::Seperators::Slash;
}

CcString CcSystem::getTemporaryDir() const
{
  return CcGlobalStrings::Seperators::Slash;
}

CcString CcSystem::getUserDir() const
{
  return CcGlobalStrings::Empty;
}

CcString CcSystem::getUserDataDir() const
{
  return CcGlobalStrings::Empty;
}

CcUserList CcSystem::getUserList()
{
  CcUserList UserList;
  return UserList;
}

ISharedMemory* CcSystem::getSharedMemory(const CcString& sName, size_t uiSize)
{
  CCUNUSED(sName);
  CCUNUSED(uiSize);
  return nullptr;
}

CcGroupList CcSystem::getGroupList()
{
  return CcGroupList();
}
