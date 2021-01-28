/*
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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxGpioPin
 */
#pragma once

#include "Devices/IGpioPin.h"
#include "CcString.h"
#include "CcFile.h"

/**
 * @brief Device interface for linux pins in /sys/class/gpio/
 */
class CcLinuxGpioPin : public IGpioPin
{
public:
  /**
   * @brief Initialize interface with predefined pin index in /sys/class/gpio/
   * @param uiPinNr: Target pin number
   */
  CcLinuxGpioPin(uint8 uiPinNr);
  virtual ~CcLinuxGpioPin();

  /**
   * @brief Initialize pin by opening file handles etc.,
   *        which are required to enable pin control.
   */
  void init();

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param eDirection: New configuration for pin to set.
   * @param uiValue:    Aditional value for directions settings, not required for pins in linux.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection( EDirection eDirection, size_t uiValue = 0) override;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual EDirection getDirection() override;

  /**
   * @brief Set Value of Output pin
   * @param bValue: value to set
   */
  virtual void setValue(bool bValue) override;

  /**
   * @brief Get current value of pin in Input mode. If Output, last set value should be returned.
   * @return Value of pin.
   */
  virtual bool getValue() override;

  virtual bool toggle() override;
private:
  /**
   * @brief Write "out" to Gpio-File
   * @return true if writing was succeeded. ( No check if it was overtaken correctly )
   */
  bool writeOutput();
  /**
   * @brief Write "in" to Gpio-File
   * @return true if writing was succeeded. ( No check if it was overtaken correctly )
   */
  bool writeInput();
private:
  CcString m_sPinNr;  //!< Pin-number as String
  CcString m_sPinPath;//!< Path to Pin-Directory
};
