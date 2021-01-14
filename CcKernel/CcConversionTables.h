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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcConversionTables
 */
#ifndef H_CcConversionTables_H_
#define H_CcConversionTables_H_

#include "CcBase.h"
#include "CcBase.h"

/**
 * @brief Static class for Console in- and output.
 *        The default target for this class is stdin and stdout, but can be changed during runtime.
 */
class CcKernelSHARED CcConversionTables
{
public:
  class CPrivate;

  static uint8  convertU8ToSineWaveU8(uint8 uiNumber);
  static uint16 convertU8LogToU12(uint8 uiNumber);
};

#endif // H_CcConversionTables_H_
