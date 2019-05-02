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
 * @subpage   IGpioPort
 *
 * @page      IGpioPort
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IGpioPort
 */

#ifndef _IGpioPort_H_
#define _IGpioPort_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IDevice.h"
#include "IGpioPin.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED IGpioPort : public IDevice
{
public:
  /**
   * @brief Destructor
   */
  virtual ~IGpioPort() = default;

  virtual uint8 count() const = 0;

  /**
   * @brief Get Pin at Position uiNr
   * @param uiNr: number of requested Pin in Port
   * @return Handle to Pin or NULL if not existing,
   */
  virtual IGpioPin* getPin(uint8 uiNr) = 0;

  virtual void setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0);
};

#endif /* _IGpioPort_H_ */
