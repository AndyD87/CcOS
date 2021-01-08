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
 * @subpage   CcDeviceI2C
 *
 * @page      CcDeviceI2C
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceI2C
 */
#ifndef H_CcDeviceI2C_H_
#define H_CcDeviceI2C_H_

#include "CcBase.h"
#include "Devices/II2C.h"
#include "CcDevice.h"

/**
 * @brief Communication Device for I2C
 */
class CcKernelSHARED CcDeviceI2C : public CcDevice
{
public:
  CcDeviceI2C(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceI2C() = default;
  
  II2C* getDevice() const
  { return CcDevice::getDevice<II2C>(); }
};

#endif // H_CcDeviceI2C_H_
