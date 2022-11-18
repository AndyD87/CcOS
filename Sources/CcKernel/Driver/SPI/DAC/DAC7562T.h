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
 * @brief     Class DAC7562T
 */

#pragma once
#include "CcBase.h"
#include "Devices/ISpi.h"

/**
 * @brief Driver for DAC7562T Chip
 */
class DAC7562T
{
public:
  /**
   * @brief Create DAC with target spi interface
   * @para pSpiDevice: Target interface for communication
   */
  DAC7562T(ISpi* pSpiDevice);

  /**
   * @brief Remove ADC
   */
  ~DAC7562T();

  /**
   * @brief Initialize DAC with it's init sequence and basic settings
   * @return True if init succeeded
   */
  bool Init();

  bool SetChannel(uint8_t uiChannel, uint16_t uiValue);

private:
  ISpi* m_pSpi;
};
