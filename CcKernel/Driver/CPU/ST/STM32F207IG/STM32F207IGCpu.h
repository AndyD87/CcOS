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
 * @page      STM32F207IG
 * @subpage   STM32F207IGCpu
 * 
 * @page      STM32F207IGCpu
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F207IGCpu
 */
#ifndef _CCLIB_STM32F207IGCpu_H_
#define _CCLIB_STM32F207IGCpu_H_

#include "CcBase.h"
#include "Devices/ICpu.h"

class STM32F207IGCpu : public ICpu
{
public: // types
  class STM32F207IGCpuPrivate;
public: // methods
  STM32F207IGCpu();
  virtual ~STM32F207IGCpu();

  virtual size_t coreNumber() override;
  virtual CcThreadContext* mainThread() override;
  virtual CcThreadContext* createThread(IThread* oTargetThread) override;
  virtual void loadThread(CcThreadContext* pThreadData) override;
  virtual void deleteThread(CcThreadContext* pThreadData) override;
  virtual void nextThread() override;
  virtual void ThreadTick() override
    { if(m_pThreadTickMethod != nullptr) (*m_pThreadTickMethod)(); }
  virtual void SystemTick()
    { if(m_pSystemTickMethod != nullptr) (*m_pSystemTickMethod)(); }
private:
  CcStatus startSysClock();
private: // member
  STM32F207IGCpuPrivate* m_pPrivate;
};

#endif /* _CCLIB_STM32F207IGCpu_H_ */
