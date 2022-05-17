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
 * @brief     Class IProgressReceiver
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief Class implementation
 */
class IProgressReceiver
{
public:
  virtual ~IProgressReceiver() = default;

  /**
   * @brief This method will called with a value and it's maximum.
   *        Both values can be used to  generate a progress.
   *        Units are define from sender.
   * @param uiValue: Value of current state
   * @param uiFrom:  Target state fo uiValue
   */
  virtual void update(uint64 uiValue, uint64 uiFrom) = 0;
};
