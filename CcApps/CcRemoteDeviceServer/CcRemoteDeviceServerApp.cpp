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

CcRemoteDeviceServerApp::CcRemoteDeviceServerApp(int iArgs, char** pArgv) :
  CcRemoteDeviceServer(),
  m_oArguments(iArgs, pArgv)
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
