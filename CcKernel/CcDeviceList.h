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
 * @subpage   CcDeviceList
 *
 * @page      CcDeviceList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDeviceList
 */
#ifndef _CcDeviceList_H_
#define _CcDeviceList_H_

#include "CcBase.h"
#include "CcDevice.h"
#include "CcHandle.h"
#include "CcList.h"
#include "CcDeviceListItem.h"
#include "CcKernelBase.h"

/**
 * @brief Handles all devices and Interfaces connected to Kernel
 */
class CcKernelSHARED CcDeviceList : public CcList<CcDeviceListItem>
{
public:
  /**
   * @brief Constructor
   */
  CcDeviceList() = default;

  /**
   * @brief Destructor
   */
  ~CcDeviceList() = default;

  CcDeviceList& append(EDeviceType eType, CcDeviceHandle cDevice);
  CcDeviceHandle getDevice(EDeviceType eType);
  CcDeviceHandle getDevice(EDeviceType eType, size_t uiNr);
};

#endif /* _CcDeviceList_H_ */
