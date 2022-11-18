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
  * @copyright Andreas Dirmeier (C) 2021
  * @author    Andreas Dirmeier
  * @par       Web:      https://coolcow.de/projects/CcOS
  * @par       Language: C++11
  * @brief     Class PCF8574
  */

#pragma once
#include "CcBase.h"
#include "Devices/II2C.h"

/**
 * @brief Access class to PCF8574 I2C Port Expander
 */
class PCF8574
{
public:
  /**
   * @brief Create portexpander with I2C Device.
   * @param pIfc:       Interface to target I2C Port
   * @param uiAddress:  Client address on I2C Port
   */
  PCF8574(II2CSlave* pIfc);

  /**
   * @brief Close all handles to interface
   */
  ~PCF8574();

  /**
   * @brief Set value of specific pin
   * @param uiNr:   Target pin to set
   * @param bValue: Value for target pin
   */
  void setPin(uint8 uiNr, bool bValue);

  /**
   * @brief Get value of specific pin.
   *        On I2C Interface, the value will not be send directly.
   *        A call to write() is required;
   * @param uiNr: Target pin to get value from
   * @return Value of pin or 0 if not found.
   */
  bool getPin(uint8 uiNr);

  /**
   * @brief Set the whole Port Expander Values at once
   * @param uiValue: Target Bitmask to set on PortExpander
   */
  void setPort(uint8 uiValue);

  /**
   * @brief Get the whole Port Expander Value at once
   * @return Value of PortExpander as bitmask
   */
  uint8 getPort();

  /**
   * @brief Write stored pin settings to PortExpander.
   *        No longer required with gpiod
   * @return True if write was successfully
   */
  bool write();
private:
  II2CSlave*  m_pIfc = nullptr;
  uint8       m_uiValue = 0xff; //! Dafault value as described in datasheet
};
