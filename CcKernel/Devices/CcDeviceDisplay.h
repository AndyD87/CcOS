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
 * @subpage   CcDeviceDisplay
 *
 * @page      CcDeviceDisplay
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceDisplay
 */
#ifndef H_CcDeviceDisplay_H_
#define H_CcDeviceDisplay_H_

#include "CcBase.h"
#include "Devices/IDisplay.h"
#include "CcDevice.h"

/**
 * @brief This class describes an Display Device wich is possible to
 *        show dot matrix.
 */
class CcKernelSHARED CcDeviceDisplay : public CcDevice
{
public:
  CcDeviceDisplay(const CcDevice& oHandle) :
    CcDevice(oHandle)
  {}
  virtual ~CcDeviceDisplay() = default;
  
  IDisplay* getDevice() const
  { return CcDevice::getDevice<IDisplay>(); }

};

#endif // _CcDeviceDisplay_H_
