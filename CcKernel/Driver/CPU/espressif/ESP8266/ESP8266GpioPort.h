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
 * @brief     Class ESP8266GpioPort
 */
#pragma once

#include "CcBase.h"
#include "Devices/IGpioPort.h"
#include "CcVector.h"

/**
 * @brief Only one port is defined for all pins on ESP8266
 */
class ESP8266GpioPort : public IGpioPort
{
public:
  /**
   * @brief Alternate values to enable for special functions on pins
   */
  enum class EAlternate
  {
    SPI = 2
  };
public:
  ESP8266GpioPort();
  virtual ~ESP8266GpioPort();
  virtual uint8 count() const override;
  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;
  virtual IGpioPin::EDirection getDirection(size_t uiPin) override;
  virtual bool setValue(size_t uiPin, bool bValue) override;
  virtual bool getValue(size_t uiPin) override;
private:
  class CPin;
private:
  CcVector<CPin*>                 m_oPins;
  CcVector<IGpioPin::EDirection>  m_oDirections;
};
