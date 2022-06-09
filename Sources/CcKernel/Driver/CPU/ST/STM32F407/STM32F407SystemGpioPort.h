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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class STM32F407SystemGpioPort
 */
#pragma once

#include "CcBase.h"
#include "STM32F407.h"
#include "Devices/IGpioPort.h"

//! _Number of pins per port
#define NUMBER_OF_PINS 32

class IGpioPin;

/**
 * @brief Port interfaces for STM32F407
 */
class STM32F407SystemGpioPort : public IGpioPort
{
public: //methods
  /**
   * @brief Initialize interface by number:
   * @param uiPort: Target port number
   */
  STM32F407SystemGpioPort(uint8 uiPort);
  virtual ~STM32F407SystemGpioPort();

  virtual CcStatus onState(EState eState) override;

  virtual uint8 count() const override
  { return NUMBER_OF_PINS;}
  virtual IGpioPin* getPin(uint8 uiNr) override;

  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;

  /**
   * @brief Set direction of a pin on this port
   * @param uiPin:      Target pin to set direction for
   * @param eDirection: New configuration for pin to set.
   * @param uiValue:    Define alternative function with that value
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual IGpioPin::EDirection getDirection(size_t uiPin) override;

  virtual bool setPinValue(size_t uiPin, bool bValue) override;
  virtual bool getPinValue(size_t uiPin) override;
  virtual bool setSpeedValue(size_t uiPin, size_t uiValue) override;

private: // Member
  GPIO_TypeDef* m_pPort;
  IGpioPin*     m_aPins[NUMBER_OF_PINS] = {nullptr};
};