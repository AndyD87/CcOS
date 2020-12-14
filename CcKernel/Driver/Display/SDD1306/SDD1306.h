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
 * @page      Display_Driver
 * @subpage   SDD1306
 * 
 * @page      SDD1306
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class SDD1306
 */
#ifndef H_SDD1306_H_
#define H_SDD1306_H_

#include "CcBase.h"
#include "Devices/IDisplay.h"

class CcKernelSHARED SDD1306 : public IDisplay
{
public: //methods
  SDD1306();
  virtual ~SDD1306();
};

#endif // H_SDD1306_H_
