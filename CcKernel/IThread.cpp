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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IThread
 */
#include "IThread.h"
#include "CcKernel.h"
#include "CcEventHandler.h"

IThread::IThread(const CcString& sName) : 
  m_sName(sName), 
  m_State(EThreadState::Stopped)
{
  CcKernel::getShutdownHandler().append(NewCcEvent(IThread, void, IThread::stop, this));
}

IThread::~IThread()
{ 
  // Wait a litte bit and try again until thread is stopped.
  while (m_State != EThreadState::Stopped)
  {
    enterState(EThreadState::Stopping);
    CcKernel::delayMs(1);
  }
  CcKernel::getShutdownHandler().removeObject(this);
  if (m_bIsDeleted)
    CcKernel::message(EMessage::Error);
  m_bIsDeleted = true;
}

void IThread::start()
{ 
  m_State = EThreadState::Starting;
  CcKernel::createThread(*this);
}

void IThread::startOnCurrent()
{
  enterState(EThreadState::Starting);
  enterState(EThreadState::Running);
  enterState(EThreadState::Stopping);
  enterState(EThreadState::Stopped);
}

CcStatus IThread::enterState(EThreadState State)
{
  CcStatus oSuccess = false;
  switch (State)
  {
    case EThreadState::Starting:
      if (EThreadState::Stopped == m_State)
      {
        oSuccess = true;
        m_State = State;
      }
      break;
    case EThreadState::Running:
      if (EThreadState::Starting == m_State)
      {
        m_State = State;
        run();
        oSuccess = true;
      }
      break;
    case EThreadState::Stopping:
      if (m_State < EThreadState::Stopping)
      {
        m_State = State;
        onStop();
        oSuccess = true;
      }
      break;
    case EThreadState::Stopped:
      if (m_State != EThreadState::Stopped)
      {
        m_State = State;
        oSuccess = getExitCode();
        onStopped();
      }
      else
      {
        m_State = EThreadState::Starting;
      }
      // Set stopped at the end if all is done to avoid conflicts
      // with other thread wich are waiting for stopped.
      break;
  }
  return oSuccess;
}

CcStatus IThread::waitForState(EThreadState eState, const CcDateTime& oTimeout)
{
  CcStatus oRet;
  CcDateTime oTimeWaiting;
  while (m_State < eState &&
         oRet)
  {
    if (oTimeout.getTimestampUs() != 0 && oTimeout < oTimeWaiting)
    {
      oRet = EStatus::TimeoutReached;
    }
    else if (oTimeout != 0)
    {
      CcKernel::delayMs(10);
      oTimeWaiting.addMSeconds(10);
    }
    else
    {
      CcKernel::delayMs(10);
    }
  }
  return oRet;
}
