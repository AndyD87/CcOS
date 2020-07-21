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
#ifdef CCOS_GENERIC_NETWORK
  #include "Network/CcSocket.h"
#endif
#include "CcGlobalStrings.h"
#include "CcUserList.h"
#include "CcObject.h"
#include "Devices/ITimer.h"
#include "Devices/ICpu.h"
#include "CcThreadContext.h"
#include "CcFileSystem.h"
#include "CcMutex.h"
#include "CcGenericFilesystem.h"
#ifdef CCOS_GENERIC_NETWORK
  #include "Network/INetworkStack.h"
  #if defined(CCOS_CCKERNEL_GENERIC_LWIP_STACK)
    #include "Network/LwipStack/LwipNetworkStack.h"
  #else
    #include "Network/Stack/CcNetworkStack.h"
  #endif
#endif
#include "CcList.h"
#include "Devices/ILed.h"
#include "CcVersion.h"
#ifdef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
  #include <time.h>
#endif

extern "C" void __cxa_pure_virtual() { while (1); }

class CcSystem::CPrivate
#ifndef CCOS_NO_SYSTEM_THREAD
    : public IThread
#endif
{
public:
  CPrivate()
  {
    s_pInstance = this;
    pCpu = CcKernel::getDevice(EDeviceType::Cpu).cast<ICpu>();
#ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
    oThreadsRunning.append(pCpu->mainThread());
    pCpu->setSystemTick(CcSystem::CPrivate::tick);
    pCpu->setThreadTick(CcSystem::CPrivate::changeThread);
#endif
  }

#ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
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
    if(s_pInstance->pCpu->checkOverflow())
    {
      if(s_pInstance->oThreadListLock.isLocked() == false)
      {
        s_pInstance->oThreadListLock.lock();
        s_pInstance->uiThreadCount = 0;
        CcThreadContext* pCurrentThreadContext = s_pInstance->pCpu->currentThread();
        if(pCurrentThreadContext != nullptr)
        {
          size_t uiPos = s_pInstance->oThreadsRunning.find(pCurrentThreadContext);
          if(uiPos < s_pInstance->oThreadsRunning.size())
          {
            CcList<CcThreadContext*>::iterator oListItem = s_pInstance->oThreadsRunning.dequeue(uiPos);
            if(pCurrentThreadContext->bClosed == false)
            {
              s_pInstance->oThreadsWaiting.append(oListItem);
            }
            else
            {
              s_pInstance->pCpu->deleteThread(*oListItem);
              s_pInstance->oThreadsRunning.removeIterator(oListItem);
            }
          }
          else
          {
            CcKernel::message(EMessage::Error);
          }
        }
        else
        {
          CcKernel::message(EMessage::Error);
        }

        if(s_pInstance->oThreadsWaiting.size() > 0)
        {
          CcList<CcThreadContext*>::iterator oListItem = s_pInstance->oThreadsWaiting.dequeueFirst();
          s_pInstance->oThreadsRunning.append(oListItem);
          s_pInstance->pCpu->loadThread((*oListItem));
        }
        else
        {
          s_pInstance->pCpu->loadThread(s_pInstance->pCpu->mainThread());
        }
        s_pInstance->oThreadListLock.unlock();
      }
    }
    else
    {
      CcKernel::message(EMessage::Error);
    }
  }
#endif

  bool appendThread(IThread* pThread)
  {
    pThread->enterState(EThreadState::Starting);
    CcThreadContext* pThreadContext = pCpu->createThread(pThread);
    bool bSuccess = nullptr != pThreadContext;
    if(bSuccess == false)
    {
      pThread->enterState(EThreadState::Stopped);
    }
#ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
    else
    {
      oThreadListLock.lock();
      oThreadsWaiting.append(pThreadContext);
      oThreadListLock.unlock();
    }
#endif // CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
    return bSuccess;
  }

  void nextThread()
  {
    pCpu->nextThread();
  }

#ifndef CCOS_NO_SYSTEM_THREAD
  void run()
  {
    pLedRun = CcKernel::getDevice(EDeviceType::Led, 0).cast<ILed>().ptr();
    if(pLedRun) pLedRun->off();
    pLedWarning = CcKernel::getDevice(EDeviceType::Led, 1).cast<ILed>().ptr();
    if(pLedWarning) pLedWarning->off();
    pLedError = CcKernel::getDevice(EDeviceType::Led, 2).cast<ILed>().ptr();
    if(pLedError) pLedError->off();
    while(getThreadState() == EThreadState::Running)
    {
      if(pLedRun != nullptr)
      {
        pLedRun->toggle();
      }
      CcKernel::delayMs(1000);
    }
  }

  virtual size_t getStackSize() override
  {
    return 128;
  }
