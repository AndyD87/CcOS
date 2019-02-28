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
 * @brief     Class CcThreadObject
 */
#include "CcThreadObject.h"
#include "CcKernel.h"
#include "CcEventHandler.h"

CcThreadObject::CcThreadObject() : 
  m_State(EThreadState::Stopped)
{
  CcKernel::getShutdownHandler().append(NewCcEvent(CcThreadObject, void, CcThreadObject::stop, this));
}

CcThreadObject::CcThreadObject(const CcString& sName) : 
  m_sName(sName), 
  m_State(EThreadState::Stopped)
{
  CcKernel::getShutdownHandler().removeObject(this);
}


CcThreadObject::~CcThreadObject()
{ 
  if (m_State != EThreadState::Stopped)
    enterState(EThreadState::Stopping);
  // Wait a litte bit and try again until thread is stopped.
  while (m_State != EThreadState::Stopped) CcKernel::delayMs(1);
}

void CcThreadObject::start()
{ 
  m_State = EThreadState::Starting;
  CcKernel::createThread(*this);
}

void CcThreadObject::startOnCurrent()
{
  enterState(EThreadState::Running);
  run();
  enterState(EThreadState::Stopped);
  onStopped();
}

void CcThreadObject::stop()
{
  if (getThreadState() == EThreadState::Running)
  {
    enterState(EThreadState::Stopping);
    onStop();
  }
  else if(getThreadState() != EThreadState::Stopped)
  {
    enterState(EThreadState::Stopped);
    onStopped();
  }
}

void CcThreadObject::enterState(EThreadState State)
{
  m_State = State;
}

CcStatus CcThreadObject::waitForState(EThreadState eState, const CcDateTime& oTimeout)
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
    else
    {
      CcKernel::delayMs(10);
      oTimeWaiting.addMSeconds(10);
    }
  }
  return oRet;
}

EThreadState CcThreadObject::getThreadState()
{
  return m_State;
}
