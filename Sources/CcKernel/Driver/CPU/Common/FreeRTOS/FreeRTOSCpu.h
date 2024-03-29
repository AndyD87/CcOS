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
 * @brief     Class FreeRTOSCpu
 */
#pragma once

#include "CcBase.h"
#include "Devices/ICpu.h"

/**
 * @brief Simulate Cpu on an FreeRTOS
 */
class FreeRTOSCpu : public ICpu
{
public: // types
  class CPrivate;
public: // methods
  FreeRTOSCpu();
  virtual ~FreeRTOSCpu();

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
private:
  CcStatus startSysClock();
private: // member
  CPrivate* m_pPrivate;
};
