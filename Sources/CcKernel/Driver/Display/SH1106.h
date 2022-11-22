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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class SH1106
 */
#pragma once

#include "CcBase.h"
#include "SDD1306.h"

/**
 * @brief Display interface to SH1106 Display driver.
 *        Currently not in use.
 */
class CcKernelSHARED SH1106 : public SDD1306
{
public: //methods
  SH1106(const CcSize& oSize, IIo& oCommunication, ETransportType eType);
  virtual ~SH1106() = default;
};
