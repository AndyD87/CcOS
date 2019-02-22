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
 * @subpage   CcDevice
 *
 * @page      CcDevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDevice
 */

#ifndef _CcDevice_H_
#define _CcDevice_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Types/CcHandle.h"

class CcDevice;
#ifdef WIN32
template class CcKernelSHARED CcHandle<CcDevice>;
#endif
typedef CcHandle<CcDevice> CcDeviceHandle;

/**
 * @brief Enumeration of Known Devices.
 */
enum class EDeviceType {
  All = 0,     //!< this defines All Devices if requesting for.
  Uart,        //!< Universal Asynchronous Receivce Transmit Device.
  Spi,         //!< Serial Protocol Interface Device
  I2C,         //!< I2C-Device
  Display,     //!< LCD-Device
  TouchPanel,  //!< TouchPanel-Device
  Ethernet,    //!< Network-Device
  Timer,       //!< Timer-Device
  Camera,      //!< Camera-Modul as Device
  Led,         //!< single LED-Device.
  Hdd,          //!< Device is a Hard Disk Drive
  GPIOPort     //!< Get a Device connected with a range of GPIO-Pins
};

/**
 * @brief Basic Class for all Devices in System.
 */
class CcKernelSHARED CcDevice 
{
public:
  /**
   * @brief Constructor
   */
  CcDevice() = default;

  /**
   * @brief Destructor
   */
  virtual ~CcDevice() = default;
};

#endif /* _CcDevice_H_ */
