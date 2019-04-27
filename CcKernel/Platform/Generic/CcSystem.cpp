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
#include "CcMutex.h"
#include "CcGenericFilesystem.h"
#include "Network/CcNetworkStack.h"
#include "CcList.h"

class CcSystem::CPrivate
{
public:
  CPrivate()
  {
    s_pInstance = this;
    pCpu = CcKernel::getDevice(EDeviceType::Cpu).cast<ICpu>();
    pCpu->setSystemTick(CcSystem::CPrivate::tick);
    pCpu->setThreadTick(CcSystem::CPrivate::changeThread);
    pCurrentThreadContext = pCpu->mainThread();
  }

  static void tick()
  {
    s_pInstance->uiUpTime += 1000;
    s_pInstance->uiThreadCount++;
    if(s_pInstance->uiThreadCount >= 10)
    {
      changeThread();
    }
  }

  static void changeThread()
  {
    if(s_pInstance->s_pInstance->oThreadLock == false)
    {
      s_pInstance->s_pInstance->oThreadLock = true;
      s_pInstance->s_pInstance->uiThreadCount = 0;
      if(s_pInstance->pCurrentThreadContext != nullptr)
      {
        if(s_pInstance->pCurrentThreadContext->pThreadObject->getThreadState() != EThreadState::Stopped)
        {
          s_pInstance->oThreads.append(s_pInstance->pCurrentThreadContext);
        }
        else
        {
          s_pInstance->pCpu->deleteThread(s_pInstance->pCurrentThreadContext);
        }
      }
      if(s_pInstance->oThreads.size())
      {
        s_pInstance->pCurrentThreadContext = s_pInstance->oThreads[0];
        s_pInstance->oThreads.remove(0);
        s_pInstance->pCpu->loadThread(s_pInstance->pCurrentThreadContext);
      }
      s_pInstance->s_pInstance->oThreadLock = false;
    }
  }

  void appendThread(IThread* pThread)
  {
    CcThreadContext* pThreadContext = pCpu->createThread(pThread);
    pThreadContext->pThreadObject->enterState(EThreadState::Starting);
    oThreads.prepend(pThreadContext);
  }

  volatile uint64           uiUpTime = 0;
  volatile uint64           uiThreadCount = 0;
  CcStringMap               oEnvVars;
  CcGenericFilesystem       oFileSystem;
  CcHandle<ICpu>            pCpu;
  CcList<CcThreadContext*>  oThreads;
  bool                      oThreadLock;
  CcThreadContext*          pCurrentThreadContext = nullptr;
CcNetworkStack*             pNetworkStack = nullptr;
private:
  static CcSystem::CPrivate* s_pInstance;
};

CcSystem::CPrivate* CcSystem::CPrivate::s_pInstance = nullptr;

CcSystem::CcSystem()
{
  m_pPrivateData = new CcSystem::CPrivate();
  CCMONITORNEW(m_pPrivateData);
}

CcSystem::~CcSystem()
{
  CCDELETE(m_pPrivateData);
}

void CcSystem::init()
{
  m_pPrivateData->pNetworkStack = new CcNetworkStack();
  m_pPrivateData->pNetworkStack->initDefaults();
  CcFileSystem::init();
  CcFileSystem::addMountPoint("/", &m_pPrivateData->oFileSystem);
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
  return CcDateTime(m_pPrivateData->uiUpTime);
}

void CcSystem::sleep(uint32 timeoutMs)
{
  uint64 uiSystemTime(m_pPrivateData->uiUpTime);
  uiSystemTime += (timeoutMs*1000);
  // do it at least one times
  do
  {
    m_pPrivateData->pCpu->nextThread();
  } while(uiSystemTime > m_pPrivateData->uiUpTime);
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{ 
  CCUNUSED(Type); 
  CCUNUSED(Name); 
  return nullptr; 
}

CcStringMap CcSystem::getEnvironmentVariables() const
{
  return m_pPrivateData->oEnvVars;
}

CcString CcSystem::getEnvironmentVariable(const CcString& sName) const
{
  return m_pPrivateData->oEnvVars[sName];
}

bool CcSystem::getEnvironmentVariableExists(const CcString& sName) const
{
  return m_pPrivateData->oEnvVars.containsKey(sName);
}

bool CcSystem::removeEnvironmentVariable(const CcString& sName)
{
  return m_pPrivateData->oEnvVars.removeKey(sName);
}

bool CcSystem::setEnvironmentVariable(const CcString& sName, const CcString& sValue)
{
  if(m_pPrivateData->oEnvVars.containsKey(sName))
  {
    m_pPrivateData->oEnvVars[sName] = sValue;
  }
  else
  {
    m_pPrivateData->oEnvVars.add(sName, sValue);
  }
  return true;
}

ISocket* CcSystem::getSocket(ESocketType eType)
{
  if(m_pPrivateData->pNetworkStack != nullptr)
    return m_pPrivateData->pNetworkStack->getSocket(eType);
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
