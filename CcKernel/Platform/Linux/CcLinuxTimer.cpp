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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxTimer
 */
#include "CcLinuxTimer.h"
#include "CcDateTime.h"

#include <signal.h>
#include <time.h>

class CcLinuxTimer::CPrivate
{
public:
  CPrivate(CcLinuxTimer* pParent) : pParent(pParent)
    {}
  static void TimerRoutine(sigval timer_data);

  timer_t           oTimerId;
  struct sigevent   oSignalEvent;
  struct sigaction  oSignalAction;
  struct itimerspec iTimerSpec;
  CcDateTime        oTimeout;
  sigset_t          oSigMask;
  CcLinuxTimer* pParent;
};

void CcLinuxTimer::CPrivate::TimerRoutine(union sigval timer_data)
{
  CcLinuxTimer::CPrivate* pTimer = reinterpret_cast<CcLinuxTimer::CPrivate*>(timer_data.sival_ptr);
  if (pTimer && pTimer->pParent)
  {
    if(pTimer->pParent->timeout())
      pTimer->pParent->stop();
  }
}

CcLinuxTimer::CcLinuxTimer()
{
  CCNEW(m_pPrivate, CPrivate, this);
}

CcLinuxTimer::~CcLinuxTimer()
{
  CCDELETE(m_pPrivate);
}

CcStatus CcLinuxTimer::setState(EState eState)
{
  CcStatus oStatus;
  switch (eState)
  {
    case EState::Run:
    {

      //m_pPrivate->oSignalAction.sa_flags = SA_SIGINFO;
      //m_pPrivate->oSignalAction.sa_sigaction = CcLinuxTimer::CPrivate::TimerRoutine;
      //sigemptyset(&m_pPrivate->oSignalAction.sa_mask);
      //if (sigaction(SIGRTMIN, &m_pPrivate->oSignalAction, NULL) == -1)
      //{
      //    CCERROR("sigaction");
      //}
      //else
      //{
        m_pPrivate->oSignalEvent.sigev_notify = SIGEV_THREAD;
        m_pPrivate->oSignalEvent.sigev_notify_function = CcLinuxTimer::CPrivate::TimerRoutine;
        m_pPrivate->oSignalEvent.sigev_value.sival_ptr = m_pPrivate;

        if (timer_create(CLOCK_REALTIME, &m_pPrivate->oSignalEvent, &m_pPrivate->oTimerId) == -1)
        {
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
               CCERROR("timer_settime");
          }
          else
          {
            sigemptyset(&m_pPrivate->oSigMask);
            sigaddset(&m_pPrivate->oSigMask, SIGRTMIN);
            if (sigprocmask(SIG_SETMASK, &m_pPrivate->oSigMask, NULL) == -1)
                CCERROR("sigprocmask");
          }
        //}
      }
      break;
    }
    case EState::Stop:
    {
      if (sigprocmask(SIG_UNBLOCK, &m_pPrivate->oSigMask, NULL) == -1)
        CCERROR("sigprocmask");

      timer_delete(m_pPrivate->oTimerId);
      break;
    }
    default:
      break;
  }
  if (oStatus)
  {
    oStatus = IDevice::setState(eState);
  }
  return oStatus;
}

CcStatus CcLinuxTimer::setTimeout(const CcDateTime& oTimeout)
{
  m_pPrivate->oTimeout = oTimeout;
  //Check for request lower than 1 ms
  if (m_pPrivate->oTimeout.getMSecond() == 0 &&
      m_pPrivate->oTimeout > 0)
  {
    m_pPrivate->oTimeout.setTimestampMs(1);
  }
  return true;
}
