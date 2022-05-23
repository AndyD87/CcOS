/*
 * GenericThreadManager.cpp
 *
 *  Created on: 19.05.2022
 *      Author: Dirmeier
 */
#include "CcGenericThreadManager.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcHandle.h"
#include "Devices/ICpu.h"

CcGenericThreadManager* CcGenericThreadManager::s_pInstance = nullptr;


CcGenericThreadManager::CcGenericThreadManager()
{
  s_pInstance = this;
}

CcGenericThreadManager::~CcGenericThreadManager()
{
  s_pInstance = nullptr;
}

void CcGenericThreadManager::init()
{
  pCpu = CcKernel::getDevice(EDeviceType::Cpu).cast<ICpu>();
}

void CcGenericThreadManager::tick()
{
  uiUpTime += 1000;
  uiThreadCount++;
  if(uiThreadCount >= 10)
  {
    changeThread();
  }
}

void CcGenericThreadManager::changeThread()
{
  if(pCpu.isValid() &&
      pCpu->checkOverflow() &&
      oThreadsWaiting.size() > 0)
  {
    if(oThreadListLock.isLocked() == false)
    {
      oThreadListLock.lock();
      uiThreadCount = 0;
      CcThreadContext* pCurrentThreadContext = pCpu->currentThread();
      if(pCurrentThreadContext != nullptr)
      {
        size_t uiPos = oThreadsRunning.find(pCurrentThreadContext);
        if(uiPos < oThreadsRunning.size())
        {
          CcList<CcThreadContext*>::iterator oListItem = oThreadsRunning.dequeue(uiPos);
          if((*oListItem)->bClosed == false)
          {
            oThreadsWaiting.append(oListItem);
          }
          else
          {
            pCpu->deleteThread(*oListItem);
            oThreadsRunning.cleanupIterator(oListItem);
          }
          CcList<CcThreadContext*>::iterator oListItemWaiting = oThreadsWaiting.dequeueFirst();
          oThreadsRunning.append(oListItemWaiting);
          pCpu->loadThread((*oListItemWaiting));
        }
        else
        {
          // Ignore temporarily because of missing startup thread.
          CcKernel::message(EMessage::Error);
        }
      }
      else
      {
        CcKernel::message(EMessage::Error);
      }

      oThreadListLock.unlock();
    }
  }
  else
  {
    // May be we are in the startup state
    //CcKernel::message(EMessage::Error);
  }
}

bool CcGenericThreadManager::appendThread(IThread* pThread)
{
  CcThreadContext* pThreadContext = pCpu->createThread(pThread);
  bool bSuccess = nullptr != pThreadContext;
  if(bSuccess == false)
  {
    pThread->stop();
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

void CcGenericThreadManager::nextThread()
{
  pCpu->nextThread();
}

void CcGenericThreadManager::start()
{
  pLedRun = CcKernel::getDevice(EDeviceType::Led, 0).cast<ILed>().ptr();
  if(pLedRun) pLedRun->off();
}

bool CcGenericThreadManager::idle()
{
  bool bKeepLooping = true;
  if( pLedRun != nullptr &&
      m_oNextToggle < uiUpTime)
  {
    m_oNextToggle += 500000;
    pLedRun->toggle();
  }
  for(IDevice* pDev : oIdleList)
    pDev->idle();
  return bKeepLooping;
}

void CcGenericThreadManager::stop()
{
}
