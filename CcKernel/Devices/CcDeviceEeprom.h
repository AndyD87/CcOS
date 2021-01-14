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
 * @brief     Class CcDeviceEeprom
 */

#ifndef H_CcDeviceEeprom_H_
#define H_CcDeviceEeprom_H_

#include "CcBase.h"
#include "Devices/IEeprom.h"
#include "CcDevice.h"
#include "IIo.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDeviceEeprom : public CcDevice, public IIo
{
public:
  CcDeviceEeprom(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceEeprom() = default;
  
  IEeprom* getDevice() const
  { return CcDevice::getDevice<IEeprom>(); }

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags eOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  size_t size() const;
  CcStatus setPosition(size_t uiPosition);
};

#endif // _CcDeviceEeprom_H_
