/*
 * CcGenericThreadManager.h
 *
 *  Created on: 19.05.2022
 *      Author: Dirmeier
 */

#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "Devices/ILed.h"
#include "CcThreadContext.h"
#include "CcMutex.h"
#include "CcVector.h"

class ICpu;

class CcGenericThreadManager
{
public:
  CcGenericThreadManager();
  ~CcGenericThreadManager();
  void init();
  void tick();
  void changeThread();

  bool appendThread(IThread* pThread);
  void nextThread();

  void start();
  bool idle();
  void stop();

  volatile uint64           uiThreadCount = 0;
  volatile uint64           uiUpTime = 0;
  CcList<CcThreadContext*>  oThreadsWaiting;
  CcList<CcThreadContext*>  oThreadsRunning;
  CcMutex                   oThreadListLock;

  CcHandle<ICpu>            pCpu;
  ILed*                     pLedRun = nullptr;
  ILed*                     pLedWarning = nullptr;
  ILed*                     pLedError = nullptr;
  CcVector<IDevice*>        oIdleList;

  static CcGenericThreadManager* getInstance()
  { return s_pInstance; }
private:
  static CcGenericThreadManager*  s_pInstance;
  uint64                          m_oNextToggle = 0;
};
