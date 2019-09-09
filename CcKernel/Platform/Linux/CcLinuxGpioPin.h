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
 * @page      Linux
 * @subpage   CcLinuxGpioPin
 *
 * @page      CcLinuxGpioPin
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxGpioPin
 */

#ifndef H_CcLinuxGpioPin_H_
#define H_CcLinuxGpioPin_H_

#include "Devices/IGpioPin.h"
#include "CcString.h"
#include "CcFile.h"

class CcLinuxGpioPin : public IGpioPin {
public:
  CcLinuxGpioPin(uint8 nr);
  virtual ~CcLinuxGpioPin();

  void init();

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param eDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection( EDirection eDirection) override;

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

#endif // H_CcLinuxGpioPin_H_
