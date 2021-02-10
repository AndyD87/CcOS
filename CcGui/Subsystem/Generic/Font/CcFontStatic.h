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
 * @brief     Class CcFontStatic
 */
#pragma once

#include "CcBase.h"

/**
 * @brief Static methods for handling with fonts
 */
class CcFontStatic
{
public:
  /**
   * @brief Swap 16 bit
   * @param iInput: input to swap
   * @return Swapped output
   */
  static int16 swapInt16(int16 iInput);

  /**
   * @brief Swap 32 bit
   * @param iInput: input to swap
   * @return Swapped output
   */
  static int32 swapInt32(int32 iInput);

  /**
   * @brief Swap 64 bit
   * @param iInput: input to swap
   * @return Swapped output
   */
  static int64 swapInt64(int64 iInput);

  /**
   * @brief Swap 16 bit
   * @param uiInput: input to swap
   * @return Swapped output
   */
  static uint16 swapUint16(uint16 uiInput);

  /**
   * @brief Swap 32 bit
   * @param uiInput: input to swap
   * @return Swapped output
   */
  static uint32 swapUint32(uint32 uiInput);

  /**
   * @brief Swap 64 bit
   * @param uiInput: input to swap
   * @return Swapped output
   */
  static uint64 swapUint64(uint64 uiInput);

  /**
   * @brief Convert font uint16 value to float value
   * @param uiInput: 16 bit input to convert
   * @return Result of convertion.
   */
  static float  uint16ToFloat(uint16 uiInput);

  /**
   * @brief Swap buffer in a specific size
   * @param pBuffer: buffer to swap
   * @param uiSize: Size in bytes to swap
   */
  static void swap(void *pBuffer, size_t uiSize);
};
