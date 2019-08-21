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
 * @brief     Implementation of class FreeRTOSCpu
 **/

#include <Driver/CPU/Common/FreeRTOS/FreeRTOSCpu.h>
#include <Driver/CPU/Common/CcThreadData.h>
#include "CcKernel.h"
#include "CcGenericThreadHelper.h"
#include "CcStatic.h"
#include "IThread.h"
CCEXTERNC_BEGIN
CCEXTERNC_END

typedef void(*TaskFunction_t)(void* pParam);

/*-----------------------------------------------------------*/

class FreeRTOSCpu::CPrivate
{
private:
  class FreeRTOSCpuThread : public IThread
  {
  public:
    FreeRTOSCpuThread() :
      IThread("CcOS")
      {enterState(EThreadState::Running);}
    virtual void run() override
      {}
    virtual size_t getStackSize() override
      { return 4; }
  };
public:
  CPrivate() :
    oCpuThreadContext(&oCpuThread, nullptr),
    oCpuThreadData(&oCpuThreadContext)
  {
  }

public:
  FreeRTOSCpuThread    oCpuThread;
  CcThreadContext         oCpuThreadContext;
  CcThreadData            oCpuThreadData;
  static FreeRTOSCpu*  pCpu;
  #ifdef THREADHELPER
  static CcGenericThreadHelper oThreadHelper;
  #endif
};

FreeRTOSCpu* FreeRTOSCpu::CPrivate::pCpu = nullptr;
volatile CcThreadContext* pCurrentThreadContext = nullptr;
volatile CcThreadData* pCurrentThreadData       = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;
#ifdef THREADHELPER
CcGenericThreadHelper FreeRTOSCpu::CPrivate::oThreadHelper;
#endif

CCEXTERNC void FreeRTOSCpu_SysTick()
{
  //HAL_IncTick();
  if(FreeRTOSCpu::CPrivate::pCpu != nullptr)
  {
    FreeRTOSCpu::CPrivate::pCpu->tick();
  }
}

CCEXTERNC void FreeRTOSCpu_ThreadTick()
{
  //NVIC_ClearPendingIRQ(USART3_IRQn);
  if(FreeRTOSCpu::CPrivate::pCpu != nullptr)
  {
    FreeRTOSCpu::CPrivate::pCpu->changeThread();
  }
}

FreeRTOSCpu::FreeRTOSCpu()
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->pCpu = this;
  m_pPrivate->oCpuThreadContext.setData(&m_pPrivate->oCpuThreadData);
  pCurrentThreadContext    = &m_pPrivate->oCpuThreadContext;
  pCurrentThreadData       = &m_pPrivate->oCpuThreadData;
  enterCriticalSection();
  leaveCriticalSection();
  startSysClock();
}

FreeRTOSCpu::~FreeRTOSCpu()
{
  CCDELETE(m_pPrivate);
}

size_t FreeRTOSCpu::coreNumber()
{
  return 1;
}

CcThreadContext* FreeRTOSCpu::mainThread()
{
  return &m_pPrivate->oCpuThreadContext;
}

CcThreadContext* FreeRTOSCpu::createThread(IThread* pTargetThread)
{
  CCNEWTYPE(pReturn, CcThreadContext, pTargetThread, nullptr);
  CCNEW(pReturn->pData, CcThreadData, pReturn);
  return pReturn;
}

void  FreeRTOSCpu::loadThread(CcThreadContext* pTargetThread)
{
  if(pCurrentThreadContext->pData != nullptr)
  {
    pCurrentThreadContext = pTargetThread;
    pCurrentThreadData    = static_cast<CcThreadData*>(pTargetThread->pData);
  }
}

void  FreeRTOSCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error);
  }
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void FreeRTOSCpu::nextThread()
{
  // Do not change thread in isr!
  if(!isInIsr())
  {
    //NVIC_SetPendingIRQ(USART3_IRQn);
  }
}

CcThreadContext* FreeRTOSCpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool FreeRTOSCpu::checkOverflow()
{
  bool bSuccess = true;
  if(pCurrentThreadData->isOverflowDetected())
  {
    bSuccess = false;
  }
  return bSuccess;
}

void FreeRTOSCpu::enterCriticalSection()
{
  //__malloc_lock(nullptr);
}

void FreeRTOSCpu::leaveCriticalSection()
{
  //__malloc_unlock(nullptr);
}

bool FreeRTOSCpu::isInIsr()
{
  bool bRet = false;
  uint32 uiIpsr = 0;
  if(uiIpsr != 0)
  {
    bRet = true;
  }
  else
  {
    bRet = false;
  }
  return bRet;
}

CcStatus FreeRTOSCpu::startSysClock()
{
  CcStatus oStatus(false);
  return oStatus;
}
