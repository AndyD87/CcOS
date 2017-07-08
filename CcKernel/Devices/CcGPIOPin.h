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
 * @subpage   CcGPIOPin
 *
 * @page      CcGPIOPin
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcGPIOPin
 */

#ifndef CcGPIOPin_H_
#define CcGPIOPin_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Control for General Purpose Input Output Pins
 */
class CcKernelSHARED CcGPIOPin {
public:
  /**
   * @brief Enumerate Pin settings
   */
  enum class EDirection : uint8
  {
    Unknown,
    Alternate,            //!< Pin will become an alternate function, for example SPI, I2C
    Input,                //!< Define pin as an input pin
    Output,               //!< Define pin as an default output-pin
  };

  /**
   * @brief Constructor
   */
  CcGPIOPin();

  /**
   * @brief Destructor
   */
  virtual ~CcGPIOPin();

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param EDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection( EDirection EDirection) = 0;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual EDirection getDirection(void) = 0;

  /**
   * @brief Set Value of Output pin
   * @param bValue: value to set
   */
  virtual void setValue(bool bValue) = 0;

  /**
   * @brief Get current value of pin in Input mode. If Output, last set value should be returned.
   * @return Value of pin.
   */
  virtual bool getValue(void) = 0;
};

#endif /* CcGPIOPin_H_ */
