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
 * @subpage   CcDeviceGrayscaleMap
 *
 * @page      CcDeviceGrayscaleMap
 * @par       Language: C++11
 * @brief     Class CcDeviceGrayscaleMap
 */

#ifndef H_CcDeviceGrayscaleMap_H_
#define H_CcDeviceGrayscaleMap_H_

#include "CcBase.h"
#include "Devices/IGrayscaleMap.h"
#include "Devices/CcDevice.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcDevice<IGrayscaleMap>;
#endif

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcDeviceGrayscaleMap : public CcDevice<IGrayscaleMap>
{
public:
  CcDeviceGrayscaleMap(const CcDeviceHandle& oHandle) :
    CcDevice<IGrayscaleMap>(oHandle)
  {}
  virtual ~CcDeviceGrayscaleMap() = default;
};

#endif // _CcDeviceGrayscaleMap_H_
