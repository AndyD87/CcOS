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

void CcGenericThreadManager::init()
{
  pCpu = CcKernel::getDevice(EDeviceType::Cpu).cast<ICpu>();
}

#ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
  void CcGenericThreadManager::tick()
  {
    s_pInstance->uiUpTime += 1000;
    s_pInstance->uiThreadCount++;
    if(s_pInstance->uiThreadCount >= 10)
    {
      changeThread();
    }
  }

  void CcGenericThreadManager::changeThread()
  {
    if(s_pInstance->pCpu.isValid() &&
        s_pInstance->pCpu->checkOverflow())
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
            // Ignore temporarily because of missing startup thread.
            //CcKernel::message(EMessage::Error);
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
    //else
    //{
    //  CcKernel::message(EMessage::Error);
    //}
  }
#endif

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

#ifndef CCOS_NO_SYSTEM_THREAD
  void CcGenericThreadManager::run()
  {
    pLedRun = CcKernel::getDevice(EDeviceType::Led, 0).cast<ILed>().ptr();
    if(pLedRun) pLedRun->off();
    pLedWarning = CcKernel::getDevice(EDeviceType::Led, 1).cast<ILed>().ptr();
    if(pLedWarning) pLedWarning->off();
    pLedError = CcKernel::getDevice(EDeviceType::Led, 2).cast<ILed>().ptr();
    if(pLedError) pLedError->off();
    CcDateTime oNextToggle = 0;
    while(getThreadState() == EThreadState::Running)
    {
      if( pLedRun != nullptr &&
          oNextToggle < CcDateTime(uiUpTime))
      {
        oNextToggle.addMSeconds(500);
        pLedRun->toggle();
      }
      for(IDevice* pDev : oIdleList)
        pDev->idle();
    }
  }

  size_t CcGenericThreadManager::getStackSize()
  {
    return 256;
  }
#endif // CCOS_NO_SYSTEM_THREAD
