/*
 *
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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class CcLinuxDbus
 */
#pragma once

#include "Devices/IGpioPin.h"
#include "CcString.h"
#include "CcFile.h"

/**
 * @brief Linux dbus handler. Currently without function.
 */
class CcLinuxDbus
{
public:
  CcLinuxDbus();
  ~CcLinuxDbus();

  /**
   * @brief Connect to dbus
   */
  void init();

  /**
   * @brief Disconnect from dbus
   */
  void deinit();
private:
  class CPrivate;
  CPrivate* m_pPrivate;
};
