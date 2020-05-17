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
 * @page      Linux
 * @subpage   CcLinuxDbus
 *
 * @page      CcLinuxDbus
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxDbus
 */

#ifndef H_CcLinuxDbus_H_
#define H_CcLinuxDbus_H_

#include "Devices/IGpioPin.h"
#include "CcString.h"
#include "CcFile.h"

class CcLinuxDbus
{
public:
  CcLinuxDbus();
  ~CcLinuxDbus();

  void init();
  void deinit();
private:
  class CPrivate;
  CPrivate* m_pPrivate;
};

#endif // H_CcLinuxDbus_H_
