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
 * @brief     Class IDriver
 */
#pragma once

#include "CcBase.h"
#include "CcStatus.h"
#include "CcDevice.h"
#include "CcVector.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<IDevice*>;
#endif


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
   *        All devices in m_oDevices will be deleted here.
   *        If overloaded the derived class has to delete it self or has to call this method.
   */
  virtual CcStatus unload();

protected:
  CcVector<IDevice*> m_oDevices;  //!< Storage of all devices created by this driver
                                  //! They will be deleted on unload
};
