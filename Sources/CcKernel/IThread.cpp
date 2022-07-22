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
 * @brief     Class IThread
 */
#include "IThread.h"
#include "CcKernel.h"
#include "CcEventHandler.h"

IThread::IThread(const CcString& sName) :
  CcReferenceCount(0),
  m_sName(sName),
  m_State(EThreadState::Stopped)
{
}

IThread::~IThread()
{
  // Wait a litte bit and try again until thread is stopped.
  enterState(EThreadState::Stopping);
  waitForExit();
}

CcStatus IThread::start()
{
  CcStatus oState = enterState(EThreadState::Starting);
  if (oState)
  {
    oState = CcKernel::createThread(*this);
  }
  return oState;
}

CcStatus IThread::startOnCurrent()
{
  CcStatus oState = enterState(EThreadState::Starting);
  if (oState)
  {
    oState = startOnThread();
  }
  return oState;
}

CcStatus IThread::startOnThread()
{
  CcStatus oStatus = enterState(EThreadState::Starting);
  // Check preconditions
  if(oStatus)
    oStatus = enterState(EThreadState::Running);
  if(oStatus || oStatus == EStatus::AlreadyStopped)
    oStatus = enterState(EThreadState::Stopped);
  if (oStatus)
    oStatus = getExitCode();
  return oStatus;
}

CcStatus IThread::waitForState(EThreadState eState, const CcDateTime& oTimeout)
{
  referenceCountIncrement();
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
  referenceCountDecrement();
  return oRet;
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
        setState(State);
      }
      else if (m_State == EThreadState::Starting)
        oSuccess = true;
      else if (m_State >= EThreadState::Stopping)
        oSuccess = EStatus::AlreadyStopped;
      else
        oSuccess = false;
      break;
    case EThreadState::Running:
      if (EThreadState::Starting == m_State)
      {
        setState(State);
        m_oStateLock.unlock();
        bDoUnlock = false;
        run();
        oSuccess = true;
      }
      else if (m_State == EThreadState::Starting)
        oSuccess = true;
      else if (m_State >= EThreadState::Stopping)
        oSuccess = EStatus::AlreadyStopped;
      break;
    case EThreadState::Stopping:
      if (m_State < EThreadState::Stopping)
      {
        setState(State);
        onStop();
      }
      oSuccess = true;
      break;
    case EThreadState::Stopped:
      // Start onStop Methods if not already set
      if (m_State < EThreadState::Stopping)
      {
        setState(EThreadState::Stopping);
        onStop();
      }
      if (m_State != EThreadState::Stopped)
      {
        setState(State);
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
  if (bDoUnlock)
    m_oStateLock.unlock();
  return oSuccess;
}
