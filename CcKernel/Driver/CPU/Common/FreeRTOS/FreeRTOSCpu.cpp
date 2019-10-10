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
#include "CcList.h"

#ifndef FREERTOS_MINIMUM_STACK_SIZE
  #define FREERTOS_MINIMUM_STACK_SIZE 2048
#endif
CCEXTERNC uint32_t esp_get_free_heap_size(void);
/*-----------------------------------------------------------*/

class FreeRTOSCpu::CPrivate
{
public:
  CPrivate()
  {}

public:
  static FreeRTOSCpu*  pCpu;

  static void task(void* pParam);
  static void idleTask(void* pParam);

  static bool s_bIdleStarted;
  static CcList<TaskHandle_t> s_oDeleteTasks;

};

bool FreeRTOSCpu::CPrivate::s_bIdleStarted = false;
CcList<TaskHandle_t> FreeRTOSCpu::CPrivate::s_oDeleteTasks;
FreeRTOSCpu* FreeRTOSCpu::CPrivate::pCpu = nullptr;

void FreeRTOSCpu::CPrivate::task(void* pParam)
{
  if(pParam)
  {
    CCDEBUG(CcString::fromNumber(esp_get_free_heap_size()));
    CcThreadContext* pThreadContext = static_cast<CcThreadContext*>(pParam);
    pThreadContext->pThreadObject->startOnThread();
    TaskHandle_t hTaskToDelete = CCVOIDPTRCAST(TaskHandle_t, pThreadContext->pData);
    FreeRTOSCpu::CPrivate::pCpu->deleteThread(pThreadContext);
    s_oDeleteTasks.append(hTaskToDelete);
  }
  while(1);
}

void FreeRTOSCpu::CPrivate::idleTask(void* pParam)
{
  while(1)
  {
    while(s_oDeleteTasks.size() > 0)
    {
      vTaskDelete( s_oDeleteTasks[0] );
      s_oDeleteTasks.remove(0);
    }
    CcKernel::sleep(1);
  }
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
  if(CPrivate::s_bIdleStarted == false)
  {
    CPrivate::s_bIdleStarted = true;
    TaskHandle_t oHandle;
    configSTACK_DEPTH_TYPE uiStackSize = 1024;
    xTaskCreate(CPrivate::idleTask, // Thread function
                "CcOS idle task",   // Thread name
                uiStackSize,        // Thread stack size
                NULL,               // Thread context
                10,                 // Thread priority
                &oHandle);
  }
  TaskHandle_t oHandle;
  pTargetThread->enterState(EThreadState::Starting);
  CCNEWTYPE(pThreadContext, CcThreadContext, pTargetThread, nullptr);
  configSTACK_DEPTH_TYPE uiStackSize = static_cast<configSTACK_DEPTH_TYPE>(pTargetThread->getStackSize());
  if(uiStackSize < FREERTOS_MINIMUM_STACK_SIZE)
    uiStackSize = FREERTOS_MINIMUM_STACK_SIZE;
  long iTaskId = xTaskCreate(CPrivate::task,            // Thread function
              pTargetThread->getName().getCharString(), // Thread name
              uiStackSize,                              // Thread stack size
              pThreadContext,                           // Thread context
              10,                                       // Thread priority
              &oHandle);
  if(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == iTaskId)
  {
    CCDELETE(pThreadContext);
  }
  else
  {
    pThreadContext->setData(CCVOIDPTRCAST(CcThreadData*, oHandle));
  }
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
  taskENTER_CRITICAL();
}

void FreeRTOSCpu::leaveCriticalSection()
{
  taskEXIT_CRITICAL();
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
