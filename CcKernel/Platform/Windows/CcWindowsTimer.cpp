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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWindowsTimer
 **/
#include "CcWindowsTimer.h"
#include "CcDateTime.h"

class CcWindowsTimer::CPrivate
{
public:
  CPrivate(CcWindowsTimer* pParent) : pParent(pParent)
    {}
  static VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired);
  HANDLE hEvent = NULL;
  HANDLE hTimer = NULL;
  HANDLE hTimerQueue = NULL;
  CcDateTime oTimeout;
  CcWindowsTimer* pParent;
};

VOID CALLBACK CcWindowsTimer::CPrivate::TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
  if (lpParam == NULL)
  {
    CCDEBUG("TimerRoutine lpParam is NULL\n");
  }
  else
  {
    CcWindowsTimer::CPrivate* pTimer = reinterpret_cast<CcWindowsTimer::CPrivate*>(lpParam);
    if (TimerOrWaitFired)
    {
      pTimer->pParent->timeout();
    }
    SetEvent(pTimer->hEvent);
  }
}

CcWindowsTimer::CcWindowsTimer() 
{
  CCNEW(m_pPrivate, CPrivate, this);
}

CcWindowsTimer::~CcWindowsTimer() 
{
  if (getState() != EState::Stopped)
    setState(EState::Stop);
  CCDELETE(m_pPrivate);
}

CcStatus CcWindowsTimer::setState(EState eState)
{
  CcStatus oStatus;
  switch (eState)
  {
    case EState::Run:
    {
      // Use an event object to track the TimerRoutine execution
      m_pPrivate->hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
      if (NULL == m_pPrivate->hEvent)
      {
        oStatus.setSystemError(GetLastError());
        CCDEBUG("CreateEvent failed " + CcString::fromNumber(oStatus.getErrorInt()));
      }
      else
      {
        // Create the timer queue.
        m_pPrivate->hTimerQueue = CreateTimerQueue();
        if (NULL == m_pPrivate->hTimerQueue)
        {
          CloseHandle(m_pPrivate->hEvent);
          m_pPrivate->hEvent = NULL;
          oStatus.setSystemError(GetLastError());
          CCDEBUG("CreateTimerQueue failed (%d)\n" + CcString::fromNumber(oStatus.getErrorInt()));
        }
        else
        {
          // Set a timer to call the timer routine in 10 seconds.
          if (!CreateTimerQueueTimer(
                &m_pPrivate->hTimer, 
                m_pPrivate->hTimerQueue, 
                (WAITORTIMERCALLBACK) CcWindowsTimer::CPrivate::TimerRoutine, 
                m_pPrivate,
                static_cast<DWORD>(m_pPrivate->oTimeout.getTimestampMs()),
                0, 
                0)
          )
          {
            DeleteTimerQueue(m_pPrivate->hTimerQueue);
            m_pPrivate->hTimerQueue = NULL;
            CloseHandle(m_pPrivate->hEvent);
            m_pPrivate->hEvent = NULL;
            oStatus.setSystemError(GetLastError());
            CCDEBUG("CreateTimerQueueTimer failed (%d)\n" + CcString::fromNumber(oStatus.getErrorInt()));
          }
        }
      }

      break;
    }
    case EState::Stop:
    {
      if (m_pPrivate->hTimer && m_pPrivate->hTimerQueue)
      {
        DeleteTimerQueueTimer(m_pPrivate->hTimerQueue, m_pPrivate->hTimer, m_pPrivate->hEvent);
        m_pPrivate->hTimer = NULL;
      }
      if (m_pPrivate->hTimerQueue)
      {
        DeleteTimerQueue(m_pPrivate->hTimerQueue);
        m_pPrivate->hTimerQueue = NULL;
      }
      if (m_pPrivate->hEvent)
      {
        CloseHandle(m_pPrivate->hEvent);
        m_pPrivate->hEvent = NULL;
      }
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

CcStatus CcWindowsTimer::setTimeout(const CcDateTime& oTimeout)
{
  m_pPrivate->oTimeout = oTimeout;
  //Check for request lower than 1 ms
  if (m_pPrivate->oTimeout.getTimestampMs() == 0)
  {
    m_pPrivate->oTimeout.setTimestampMs(1);
  }
  return true;
}