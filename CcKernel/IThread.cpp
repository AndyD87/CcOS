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
  enterState(EThreadState::Stopping);
  waitForState(EThreadState::Stopped);
  CcKernel::getShutdownHandler().removeObject(this);
}

CcStatus IThread::start()
{
  return CcKernel::createThread(*this);
}

CcStatus IThread::startOnCurrent()
{
  enterState(EThreadState::Starting);
  return startOnThread();
}

CcStatus IThread::startOnThread()
{
  CcStatus oStatus;
  if(oStatus)
    oStatus = enterState(EThreadState::Running);
  if(oStatus)
    oStatus = enterState(EThreadState::Stopped);
  return getExitCode();
}

CcStatus IThread::enterState(EThreadState State)
{
  m_oStateLock.lock();
  CcStatus oSuccess = false;
  bool bDoUnlock = true;
  switch (State)
  {
    case EThreadState::Starting:
      if (EThreadState::Stopped == m_State)
      {
        oSuccess = true;
        m_State = State;
      }
      else if (m_State == EThreadState::Starting)
        oSuccess = true;
      break;
    case EThreadState::Running:
      if (EThreadState::Starting == m_State)
      {
        m_State = State;
        m_oStateLock.unlock();
        bDoUnlock = false;
        run();
        oSuccess = true;
      }
      else if (m_State == EThreadState::Starting)
        oSuccess = true;
      break;
    case EThreadState::Stopping:
      if (m_State == EThreadState::Starting)
      {
        m_oStateLock.unlock();
        waitForState(EThreadState::Running);
        m_oStateLock.lock();
      }
      if (m_State < EThreadState::Stopping)
      {
        m_State = State;
        m_oStateLock.unlock();
        bDoUnlock = false;
        oSuccess = true;
        onStop();
        waitForState(EThreadState::Stopped);
      }
      else
        oSuccess = true;
      break;
    case EThreadState::Stopped:
      if (m_State == EThreadState::Starting)
      {
        m_oStateLock.unlock();
        waitForState(EThreadState::Running);
        m_oStateLock.lock();
      }
      if (m_State != EThreadState::Stopped)
      {
        m_State = State;
        bDoUnlock = false;
        m_oStateLock.unlock();
        // Be aware here! Worker will delete itself here
        // Do never change any member after onStopped()
        oSuccess = onStopped();
      }
      else
      {
        oSuccess = true;
      }
      // Set stopped at the end if all is done to avoid conflicts
      // with other thread wich are waiting for stopped.
      break;
  }
  if(bDoUnlock)
    m_oStateLock.unlock();
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
