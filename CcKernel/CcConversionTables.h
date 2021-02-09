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
 * @brief     Class CcConversionTables
 */
#pragma once

#include "CcBase.h"

/**
 * @brief Static class for Console in- and output.
 *        The default target for this class is stdin and stdout, but can be changed during runtime.
 */
class CcKernelSHARED CcConversionTables
{
public:
  class CPrivate;

  /**
   * @brief Get value at index of an 8 bit sinewave table
   * @param uiNumber: Target index, max 255
   * @return Converted table
   */
  static uint8  convertU8ToSineWaveU8(uint8 uiNumber);

  /**
   * @brief Get 8 bit value of an 12bit logarithmic increasing table.
   *        It can be used to simulate an linear brightness increasing with PWMs
   * @param uiNumber: Index of 8bit table, max 255
   * @return Converted uint12 value
   */
  static uint16 convertU8LogToU12(uint8 uiNumber);
};
