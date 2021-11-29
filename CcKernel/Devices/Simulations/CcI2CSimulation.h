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
 * @brief     Class CcI2CSimulation
 */
#pragma once

#include "CcBase.h"
#include "Devices/II2C.h"
#include "CcByteArray.h"

/**
 * @brief Control the Input and Output ports on device
 */
class CcKernelSHARED CcI2CSlaveSimulation : public II2CSlave
{
public:
  CcI2CSlaveSimulation(uint16 uiAddress) :
    II2CSlave(uiAddress)
  {}

  /**
   * @brief Destructor
   */
  virtual ~CcI2CSlaveSimulation() = default;

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;

private:
  CcByteArray m_oData;
};


/**
 * @brief Control the Input and Output ports on device
 */
class CcKernelSHARED CcI2CSimulation : public II2C
{
public:
  /**
   * @brief Destructor
   */
  virtual ~CcI2CSimulation() = default;

  virtual I2CSlave* createInterface(uint16 uiAddress) override;

};
