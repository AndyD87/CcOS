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

void IGpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
{
  uint8 uiPinCnt;
  while(uiPinMask != 0)
  {
    if((uiPinMask & 1) == 1)
    {
      IGpioPin* pPin = getPin(uiPinCnt);
      if(pPin != nullptr)
      {
        pPin->setDirection(eDirection);
        switch(eDirection)
        {
          case IGpioPin::EDirection::Output:
            pPin->setValue(uiValue != 0);
            break;
          case IGpioPin::EDirection::Alternate:
            pPin->setAlternateValue(uiValue);
            break;
          default:
            break;
        }
      }
    }
    uiPinCnt++;
    uiPinMask >>= 1;
  }
}
