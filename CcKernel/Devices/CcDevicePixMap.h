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
 * @subpage   CcDevicePixMap
 *
 * @page      CcDevicePixMap
 * @par       Language: C++11
 * @brief     Class CcDevicePixMap
 */

#ifndef H_CcDevicePixMap_H_
#define H_CcDevicePixMap_H_

#include "CcBase.h"
#include "Devices/IPixMap.h"
#include "Devices/CcDevice.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcDevice<IPixMap>;
#endif

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDevicePixMap : public CcDevice<IPixMap>
{
public:
  CcDevicePixMap(const CcDeviceHandle& oHandle) :
    CcDevice<IPixMap>(oHandle)
  {}
  virtual ~CcDevicePixMap() = default;
};

#endif // _CcDevicePixMap_H_
