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
 * @brief     Class IDevice
 */

#include "IDevice.h"
#include "CcKernel.h"
#include "CcSystem.h"

CcStatus IDevice::setState(EState eState)
{
  CcStatus oStatus;
  switch(eState)
  {
    case EState::Start:
      oStatus = onState(EState::Start);
      if(oStatus)
      {
        m_eState = EState::Start;
        oStatus = setState(EState::Starting);
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Starting:
      oStatus = onState(EState::Starting);
      if(oStatus)
      {
        m_eState = EState::Starting;
        oStatus = setState(EState::Run);
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Run:
      oStatus = onState(EState::Run);
      if(oStatus)
      {
        m_eState = EState::Run;
        oStatus = setState(EState::Running);
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Running:
      oStatus = onState(EState::Running);
      if(oStatus)
      {
        m_eState= EState::Running;
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Pause:
      oStatus = onState(EState::Pause);
      if(oStatus)
      {
        m_eState= EState::Pause;
        oStatus = setState(EState::Paused);
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Stop:
      oStatus = onState(EState::Stop);
      if(oStatus)
      {
        m_eState= EState::Stop;
        oStatus = setState(EState::Stopping);
      }
      else
      {
        setState(EState::Stopped);
      }
      break;
    case EState::Stopping:
      oStatus = onState(EState::Stopping);
      if(oStatus)
      {
        m_eState= EState::Stopping;
        oStatus = setState(EState::Stopped);
      }
      break;
    case EState::Paused:
      oStatus = onState(EState::Paused);
      if(oStatus)
      {
        m_eState= EState::Paused;
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Stopped:
      if(oStatus == onState(EState::Stopped))
      {
      }
      m_eState= EState::Stopped;
      break;
  }
  return oStatus;
}

CcStatus IDevice::restart()
{
  CcStatus oStatus;
  if(getState() > EState::Starting)
  {
    oStatus = setState(EState::Stop);
    if(oStatus) oStatus = setState(EState::Start);
  }
  else
  {
    oStatus = EStatus::DeviceNotRunning;
  }
  return oStatus;
}

void IDevice::registerIdle()
{
  CcKernel::getSystem().registerForIdle(this);
}

void IDevice::deregisterIdle()
{
  CcKernel::getSystem().deregisterForIdle(this);
}
