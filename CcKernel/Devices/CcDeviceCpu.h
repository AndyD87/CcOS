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
 *
 * @par       Language: C++11
 * @brief     Class CcDeviceCpu
 */

#ifndef H_CcDeviceCpu_H_
#define H_CcDeviceCpu_H_

#include "CcBase.h"
#include "Devices/ICpu.h"
#include "CcDevice.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDeviceCpu : public CcDevice
{
public:
  CcDeviceCpu(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceCpu() = default;
  
  ICpu* getDevice() const
  { return CcDevice::getDevice<ICpu>(); }

};

#endif // _CcDeviceCpu_H_