#endif // CCOS_NO_SYSTEM_THREAD

  #ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
    volatile uint64           uiThreadCount = 0;
    volatile uint64           uiUpTime = 0;
    CcList<CcThreadContext*>  oThreadsWaiting;
    CcList<CcThreadContext*>  oThreadsRunning;
    CcMutex                   oThreadListLock;
  #endif
  CcStringMap               oEnvVars;
  CcGenericFilesystem       oFileSystem;
  CcHandle<ICpu>            pCpu;
  #ifdef CCOS_GENERIC_NETWORK
    INetworkStack*            pNetworkStack = nullptr;
  #endif
  ILed*                     pLedRun = nullptr;
  ILed*                     pLedWarning = nullptr;
  ILed*                     pLedError = nullptr;
  static CPrivate*          s_pInstance;
};

CcSystem::CPrivate* CcSystem::CPrivate::s_pInstance = nullptr;

CcSystem::CcSystem()
{
  CCNEW(m_pPrivateData, CcSystem::CPrivate);
}

CcSystem::~CcSystem()
{
  CCDELETE(m_pPrivateData);
}

void CcSystem::init()
{
  #ifndef CCOS_NO_SYSTEM_THREAD
    m_pPrivateData->start();
  #endif // CCOS_NO_SYSTEM_THREAD

  #ifdef CCOS_GENERIC_NETWORK
    #if defined(CCOS_CCKERNEL_GENERIC_LWIP_STACK)
      CCNEW(m_pPrivateData->pNetworkStack, LwipNetworkStack);
      m_pPrivateData->pNetworkStack->init();
    #else
      CCNEW(m_pPrivateData->pNetworkStack, CcNetworkStack);
      m_pPrivateData->pNetworkStack->init();
    #endif
  #endif // CCOS_GENERIC_NETWORK

  CcFileSystem::init();
  CcFileSystem::addMountPoint("/", &m_pPrivateData->oFileSystem);
}

void CcSystem::deinit()
{
  #ifndef CCOS_NO_SYSTEM_THREAD
    m_pPrivateData->stop();
    m_pPrivateData->waitForExit();
  #endif // CCOS_NO_SYSTEM_THREAD

  #ifdef CCOS_GENERIC_NETWORK
    CCDELETE(m_pPrivateData->pNetworkStack);
  #endif
}

bool CcSystem::initGUI()
{
  return false; // we do not have a gui on a generic system
}

bool CcSystem::initCLI()
{
  return false; // we do not have a cli on a generic system
}

bool CcSystem::deinitCLI()
{
  return false;
}

int CcSystem::initService()
{
  return 1;
}

bool CcSystem::isAdmin()
{
  return true;
}

bool CcSystem::createThread(IThread &oThread)
{
  return m_pPrivateData->appendThread(&oThread);
}

bool CcSystem::createProcess(CcProcess &oProcessToStart)
{
  CCUNUSED(oProcessToStart);
  return false;
}

void CcSystem::error()
{
  if( m_pPrivateData->pLedError != nullptr)
  {
    m_pPrivateData->pLedError->on();
  }
}

void CcSystem::warning()
{
  if( m_pPrivateData->pLedWarning != nullptr)
  {
    m_pPrivateData->pLedWarning->on();
  }
}

CcDateTime CcSystem::getDateTime()
{
  return getUpTime();
}

#ifndef CCOS_NO_SYSTEM_THREAD
CcDateTime CcSystem::getUpTime()
{
  return CcDateTime(m_pPrivateData->uiUpTime);
}

void CcSystem::sleep(uint32 timeoutMs)
{
  uint64 uiSystemTime(getUpTime().getTimestampUs());
  uiSystemTime += (timeoutMs*1000);
  // do it at least one times
  do
  {
    m_pPrivateData->nextThread();
  } while(uiSystemTime > m_pPrivateData->uiUpTime);
  m_pPrivateData->nextThread();
}
#endif

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, size_t uiNr)
{
  CCUNUSED(Type); CCUNUSED(uiNr); return nullptr;
}

CcDeviceHandle CcSystem::getDevice(EDeviceType Type, const CcString& Name)
{
  CCUNUSED(Type);
  CCUNUSED(Name);
  return nullptr;
}

INetworkStack* CcSystem::getNetworkStack()
{
  #ifdef  CCOS_GENERIC_NETWORK
    return m_pPrivateData->pNetworkStack;
  #else
    return nullptr;
  #endif  // CCOS_GENERIC_NETWORK
}

CcString CcSystem::getName()
{
  return CcGlobalStrings::CcOS;
}

CcVersion CcSystem::getVersion()
{
  return CcKernel::getVersion();
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
  #ifdef CCOS_GENERIC_NETWORK
    if(m_pPrivateData->pNetworkStack != nullptr)
      return m_pPrivateData->pNetworkStack->getSocket(eType);
    else
  #else
    CCUNUSED(eType);
  #endif // CCOS_GENERIC_NETWORK
    CCDEBUG("CcSystem::getSocket no network stack");
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

CcStatus CcSystem::loadModule(const CcString& sPath, const IKernel& oKernel)
{
  CCUNUSED(sPath);
  CCUNUSED(oKernel);
  return false;
}

CcStatus CcSystem::setWorkingDir(const CcString& sPath)
{
  CcStatus oOk(false);
  CCUNUSED(sPath);
  return oOk;
}
