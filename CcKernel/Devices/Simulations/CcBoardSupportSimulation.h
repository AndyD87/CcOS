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
 * @brief     Class CcSpiSimulation
 */
#pragma once

#include "CcBase.h"
#include "Devices/IBoardSupport.h"

/**
 * @brief Setup test hardware board with:
 *        - 2 Hardware ports
 *        - 20 Pins each port
 *        - 3 Example Functions with I2C, SPI and UART
 *        - I2C and SPI are Multiplexed and mapped to 16 GPIO Pins each port
 */
class CcBoardSupportSimulation : public IBoardSupport
{
public:
  /**
   * @brief Constructor
   */
  CcBoardSupportSimulation();

  /**
   * @brief Destructor
   */
  virtual ~CcBoardSupportSimulation();

  virtual CcDevice createDevice(EDeviceType eDeviceType, uint32 uiDeviceNumber) override;
};
