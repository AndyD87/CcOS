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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class OrangePiZeroGpioPin
 */
#pragma once

#include "CcBase.h"
#include "STM32F407.h"
#include "Devices/IGpioPin.h"

class OrangePiZeroGpioPinPrivate;

/**
 * @brief Create pin on port of STM32F407 to manage with CcOS
 */
class OrangePiZeroGpioPin : public IGpioPin
{
public: //methods
  /**
   * @brief Create pin for STM32F407
   * @param pPort:    Port of this pin
   * @param uiPinNr:  Pin number on port pPort.
   */
  OrangePiZeroGpioPin(void* pPort, uint8 uiPinNr);
  virtual ~OrangePiZeroGpioPin();

  virtual bool setDirection( EDirection eDirection, size_t uiValue = 0) override;
  virtual EDirection getDirection() override;
  virtual void setValue(bool bValue) override;
  virtual bool getValue() override;
  virtual bool toggle() override;
  virtual bool setSpeedValue(size_t uiValue) override;

  /**
   * @brief Force reconfiguration of pin settings
   */
  void reconfigure();
private: // Member
  GPIO_TypeDef* m_pPort;
  uint32 m_uiPinNr;
  GPIO_InitTypeDef m_oGpioInitStruct;
};
