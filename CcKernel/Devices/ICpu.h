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
 * @page      Driver
 * @subpage   ICpu
 * 
 * @page      ICpu
 * @par       Language: C++11
 * @brief     Class ICpu
 */

#ifndef _ICpu_H_
#define _ICpu_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IDevice.h"
#include "CcThreadContext.h"

class IThread;
class CcThreadContext;

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED ICpu : public IDevice
{
public: // types
  typedef void(*FSystemTick)();
  typedef void(*FThreadTick)();
public:
  ICpu() = default;
  /**
   * @brief Destructor
   */
  virtual ~ICpu();
  virtual size_t coreNumber() = 0;
  virtual CcThreadContext* mainThread() = 0;
  virtual CcThreadContext* createThread(IThread* pTargetThread) = 0;
  virtual void loadThread(CcThreadContext* pThreadData) = 0;
  virtual void deleteThread(CcThreadContext* pThreadData) = 0;
  virtual void nextThread() = 0;
  void setSystemTick(FSystemTick pSystemTickMethod)
    { m_pSystemTickMethod = pSystemTickMethod; }
  void setThreadTick(FThreadTick pThreadTickMethod)
    { m_pThreadTickMethod = pThreadTickMethod; }

  static void CreateThread(void* pParam);

protected:
  virtual void changeThread() = 0;
  virtual void tick() = 0;
protected:
  FSystemTick m_pSystemTickMethod = nullptr;
  FThreadTick m_pThreadTickMethod = nullptr;
};

#endif /* _ICpu_H_ */
