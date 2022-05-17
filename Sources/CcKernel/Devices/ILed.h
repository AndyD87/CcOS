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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ILed
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED ILed : public IDevice
{
public:

  virtual ~ILed() = default;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   */
  virtual CcStatus setMaxBirghtness(uint16 uiBrightness) = 0;

  /**
   * @brief Set Toggle time as pwm
   * @param uiOnTime:   On time
   * @param uiOffTime:  Off time
   * @return Status of operation
   */
  virtual CcStatus setToggleTime(uint16 uiOnTime, uint16 uiOffTime)
  { CCUNUSED(uiOnTime); CCUNUSED(uiOffTime); return EStatus::NotSupported; }

  /**
   * @brief Set led value to on with birghtness.
   *        If brightness is not supported: 0 = off, all other on
   * @param uiBrightness: brightness value
   * @return Status of operation
   */
  virtual CcStatus on(uint16 uiBrightness = 255) = 0;

  /**
   * @brief Set led to off
   * @return Status of operation
   */
  virtual CcStatus off() = 0;

  /**
   * @brief Toggle led, on to off or off to on.
   * @return Status of operation
   */
  virtual CcStatus toggle() = 0;

  //! @return True if led is on
  virtual bool IsOn() = 0;

  //! @return True if led is off
  virtual bool IsOff()
    { return !IsOn();}
};
