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
 * @subpage   CcDeviceGpioPin
 *
 * @page      CcDeviceGpioPin
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceGpioPin
 */

#ifndef H_CcDeviceGpioPin_H_
#define H_CcDeviceGpioPin_H_

#include "CcBase.h"
#include "Devices/IGpioPin.h"
#include "CcDevice.h"

/**
 * @brief Control for General Purpose Input Output Pins
 */
class CcKernelSHARED CcDeviceGpioPin : public CcDevice
{
public:
  CcDeviceGpioPin(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceGpioPin() = default;
  
  IGpioPin* getDevice() const
  { return CcDevice::getDevice<IGpioPin>(); }

  bool setDirection(IGpioPin::EDirection eDirection, size_t uiValue = 0)
  { if(isValid()) return getDevice()->setDirection(eDirection, uiValue); return false; }
  IGpioPin::EDirection getDirection()
  { if(isValid()) return getDevice()->getDirection(); return IGpioPin::EDirection::Unknown; }
  void setValue(bool bValue)
  { if(isValid()) getDevice()->setValue(bValue); }
  bool getValue()
  { if(isValid()) return getDevice()->getValue(); return false; }
  bool toggle()
  { if(isValid()) return getDevice()->toggle(); return false; }
  bool setSpeedValue(size_t uiValue)
  { if(isValid()) return getDevice()->setSpeedValue(uiValue); return false; }

};

#endif // _CcDeviceGpioPin_H_
