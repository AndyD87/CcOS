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
      if(m_eState <= EState::Stop)
      {
        oStatus = setState(EState::Stopping);
      }
      oStatus = setState(EState::Starting);
      break;
    case EState::Starting:
      if(m_eState <= EState::Stop)
      {
        oStatus = setState(EState::Stopping);
      }
      oStatus = setState(EState::Run);
      break;
    case EState::Run:
      if(m_eState <= EState::Run)
      {
        oStatus = setState(EState::Running);
      }
      break;
    case EState::Pause:
      if(m_eState <= EState::Running)
      {
        oStatus = setState(EState::Paused);
      }
      break;
    case EState::Stop:
      if(m_eState <= EState::Stop)
      {
        oStatus = setState(EState::Stopping);
      }
      break;
    case EState::Stopping:
      if(m_eState <= EState::Stopping)
      {
        oStatus = setState(EState::Stopped);
      }
      break;
    case EState::Running:
      m_eState= EState::Running;
      break;
    case EState::Paused:
      m_eState= EState::Paused;
      break;
    case EState::Stopped:
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
