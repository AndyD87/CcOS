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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class STM32F303VCT6Cpu
 */
#pragma once

#include "CcBase.h"
#include "Devices/ICpu.h"
#include "Driver/CPU/Common/CcThreadData.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcGenericThreadHelper.h"
#include "IThread.h"

/**
 * @brief Cpu interface for STM32F303VCT6 microcontroller
 */
class STM32F303VCT6Cpu : public ICpu
{
public: // types
  class CPrivate;
public: // methods
  STM32F303VCT6Cpu();
  virtual ~STM32F303VCT6Cpu();

  virtual size_t coreNumber() override;
  virtual CcThreadContext* mainThread() override;
  virtual CcThreadContext* createThread(IThread* oTargetThread) override;
  virtual void loadThread(CcThreadContext* pThreadData) override;
  virtual void deleteThread(CcThreadContext* pThreadData) override;
  virtual void nextThread() override;
  virtual CcThreadContext* currentThread() override;
  virtual bool checkOverflow() override;
  virtual void enterCriticalSection() override;
  virtual void leaveCriticalSection() override;
  virtual bool isInIsr() override;

  inline static STM32F303VCT6Cpu* getCpu()
  { return pCpu; }

private:
  CcStatus startSysClock();
private: // member
  class STM32F303VCT6CpuThread : public IThread
  {
  public:
    STM32F303VCT6CpuThread() :
      IThread(CcGlobalStrings::CcOS)
      {enterState(EThreadState::Running);}
    virtual void run() override
      {}
    virtual size_t getStackSize() override
      { return 4; }
  };

  STM32F303VCT6CpuThread    oCpuThread;
  CcThreadContext         oCpuThreadContext;
  CcThreadData            oCpuThreadData;
  static STM32F303VCT6Cpu*  pCpu;
  #ifdef THREADHELPER
  static CcGenericThreadHelper oThreadHelper;
  #endif
};
