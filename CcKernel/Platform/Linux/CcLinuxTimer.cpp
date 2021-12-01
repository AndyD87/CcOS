/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @brief     Implementation of Class CcLinuxTimer
 */
#include "CcLinuxTimer.h"
#include "CcDateTime.h"
#include "CcStatic.h"
#include "CcMutex.h"

#include <signal.h>
#include <time.h>

class CcLinuxTimer::CPrivate
{
public:
  CPrivate(CcLinuxTimer* pParent) : pParent(pParent)
    {}
  static void TimerRoutine(sigval timer_data);

  timer_t           oTimerId = 0;
  struct sigevent   oSignalEvent;
  struct itimerspec iTimerSpec;
  CcDateTime        oTimeout;
  CcMutex           oTimeoutLock;
  CcLinuxTimer*     pParent = nullptr;
};

void CcLinuxTimer::CPrivate::TimerRoutine(union sigval timer_data)
{
  CcLinuxTimer::CPrivate* pTimer = reinterpret_cast<CcLinuxTimer::CPrivate*>(timer_data.sival_ptr);
  pTimer->oTimeoutLock.lock();
  if (pTimer &&
      pTimer->pParent &&
      pTimer->pParent->getState() == EState::Running)
  {
    if(pTimer->pParent->timeout())
      pTimer->pParent->stop();
  }
  pTimer->oTimeoutLock.unlock();
}

CcLinuxTimer::CcLinuxTimer()
{
  CCNEW(m_pPrivate, CPrivate, this);
}

CcLinuxTimer::~CcLinuxTimer()
{
  stop();
  CCDELETE(m_pPrivate);
}

CcStatus CcLinuxTimer::onState(EState eState)
{
  CcStatus oStatus;
  switch (eState)
  {
    case EState::Run:
    {
      CcStatic_memsetZeroObject(m_pPrivate->oSignalEvent);
      CcStatic_memsetZeroObject(m_pPrivate->oTimerId);
      m_pPrivate->oSignalEvent.sigev_notify = SIGEV_THREAD;
      m_pPrivate->oSignalEvent.sigev_notify_function = CcLinuxTimer::CPrivate::TimerRoutine;
      m_pPrivate->oSignalEvent.sigev_value.sival_ptr = m_pPrivate;

      if (timer_create(CLOCK_REALTIME, &m_pPrivate->oSignalEvent, &m_pPrivate->oTimerId) == -1)
      {
        oStatus = EStatus::CreateFailed;
        CCERROR("timer_create");
      }
      else
      {
        m_pPrivate->iTimerSpec.it_value.tv_sec = m_pPrivate->oTimeout.getTimestampS();
        m_pPrivate->iTimerSpec.it_value.tv_nsec = m_pPrivate->oTimeout.getTimestampNs() % 1000000000;
        m_pPrivate->iTimerSpec.it_interval.tv_sec = m_pPrivate->iTimerSpec.it_value.tv_sec;
        m_pPrivate->iTimerSpec.it_interval.tv_nsec = m_pPrivate->iTimerSpec.it_value.tv_nsec;

        if (timer_settime(m_pPrivate->oTimerId, 0, &m_pPrivate->iTimerSpec, NULL) == -1)
        {
          oStatus = EStatus::TimeoutReached;
          CCERROR("timer_settime");
        }
        else
        {
        }
      }
      break;
    }
    case EState::Stop:
    {
      if(m_eState == EState::Running)
      {
        // avoid new timer events by setting time to 0
        itimerspec iTimerSpec;
        CcStatic_memsetZeroObject(iTimerSpec);
        timer_settime(m_pPrivate->oTimerId, 0, &iTimerSpec, NULL);

        // delte timer object
        m_pPrivate->oTimeoutLock.lock();
        timer_delete(m_pPrivate->oTimerId);
        m_pPrivate->oTimeoutLock.unlock();
        break;
      }
    }
    default:
      break;
  }
  return oStatus;
}

CcStatus CcLinuxTimer::setTimeout(const CcDateTime& oTimeout)
{
  m_pPrivate->oTimeout = oTimeout;
  //Check for request lower than 1 ms
  if (m_pPrivate->oTimeout.getMSecond() == 0)
  {
    m_pPrivate->oTimeout.getTimestampMs();
  }
  return true;
}
