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
 * @brief     Class IGpioPort
 */

#include "Devices/IGpioPort.h"
#include "Devices/CcGpioPortPin.h"

const size_t IGpioPort::Pin0  = 0x00000001;
const size_t IGpioPort::Pin1  = 0x00000002;
const size_t IGpioPort::Pin2  = 0x00000004;
const size_t IGpioPort::Pin3  = 0x00000008;
const size_t IGpioPort::Pin4  = 0x00000010;
const size_t IGpioPort::Pin5  = 0x00000020;
const size_t IGpioPort::Pin6  = 0x00000040;
const size_t IGpioPort::Pin7  = 0x00000080;
const size_t IGpioPort::Pin8  = 0x00000100;
const size_t IGpioPort::Pin9  = 0x00000200;
const size_t IGpioPort::Pin10 = 0x00000400;
const size_t IGpioPort::Pin11 = 0x00000800;
const size_t IGpioPort::Pin12 = 0x00001000;
const size_t IGpioPort::Pin13 = 0x00002000;
const size_t IGpioPort::Pin14 = 0x00004000;
const size_t IGpioPort::Pin15 = 0x00008000;
const size_t IGpioPort::Pin16 = 0x00010000;
const size_t IGpioPort::Pin17 = 0x00020000;
const size_t IGpioPort::Pin18 = 0x00040000;
const size_t IGpioPort::Pin19 = 0x00080000;
const size_t IGpioPort::Pin20 = 0x00100000;
const size_t IGpioPort::Pin21 = 0x00200000;
const size_t IGpioPort::Pin22 = 0x00400000;
const size_t IGpioPort::Pin23 = 0x00800000;
const size_t IGpioPort::Pin24 = 0x01000000;
const size_t IGpioPort::Pin25 = 0x02000000;
const size_t IGpioPort::Pin26 = 0x04000000;
const size_t IGpioPort::Pin27 = 0x08000000;
const size_t IGpioPort::Pin28 = 0x10000000;
const size_t IGpioPort::Pin29 = 0x20000000;
const size_t IGpioPort::Pin30 = 0x40000000;
const size_t IGpioPort::Pin31 = 0x80000000;


bool IGpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
{
  return setPinsDirection(static_cast<size_t>(1) << uiPin, eDirection, uiValue);
}

IGpioPin* IGpioPort::getPin(uint8 uiNr)
{
  CCNEWTYPE(pPin, CcGpioPortPin, this, uiNr);
  return pPin;
}

bool IGpioPort::setSpeedValue(size_t uiPin, size_t uiValue)
{
  CCUNUSED(uiPin);
  CCUNUSED(uiValue);
  return false;
}
