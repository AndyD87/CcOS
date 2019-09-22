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
#include "CcStatic.h"
#include "IThread.h"
#include "CcStringUtil.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "esp_heap_caps.h"
#include "CcMemoryMonitor.h"

#ifndef FREERTOS_MINIMUM_STACK_SIZE
  #define FREERTOS_MINIMUM_STACK_SIZE 2048
#endif

/*-----------------------------------------------------------*/

class FreeRTOSCpu::CPrivate
{
public:
  CPrivate()
  {}

public:
  static FreeRTOSCpu*  pCpu;

  static void task(void* pParam);
};

FreeRTOSCpu* FreeRTOSCpu::CPrivate::pCpu = nullptr;

void FreeRTOSCpu::CPrivate::task(void* pParam)
{
  {
    CcThreadContext* pThreadContext = static_cast<CcThreadContext*>(pParam);
    pThreadContext->pThreadObject->startOnThread();
    FreeRTOSCpu::CPrivate::pCpu->deleteThread(pThreadContext);
  }
  vTaskDelete( NULL );
}

FreeRTOSCpu::FreeRTOSCpu()
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->pCpu = this;
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
  return nullptr;
}

CcThreadContext* FreeRTOSCpu::createThread(IThread* pTargetThread)
{
  TaskHandle_t oHandle;
  pTargetThread->enterState(EThreadState::Starting);
  CCNEWTYPE(pThreadContext, CcThreadContext, pTargetThread, nullptr);
  configSTACK_DEPTH_TYPE uiStackSize = static_cast<configSTACK_DEPTH_TYPE>(pTargetThread->getStackSize());
  if(uiStackSize < FREERTOS_MINIMUM_STACK_SIZE)
    uiStackSize = FREERTOS_MINIMUM_STACK_SIZE;
  xTaskCreate(CPrivate::task,                           // Thread function
              pTargetThread->getName().getCharString(), // Thread name
              uiStackSize,                              // Thread stack size
              pThreadContext,                           // Thread context
              10,                                       // Thread priority
              &oHandle);
  pThreadContext->setData(CCVOIDPTRCAST(CcThreadData*, oHandle));
  return pThreadContext;
}

void  FreeRTOSCpu::loadThread(CcThreadContext* pTargetThread)
{
  CCUNUSED(pTargetThread);
}

void  FreeRTOSCpu::deleteThread(CcThreadContext* pTargetThread)
{
  CCDELETE(pTargetThread);
}

void FreeRTOSCpu::nextThread()
{
  // Do nothing yet
}

CcThreadContext* FreeRTOSCpu::currentThread()
{
  return nullptr;
}

bool FreeRTOSCpu::checkOverflow()
{
  bool bSuccess = true;
  return bSuccess;
}

void FreeRTOSCpu::enterCriticalSection()
{
}

void FreeRTOSCpu::leaveCriticalSection()
{
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
