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
 * @page      CcKernel
 * @subpage   IDriver
 *
 * @page      IDriver
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IDriver
 */
#ifndef H_IDriver_H_
#define H_IDriver_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcStatus.h"

/**
 * @brief Default Class to create a Application
 */
class CcKernelSHARED IDriver 
{
public:

  /**
   * @brief Default virual destructor
   */
  virtual ~IDriver() = default;

  /**
   * @brief Entry point must be created from every new driver
   */
  virtual CcStatus entry() = 0;

  /**
   * @brief Unload will be called when driver will be removed.
   */
  virtual CcStatus unload() = 0;
};

#endif // _IDriver_H_
