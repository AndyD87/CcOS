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
 * @page      Driver
 * @subpage   IEeprom
 *
 * @page      IEeprom
 * @par       Language: C++11
 * @brief     Class IEeprom
 */

#ifndef H_IEeprom_H_
#define H_IEeprom_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IIo.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED IEeprom : public IIo
{
public:
  /**
   * @brief Destructor
   */
  virtual ~IEeprom() = default;

};

#endif // _IEeprom_H_
