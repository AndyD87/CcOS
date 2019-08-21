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
 * @brief     Implementation of class ESP8266Cpu
 **/

#include <Driver/CPU/espressif/ESP8266/ESP8266Cpu.h>
#include <Driver/CPU/espressif/ESP8266/ESP8266Driver.h>
#include "Driver/CPU/Common/CcThreadData.h"
#include "CcKernel.h"
#include "CcGenericThreadHelper.h"
#include "CcStatic.h"
#include "IThread.h"
#include <stdlib.h>
CCEXTERNC_BEGIN
//#include "ets_sys.h"
//#include "osapi.h"
//#include "gpio.h"
//#include "os_type.h"
CCEXTERNC_END

typedef void(*TaskFunction_t)(void* pParam);


// ESP-12 modules have LED on GPIO2. Change to another GPIO
// for other boards.

/*
static const int pin = 2;
static volatile os_timer_t some_timer;CCEXTERNC void some_timerfunc(void *arg)
{
  CCUNUSED(arg);
  //Do blinky stuff
  if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & (1 << pin))
  {
    // set gpio low
    gpio_output_set(0, (1 << pin), 0, 0);
  }
  else
  {
    // set gpio high
    gpio_output_set((1 << pin), 0, 0, 0);
  }
}

CCEXTERNC void ICACHE_FLASH_ATTR user_init()
{
  main(0, nullptr);
  // init gpio subsytem
  gpio_init();

  // configure UART TXD to be GPIO1, set as output
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);
  gpio_output_set(0, 0, (1 << pin), 0);

  // setup timer (500ms, repeating)
  os_timer_setfn(const_cast<os_timer_t*>(&some_timer),
                 (os_timer_func_t *)some_timerfunc, NULL);
  os_timer_arm(const_cast<os_timer_t*>(&some_timer), 500, 1);
}
*/

CCEXTERNC void __cxa_pure_virtual() { while (1); }

/*-----------------------------------------------------------*/

class ESP8266Cpu::CPrivate
{
private:
  class ESP8266CpuThread : public IThread
  {
  public:
    ESP8266CpuThread() :
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
  ESP8266CpuThread    oCpuThread;
  CcThreadContext         oCpuThreadContext;
  CcThreadData            oCpuThreadData;
  static ESP8266Cpu*  pCpu;
  #ifdef THREADHELPER
  static CcGenericThreadHelper oThreadHelper;
  #endif
};

ESP8266Cpu* ESP8266Cpu::CPrivate::pCpu = nullptr;
volatile CcThreadContext* pCurrentThreadContext = nullptr;
volatile CcThreadData* pCurrentThreadData       = nullptr;
const uint8 ucMaxSyscallInterruptPriority = 0;
#ifdef THREADHELPER
CcGenericThreadHelper ESP8266Cpu::CPrivate::oThreadHelper;
#endif

CCEXTERNC void ESP8266Cpu_SysTick()
{
  //HAL_IncTick();
  if(ESP8266Cpu::CPrivate::pCpu != nullptr)
  {
    ESP8266Cpu::CPrivate::pCpu->tick();
  }
}

CCEXTERNC void ESP8266Cpu_ThreadTick()
{
  //NVIC_ClearPendingIRQ(USART3_IRQn);
  if(ESP8266Cpu::CPrivate::pCpu != nullptr)
  {
    ESP8266Cpu::CPrivate::pCpu->changeThread();
  }
}

ESP8266Cpu::ESP8266Cpu()
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

ESP8266Cpu::~ESP8266Cpu()
{
  CCDELETE(m_pPrivate);
}

size_t ESP8266Cpu::coreNumber()
{
  return 1;
}

CcThreadContext* ESP8266Cpu::mainThread()
{
  return &m_pPrivate->oCpuThreadContext;
}

CcThreadContext* ESP8266Cpu::createThread(IThread* pTargetThread)
{
  CCNEWTYPE(pReturn, CcThreadContext, pTargetThread, nullptr);
  CCNEW(pReturn->pData, CcThreadData, pReturn);
  return pReturn;
}

void  ESP8266Cpu::loadThread(CcThreadContext* pTargetThread)
{
  if(pCurrentThreadContext->pData != nullptr)
  {
    pCurrentThreadContext = pTargetThread;
    pCurrentThreadData    = static_cast<CcThreadData*>(pTargetThread->pData);
  }
}

void  ESP8266Cpu::deleteThread(CcThreadContext* pTargetThread)
{
  CcThreadData* pCurrentThreadData = static_cast<CcThreadData*>(pTargetThread->pData);
  if(pCurrentThreadData->isOverflowDetected())
  {
    CcKernel::message(EMessage::Error);
  }
  CCDELETE(pCurrentThreadData);
  CCDELETE(pTargetThread);
}

void ESP8266Cpu::nextThread()
{
  // Do not change thread in isr!
  if(!isInIsr())
  {
    //NVIC_SetPendingIRQ(USART3_IRQn);
  }
}

CcThreadContext* ESP8266Cpu::currentThread()
{
  return const_cast<CcThreadContext*>(pCurrentThreadContext);
}

bool ESP8266Cpu::checkOverflow()
{
  bool bSuccess = true;
  if(pCurrentThreadData->isOverflowDetected())
  {
    bSuccess = false;
  }
  return bSuccess;
}

void ESP8266Cpu::enterCriticalSection()
{
  //__malloc_lock(nullptr);
}

void ESP8266Cpu::leaveCriticalSection()
{
  //__malloc_unlock(nullptr);
}

bool ESP8266Cpu::isInIsr()
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

CcStatus ESP8266Cpu::startSysClock()
{
  CcStatus oStatus(false);
  return oStatus;
}
