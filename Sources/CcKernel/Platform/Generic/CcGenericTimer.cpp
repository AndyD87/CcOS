/**
 * @copyright  Andreas Dirmeier (C) 2022
 *
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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGenericTimer
 */
#include "CcGenericTimer.h"
#include "CcDateTime.h"
#include "CcStatic.h"
#include "IThread.h"
#include "CcMutex.h"
#include "CcKernel.h"

class CcGenericTimer::CPrivate : public IThread
{
public:
  CPrivate(CcGenericTimer* pParent) : pParent(pParent)
  {}

  virtual void run() override
  {
    CcKernel::sleep(static_cast<uint32>(oTimeout.getTimestampMs()));
    oTimeoutLock.lock();
    if (pParent &&
        pParent->getState() == EState::Running)
    {
      if(pParent->timeout())
        pParent->stop();
    }
    oTimeoutLock.unlock();
  }

  CcDateTime        oTimeout;
  CcMutex           oTimeoutLock;
  CcGenericTimer*   pParent = nullptr;
};

CcGenericTimer::CcGenericTimer()
{
  CCNEW(m_pPrivate, CPrivate, this);
}

CcGenericTimer::~CcGenericTimer()
{
  stop();
  CCDELETE(m_pPrivate);
}

CcStatus CcGenericTimer::onState(EState eState)
{
  CcStatus oStatus;
  switch (eState)
  {
    case EState::Run:
    {
      m_pPrivate->start();
      break;
    }
    case EState::Stop:
    {
      m_pPrivate->stop();
    }
    default:
      break;
  }
  return oStatus;
}

CcStatus CcGenericTimer::setTimeout(const CcDateTime& oTimeout)
{
  m_pPrivate->oTimeout = oTimeout;
  //Check for request lower than 1 ms
  if (m_pPrivate->oTimeout.getMSecond() == 0)
  {
    m_pPrivate->oTimeout.getTimestampMs();
  }
  return true;
}
