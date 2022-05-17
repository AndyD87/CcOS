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
 * @par       Language: C++11
 * @brief     Class CcDeviceHdd
 */
#pragma once

#include "CcBase.h"
#include "Devices/IHdd.h"
#include "CcDevice.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDeviceHdd : public CcDevice
{
public:
  /**
   * @brief Create device with handle
   * @param oHandle: Handle to init device
   */
  CcDeviceHdd(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceHdd() = default;

  //! @return Get basic device handle
  IHdd* getDevice() const
  { return CcDevice::getDevice<IHdd>(); }

};
