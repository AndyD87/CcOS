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

#ifndef H_STM32F407VETBoardDriver_H_
#define H_STM32F407VETBoardDriver_H_

#include "STM32F407VETBoard.h"
#include "IDriver.h"

class CcByteArray;

/**
 * @brief Generate SM32F407V CPU Device
 */
class STM32F407VETBoardDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  STM32F407VETBoardDriver();

  /**
   * @brief Destructor
   */
  virtual ~STM32F407VETBoardDriver();

  virtual CcStatus entry() override;
  virtual CcStatus unload() override;
};

#endif // H_STM32F407VETBoardDriver_H_
