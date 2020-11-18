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
 * @page      CcKernel
 * @subpage   CcDeviceIoDevice
 *
 * @page      CcDeviceIoDevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceIoDevice
 */

#ifndef H_CcDeviceIoDevice_H_
#define H_CcDeviceIoDevice_H_

#include "CcBase.h"
#include "Devices/IIoDevice.h"
#include "Devices/CcDevice.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcDevice<IIoDevice>;
#endif

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED CcDeviceIoDevice : public CcDevice<IIoDevice>
{
public:
  CcDeviceIoDevice(const CcDeviceHandle& oHandle) :
    CcDevice<IIoDevice>(oHandle)
  {}
  virtual ~CcDeviceIoDevice() = default;
};

#endif // _CcDeviceIoDevice_H_
