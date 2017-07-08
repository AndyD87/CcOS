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
 * @subpage   CcDeviceListItem
 *
 * @page      CcDeviceListItem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcDeviceListItem
 */
#ifndef CcDeviceListItem_H_
#define CcDeviceListItem_H_

#include "CcBase.h"
#include "CcDevice.h"
#include "CcKernelBase.h"

#ifdef WIN32
template class CcKernelSHARED CcHandle<CcDevice>;
#endif

/**
* @brief Handles all devices and Interfaces connected to Kernel
*/
class CcKernelSHARED CcDeviceListItem
{
public:
  EDeviceType Type = EDeviceType::All;  //!< Type of Device containing in struct
  CcHandle<CcDevice> Device = NULL;              //!< Pointer to Device

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcDeviceListItem& oToCompare) const
    { return Device == oToCompare.Device;}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcDeviceListItem& oToCompare) const
    { return Device != oToCompare.Device;}
};

#endif /* CcDeviceListItem_H_ */
