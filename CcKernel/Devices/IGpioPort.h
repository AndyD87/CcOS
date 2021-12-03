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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IGpioPort
 */
#pragma once

#include "CcBase.h"
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

  /**
   * @brief Number of pins available on this port.
   * @return Number of pins.
   */
  virtual uint8 count() const = 0;

  /**
   * @brief Get or generate pin interface at Position uiNr
   * @param uiNr: number of requested Pin in Port
   * @return Handle to Pin or NULL if not existing,
   */
  virtual IGpioPin* getPin(uint8 uiNr);

  /**
   * @brief Set direction of multiple pins at once
   * @param uiPinMask:  Target pins ORed to mask
   * @param eDirection: Direction to apply to pins
   * @param uiValue:    Additional value information for direction, like alternate function.
   * @return True if operation succeded.
   */
  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) = 0;

  /**
   * @brief Initialize basic settings for General Purpose Input Output of single pin
   *        If not overrided, the method will forward to setPinsDirection with one pin
   * @param uiPin:      Target pin to set
   * @param eDirection: New configuration for pin to set.
   * @param uiValue:    Additional context value to set, specific on cpu/board. Default 0
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue = 0);

  /**
   * @brief Get current direction of Pin
   * @return Current direction of Pin.
   */
  virtual IGpioPin::EDirection getDirection(size_t uiPin) = 0;

  /**
   * @brief Set value of a single pin.
   * @param uiPin:  Target pin
   * @param bValue: High(true) or Low(false) value to set.
   * @return true if operation succeeded
   */
  virtual bool setPinValue(size_t uiPin, bool bValue) = 0;

  /**
   * @brief Set value of one specific pin.
   * @param uiPin: Target pin to get value
   * @return True if value is High, otherwise False for Low
   */
  virtual bool getPinValue(size_t uiPin) = 0;

  /**
   * @brief Set Speed value for one pin.
   *        Look at IGpioPin::setSpeed for further information
   * @param uiPin:    Target pin to set.
   * @param uiValue:  Speed value to set.
   * @return True if operation succeeded.
   */
  virtual bool setSpeedValue(size_t uiPin, size_t uiValue);

  virtual bool setValue(size_t uiValue);

  static const size_t Pin0;  //!< Intger value for pin 00
  static const size_t Pin1;  //!< Intger value for pin 01
  static const size_t Pin2;  //!< Intger value for pin 02
  static const size_t Pin3;  //!< Intger value for pin 03
  static const size_t Pin4;  //!< Intger value for pin 04
  static const size_t Pin5;  //!< Intger value for pin 05
  static const size_t Pin6;  //!< Intger value for pin 06
  static const size_t Pin7;  //!< Intger value for pin 07
  static const size_t Pin8;  //!< Intger value for pin 08
  static const size_t Pin9;  //!< Intger value for pin 09
  static const size_t Pin10; //!< Intger value for pin 10
  static const size_t Pin11; //!< Intger value for pin 11
  static const size_t Pin12; //!< Intger value for pin 12
  static const size_t Pin13; //!< Intger value for pin 13
  static const size_t Pin14; //!< Intger value for pin 14
  static const size_t Pin15; //!< Intger value for pin 15
  static const size_t Pin16; //!< Intger value for pin 16
  static const size_t Pin17; //!< Intger value for pin 17
  static const size_t Pin18; //!< Intger value for pin 18
  static const size_t Pin19; //!< Intger value for pin 19
  static const size_t Pin20; //!< Intger value for pin 20
  static const size_t Pin21; //!< Intger value for pin 21
  static const size_t Pin22; //!< Intger value for pin 22
  static const size_t Pin23; //!< Intger value for pin 23
  static const size_t Pin24; //!< Intger value for pin 24
  static const size_t Pin25; //!< Intger value for pin 25
  static const size_t Pin26; //!< Intger value for pin 26
  static const size_t Pin27; //!< Intger value for pin 27
  static const size_t Pin28; //!< Intger value for pin 28
  static const size_t Pin29; //!< Intger value for pin 29
  static const size_t Pin30; //!< Intger value for pin 30
  static const size_t Pin31; //!< Intger value for pin 31
};
