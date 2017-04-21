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
 * @page      Windows
 * @subpage   WindowsTouch
 *
 * @page      WindowsTouch
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsTouch
 **/
#ifndef WindowsTouch_H_
#define WindowsTouch_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include "Devices/CcTouch.h"

class WindowsTouch : public CcTouch
{
public:
  WindowsTouch();
  virtual ~WindowsTouch();

  void getTouchState(uint16 *x, uint16 *y) override;
  bool getPressState(void) override;
};

#endif /* WindowsTouch_H_ */
