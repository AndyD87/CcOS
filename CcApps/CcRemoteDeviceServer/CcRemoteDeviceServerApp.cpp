/*
 * This file is part of CcRemoteDeviceServerApp.
 *
 * CcRemoteDeviceServerApp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServerApp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServerApp.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceServerApp
 */
#include "CcRemoteDeviceServerApp.h"
#include "CcKernel.h"
#include "Devices/IGpioPort.h"
#include "Devices/IGpioPin.h"
#include "Devices/Simulations/CcGpioPortSimulation.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"

CcRemoteDeviceServerApp::CcRemoteDeviceServerApp(const CcArguments& m_oArguments) :
  CcRemoteDeviceServer(),
  m_oArguments(m_oArguments)
{
#if defined(LINUX) || defined(WINDOWS)
  CCNEW(pSimulation, CcGpioPortSimulation);
  CcKernel::addDevice(CcDevice(pSimulation, EDeviceType::GpioPort));
#endif
}

CcRemoteDeviceServerApp::~CcRemoteDeviceServerApp()
{
#if defined(LINUX) || defined(WINDOWS)
  CCDELETE(pSimulation);
#endif
}

void CcRemoteDeviceServerApp::run()
{
  CcStatus oStatus(true);
  bool bRunService = false;
  CCDEBUG("CcInstallServiceApp::run");
  size_t i = 1;
  if(m_oArguments.size() > i)
  {
    if (m_oArguments[i].compareInsensitve("Reboot"))
    {
      oStatus = CcKernel::getSystem().restart();
    }
    else if (m_oArguments[i].compareInsensitve("Stop"))
    {
      i++;
      CcString sServiceName = CcRemoteDeviceGlobals::Names::ServiceName;
      if (i < m_oArguments.size())
      {
        sServiceName = m_oArguments[i];
      }
      CcService oService(sServiceName, nullptr);
      oStatus = CcKernel::getServiceSystem().stop(&oService);
    }
    else if (m_oArguments[i].compareInsensitve("Remove"))
    {
      i++;
      CcString sServiceName = CcRemoteDeviceGlobals::Names::ServiceName;
      if (i < m_oArguments.size())
      {
        sServiceName = m_oArguments[i];
      }
      CcService oService(sServiceName, nullptr);
      oStatus = CcKernel::getServiceSystem().remove(&oService);
    }
    else if (m_oArguments[i].compareInsensitve("Start"))
    {
      i++;
      CcString sServiceName = CcRemoteDeviceGlobals::Names::ServiceName;
      if (i < m_oArguments.size())
      {
        sServiceName = m_oArguments[i];
      }
      CcService oService(sServiceName, nullptr);
      oStatus = CcKernel::getServiceSystem().start(&oService);
    }
    else if (m_oArguments[i].compareInsensitve("Create"))
    {
      i++;
      bool bAutostart = true;
      CcString sServiceName = CcRemoteDeviceGlobals::Names::ServiceName;
      for (;i < m_oArguments.size(); i++)
      {
        if (m_oArguments[i].compareInsensitve("-name"))
        {
          i++;
          if (i < m_oArguments.size())
          {
            sServiceName = m_oArguments[i];
          }
          else
          {
            oStatus = EStatus::CommandRequiredParameter;
            CCERROR("-name requires a valid service name");
          }
        }
        else if (m_oArguments[i].compareInsensitve("-manual"))
        {
          bAutostart = false;
        }
        else if (m_oArguments.size() == i+1)
        {
          sServiceName = m_oArguments[i];
        }
        else
        {
          oStatus = EStatus::CommandUnknownParameter;
          CCERROR("Unknown parameter for service create");
        }
      }

      if (oStatus)
      {
        CcService oService(sServiceName, nullptr);
        oStatus = CcKernel::getServiceSystem().create(&oService);
        if (oStatus)
          oStatus = CcKernel::getServiceSystem().setAutoStart(&oService, bAutostart);
      }
    }
    else
    {
      oStatus = EStatus::CommandUnknownParameter;
      CCERROR("Unknown parameter for service create");
    }
  }
  else
  {
    bRunService = true;
  }

  if(bRunService)
  {
    CcRemoteDeviceServer::run();
  }

  if (getExitCode() && !oStatus)
  {
    setExitCode(oStatus);
  }
}
