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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxDbusModule
 */
#pragma once

#include "IModule.h"
#include "CcLinuxDbus.h"

/**
 * @brief Create dbus module for loading from CcOS
 */
class CcLinuxDbusModule : public IModule
{
public:
  CcLinuxDbusModule() = default;
  virtual ~CcLinuxDbusModule() = default;

  /**
   * @brief Initialize dbus module
   * @return Success if initializing succeeded
   */
  virtual CcStatus init() override;

  /**
   * @brief Deinitialize and cleanup dbus module
   * @return Success if deinitializing failed
   */
  virtual CcStatus deinit() override;

private:
  CcLinuxDbus m_oDbus;
};
