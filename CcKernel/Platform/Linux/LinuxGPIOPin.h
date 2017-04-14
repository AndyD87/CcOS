/**
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class LinuxGPIOPin
 */

#ifndef LinuxGPIOPin_H_
#define LinuxGPIOPin_H_

#include "Devices/CcGPIOPin.h"
#include "CcString.h"
#include "CcFile.h"

class LinuxGPIOPin : public CcGPIOPin {
public:
  LinuxGPIOPin(uint8 nr);
  virtual ~LinuxGPIOPin();

  void init( void );

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param eDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  bool setDirection( EDirection eDirection) override;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  EDirection getDirection(void) override;

  /**
   * @brief Set Value of Output pin
   * @param bValue: value to set
   */
  void setValue(bool bValue) override;

  /**
   * @brief Get current value of pin in Input mode. If Output, last set value should be returned.
   * @return Value of pin.
   */
  bool getValue(void) override;
private:
  /**
   * @brief Write "out" to GPIO-File
   * @return true if writing was succeeded. ( No check if it was overtaken correctly )
   */
  bool writeOutput();
  /**
   * @brief Write "in" to GPIO-File
   * @return true if writing was succeeded. ( No check if it was overtaken correctly )
   */
  bool writeInput();
private:
  uint8 m_uiPinNr;    ///< Pin in /sys/class/gpioXX
  CcString m_sPinNr;  ///< Pin-number as String
  CcString m_sPinPath;///< Path to Pin-Directory
};

#endif /* LinuxGPIOPin_H_ */
