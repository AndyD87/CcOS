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
 * @brief     IDevice::CContext Implementation
 */
#pragma once

#include "CcKernelModuleContext.h"
#include "IDevice.h"
#include "IDeviceInterface.h"

namespace NKernelModule
{
/**
 * @brief IDevice::CContext definition for linux
 */
class IDevice::CContext
{
public:
  IDriver*                pDriver;      //!< Pointer to parent driver
  IDevice*                pDevice;      //!< Pointer to device interface
  IDeviceInterface        oNextDevice;  //!< Pointer to next lower device

  //!< @brief Get list of interfaces opened to this device
  CcList<CcConnection*>* getConnections()
  { if (!pConnections) CCNEW(pConnections, CcList<CcConnection*>); return pConnections; }

private:
  CcList<CcConnection*>*  pConnections;
};

}
