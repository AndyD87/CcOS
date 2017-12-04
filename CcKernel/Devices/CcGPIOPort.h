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
 * @subpage   CcGPIOPort
 *
 * @page      CcGPIOPort
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGPIOPort
 */

#ifndef CcGPIOPort_H_
#define CcGPIOPort_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcDevice.h"
#include "CcGPIOPin.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcGPIOPort : public CcDevice {
public:
  /**
   * @brief Constructor
   */
  CcGPIOPort();

  /**
   * @brief Destructor
   */
  virtual ~CcGPIOPort();

  virtual uint8 count() = 0;

  /**
   * @brief Get Pin at Position uiNr
   * @param uiNr: number of requested Pin in Port
   * @return Handle to Pin or NULL if not existing,
   */
  virtual CcGPIOPin* getPin(uint8 uiNr) = 0;
};

#endif /* CcGPIOPort_H_ */
