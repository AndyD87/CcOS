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

#ifndef H_IGpioPort_H_
#define H_IGpioPort_H_

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
  virtual IGpioPin* getPin(uint8 uiNr);

  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) = 0;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param EDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue = 0) = 0;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual IGpioPin::EDirection getDirection(size_t uiPin) = 0;

  virtual bool setValue(size_t uiPin, bool bValue) = 0;
  virtual bool getValue(size_t uiPin) = 0;
  virtual bool setSpeedValue(size_t uiPin, size_t uiValue);

  static const size_t Pin0;
  static const size_t Pin1;
  static const size_t Pin2;
  static const size_t Pin3;
  static const size_t Pin4;
  static const size_t Pin5;
  static const size_t Pin6;
  static const size_t Pin7;
  static const size_t Pin8;
  static const size_t Pin9;
  static const size_t Pin10;
  static const size_t Pin11;
  static const size_t Pin12;
  static const size_t Pin13;
  static const size_t Pin14;
  static const size_t Pin15;
  static const size_t Pin16;
  static const size_t Pin17;
  static const size_t Pin18;
  static const size_t Pin19;
  static const size_t Pin20;
  static const size_t Pin21;
  static const size_t Pin22;
  static const size_t Pin23;
  static const size_t Pin24;
  static const size_t Pin25;
  static const size_t Pin26;
  static const size_t Pin27;
  static const size_t Pin28;
  static const size_t Pin29;
  static const size_t Pin30;
  static const size_t Pin31;
};

#endif // _IGpioPort_H_
