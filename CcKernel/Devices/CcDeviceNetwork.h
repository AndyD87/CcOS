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
 * @brief     Class CcDeviceNetwork
 */
#pragma once

#include "CcBase.h"
#include "Devices/INetwork.h"
#include "CcDevice.h"

/**
 * @brief Abstract device-class for connecting with a TouchPanel
 */
class CcKernelSHARED CcDeviceNetwork : public CcDevice
{
public:
  /**
   * @brief Create device with handle
   * @param oHandle: Handle to init device
   */
  CcDeviceNetwork(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceNetwork() = default;

  //! @return Get basic device handle
  INetwork* getDevice() const
  { return CcDevice::getDevice<INetwork>(); }
};
