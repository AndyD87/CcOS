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
 * @brief     Class CcTouchTarget
 */
#pragma once

#include "CcBase.h"
#include "Devices/ITouch.h"
#include "Devices/ITimer.h"

/**
 * @brief Unimplemented clas just prepared to create touch device for
 *        STM32F4xx devices
 */
class CcTouchTarget : public ITouch
{
public:
  CcTouchTarget();
  virtual ~CcTouchTarget();

  void getTouchState(uint16& uiX, uint16& uiY);
  bool getPressState();

private:
  SPI_HandleTypeDef m_SpiHandle;
};
