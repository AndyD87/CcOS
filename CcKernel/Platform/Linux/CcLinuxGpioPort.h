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
 * @brief     Class CcLinuxGpioPort
 */
#pragma once

#include "Devices/IGpioPort.h"
#include "CcLinuxGpioPin.h"
#include "CcVector.h"


/**
 * @brief Simulate an device port with pins on linux.
 *        Each pin could be connected to a custom output pin in /sys
 */
class CcLinuxGpioPort : public IGpioPort
{
public:
  CcLinuxGpioPort()           = default;
  virtual ~CcLinuxGpioPort()  = default;

  virtual inline uint8 count() const override
  { return 0xff; }
  virtual IGpioPin* getPin(uint8 uiNr) override;
  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;
  virtual IGpioPin::EDirection getDirection(size_t uiPin) override;
  virtual bool setPinValue(size_t uiPin, bool bValue) override;
  virtual bool getPinValue(size_t uiPin) override;
private:
  /**
   * @brief Connection pin nr in /sys and pin intercace.
   */
  class CGpioPinItem
  {
  public:
    uint8 uiNr;     //!< Target system pin
    IGpioPin *cPin; //!< Interface to pin in linux system
  };
  CcVector<CGpioPinItem> m_lcPins;
};
