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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcThreadObject
 */
#include "CcThreadObject.h"
#include "CcKernel.h"

CcThreadObject::~CcThreadObject()
{ 
  if (m_State != EThreadState::Stopped)
    enterState(EThreadState::Stopping);
  // Wait a litte bit and try again until thread is stopped.
  while (m_State != EThreadState::Stopped) CcKernel::delayMs(1);
}

void CcThreadObject::start ( void )
{ 
  m_State = EThreadState::Starting;
  CcKernel::createThread(*this);
}

void CcThreadObject::startOnCurrent(void)
{
  enterState(EThreadState::Running);
  run();
  enterState(EThreadState::Stopped);
  onStopped();
}

void CcThreadObject::stop(void)
{
  if (getThreadState() == EThreadState::Running)
  {
    enterState(EThreadState::Stopping);
  }
  else
  {
    enterState(EThreadState::Stopped);
  }
}

void CcThreadObject::enterState(EThreadState State)
{
  m_State = State;
}

void CcThreadObject::waitForExit()
{
  while (m_State != EThreadState::Stopped) CcKernel::delayMs(1);
}

EThreadState CcThreadObject::getThreadState(void)
{
  return m_State;
}
