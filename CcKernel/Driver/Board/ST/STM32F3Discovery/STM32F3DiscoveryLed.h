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
 *
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */
#pragma once

#include "STM32F4Discovery.h"
#include "Devices/ILed.h"

class CcByteArray;

class STM32F4DiscoveryLedPrivate;

/**
 * @brief Generate SM32F407V CPU Device
 */
class STM32F4DiscoveryLed : public ILed
{
public:
  /**
   * @brief Constructor
   */
  STM32F4DiscoveryLed(uint8 uiLedNr);

  /**
   * @brief Destructor
   */
  virtual ~STM32F4DiscoveryLed();

  virtual CcStatus setMaxBirghtness(uint16 uiBrightness) override;

  virtual CcStatus on(uint16 uiBrightness = 0) override;

  virtual CcStatus off() override;

  virtual CcStatus toggle() override;

  virtual bool IsOn();
private:
  STM32F4DiscoveryLedPrivate* m_pPrivate;
};
