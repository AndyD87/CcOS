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
 * @page      Devices
 * @subpage   CcDeviceHdd
 *
 * @page      CcDeviceHdd
 * @par       Language: C++11
 * @brief     Class CcDeviceHdd
 */

#ifndef H_CcDeviceHdd_H_
#define H_CcDeviceHdd_H_

#include "CcBase.h"
#include "Devices/IHdd.h"
#include "CcDevice.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDeviceHdd : public CcDevice
{
public:
  CcDeviceHdd(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceHdd() = default;
  
  IHdd* getDevice()
  { return CcDevice::getDevice<IHdd>(); }

};

#endif // _CcDeviceHdd_H_
