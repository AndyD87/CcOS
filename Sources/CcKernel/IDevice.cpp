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
      if(m_eState <= EState::Stop)
      {
        oStatus = setState(EState::Stopping);
      }
      if(oStatus)
      {
        if(onState(EState::Start))
        {
          oStatus = setState(EState::Starting);
        }
        else
        {
          setState(EState::Stop);
        }
      }
      break;
    case EState::Starting:
      if(m_eState <= EState::Stop)
      {
        oStatus = setState(EState::Stopping);
      }
      if(oStatus)
      {
        if(onState(EState::Starting))
        {
        }
        else
        {
          setState(EState::Stop);
        }
        oStatus = setState(EState::Run);
      }
      break;
    case EState::Run:
      if(m_eState <= EState::Run)
      {
        if(oStatus == onState(EState::Run))
        {
          oStatus = setState(EState::Running);
        }
        else
        {
          setState(EState::Stop);
        }
      }
      break;
    case EState::Pause:
      if(m_eState <= EState::Running)
      {
        if(oStatus == onState(EState::Pause))
        {
           oStatus = setState(EState::Paused);
        }
        else
        {
          setState(EState::Stop);
        }
      }
      break;
    case EState::Stop:
      if(m_eState <= EState::Stop)
      {
        if(oStatus == onState(EState::Stop))
        {
          oStatus = setState(EState::Stopping);
        }
        else
        {
          setState(EState::Stop);
        }
      }
      break;
    case EState::Stopping:
      if(m_eState <= EState::Stopping)
      {
        if(oStatus == onState(EState::Stopping))
        {
        }
        else
        {
          setState(EState::Stopped);
        }
      }
      break;
    case EState::Running:
      if(oStatus == onState(EState::Running))
      {
         m_eState= EState::Running;
      }
      else
      {
        setState(EState::Stop);
      }
      break;
    case EState::Paused:
      if(oStatus == onState(EState::Paused))
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
