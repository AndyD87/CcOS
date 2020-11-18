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
 * @subpage   CcDeviceLed
 *
 * @page      CcDeviceLed
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceLed
 */

#ifndef H_CcDeviceLed_H_
#define H_CcDeviceLed_H_

#include "CcBase.h"
#include "Devices/ILed.h"
#include "Devices/CcDevice.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcDevice<ILed>;
#endif

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDeviceLed : public CcDevice<ILed>
{
public:
  CcDeviceLed(const CcDeviceHandle& oHandle) :
    CcDevice<ILed>(oHandle)
  {}
  virtual ~CcDeviceLed() = default;
};

#endif // H_CcDeviceLed_H_
