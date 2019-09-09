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
 * @page      STM3220GEVAL
 * @subpage   STM3220GEVALDriver
 *
 * @page      STM3220GEVALDriver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef H_STM3220GEVALDriver_H_
#define H_STM3220GEVALDriver_H_

#include <Driver/Board/ST/STM3220GEVAL/STM3220GEVAL.h>
#include "IDriver.h"

class CcByteArray;

/**
 * @brief Generate SM32F407V CPU Device
 */
class STM3220GEVALDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  STM3220GEVALDriver();

  /**
   * @brief Destructor
   */
  virtual ~STM3220GEVALDriver();

  virtual CcStatus entry() override;
};

#endif // H_STM3220GEVALDriver_H_
