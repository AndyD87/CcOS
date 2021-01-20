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
 * @brief     Class IEeprom
 */
#pragma once

#include "CcBase.h"
#include "IIo.h"
#include "IDevice.h"

/**
 * @brief Eeprom device wich is also an io device with methods to overload
 *        like read and write.
 */
class CcKernelSHARED IEeprom : public IDevice, public IIo
{
public:
  IEeprom() = default;
  virtual ~IEeprom() = default;

  /**
   * @brief Get size of current eeprom
   * @return size of eeprom
   */
  virtual size_t size() const = 0;

  /**
   * @brief Set postion of pointer for next read/write
   * @param uiPosition: New position.
   * @return Status of operation.
   */
  virtual CcStatus setPosition(size_t uiPosition) = 0;

  /**
   * @brief Get size of an single sector of eeprom to optimize read/write requests.
   * @return Number of bytes per sektor
   */
  virtual size_t getSectorSize() const
  { return size(); }
};
