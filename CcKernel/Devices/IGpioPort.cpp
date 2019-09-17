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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IGpioPort
 */

#include "Devices/IGpioPort.h"
#include "Devices/CcGpioPortPin.h"

IGpioPin* IGpioPort::getPin(uint8 uiNr)
{
  CCNEWTYPE(pPin, CcGpioPortPin, this, uiNr);
  return pPin;
}

bool IGpioPort::setAlternateValue(size_t uiPin, size_t uiValue)
{
  CCUNUSED(uiPin);
  CCUNUSED(uiValue);
  return false;
}

bool IGpioPort::setSpeedValue(size_t uiPin, size_t uiValue)
{
  CCUNUSED(uiPin);
  CCUNUSED(uiValue);
  return false;
}
