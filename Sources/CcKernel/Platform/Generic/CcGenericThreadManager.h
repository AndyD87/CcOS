/*
 * CcGenericThreadManager.h
 *
 *  Created on: 19.05.2022
 *      Author: Dirmeier
 */

#pragma once

#include "CcBase.h"
#include "IThread.h"
#include "CcList.h"
#include "Devices/ILed.h"
#include "CcThreadContext.h"

class ICpu;

class CcGenericThreadManager
#ifndef CCOS_NO_SYSTEM_THREAD
    : public IThread
#endif
{
public:
  CcGenericThreadManager();
  void init();
  #ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
    static void tick();
    static void changeThread();
  #endif

  bool appendThread(IThread* pThread);
  void nextThread();

  #ifndef CCOS_NO_SYSTEM_THREAD
    void run();
    virtual size_t getStackSize() override;
  #endif // CCOS_NO_SYSTEM_THREAD

  #ifndef CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER
    volatile uint64           uiThreadCount = 0;
    volatile uint64           uiUpTime = 0;
    CcList<CcThreadContext*>  oThreadsWaiting;
    CcList<CcThreadContext*>  oThreadsRunning;
    CcMutex                   oThreadListLock;
  #endif

  CcHandle<ICpu>            pCpu;
  ILed*                     pLedRun = nullptr;
  ILed*                     pLedWarning = nullptr;
  ILed*                     pLedError = nullptr;
  CcVector<IDevice*>        oIdleList;

  static CcGenericThreadManager*          s_pInstance;
};
