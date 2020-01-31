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
 * @subpage   IGpioPin
 *
 * @page      IGpioPin
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IGpioPin
 */

#ifndef H_IGpioPin_H_
#define H_IGpioPin_H_

#include "CcBase.h"
#include "CcBase.h"
#include "IDevice.h"

class IGpioPort;

/**
 * @brief Control for General Purpose Input Output Pins
 */
class CcKernelSHARED IGpioPin : public IDevice
{
public:
  /**
   * @brief Enumerate Pin settings
   */
  enum class EDirection : uint8
  {
    Unknown = 0,
    Input,                //!< Define pin as an input pin
    Output,               //!< Define pin as an default output-pin
    Analog,               //!< Pin will become an analog input
    Alternate,            //!< Pin will become an alternate function, for example SPI, I2C
  };

  /**
   * @brief Destructor
   */
  virtual ~IGpioPin() = default;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param EDirection: New configuration for pin to set.
   * @param uiValue:    If new configuration requires a value.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection(EDirection eDirection, size_t uiValue = 0) = 0;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual EDirection getDirection() = 0;

  /**
   * @brief Set Value of Output pin
   * @param bValue: value to set
   */
  virtual void setValue(bool bValue) = 0;

  /**
   * @brief Get current value of pin in Input mode. If Output, last set value should be returned.
   * @return Value of pin.
   */
  virtual bool getValue() = 0;
  virtual bool toggle();
  virtual bool setSpeedValue(size_t uiValue);
};

#endif // _IGpioPin_H_
