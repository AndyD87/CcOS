/**
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
 * @version   0.01
 * @date      2015-10
 * @par       Language: C++11
 * @brief     Class CcLinuxTouch
 */
#pragma once

#include "CcBase.h"
#include "Devices/ITouch.h"

/**
 * @brief Touch interface on linux, no well implementation yet.
 */
class CcLinuxTouch : public ITouch
{
public:
  CcLinuxTouch() = default;
  virtual ~CcLinuxTouch() = default;

  virtual void getTouchState(uint16& x, uint16& y);
  virtual bool getPressState();
};
