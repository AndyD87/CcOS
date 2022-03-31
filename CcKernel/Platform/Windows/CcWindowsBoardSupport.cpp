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
 * @brief     Implementation of Class CcWindowsBoardSupport
 */

#include "CcWindowsBoardSupport.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "CcWindowsTimer.h"

CcWindowsBoardSupport::CcWindowsBoardSupport()  :
  // @todo remove copy from simulation
  IBoardSupport(
    {
    },
    {
    }
  )
{
}

CcWindowsBoardSupport::~CcWindowsBoardSupport()
{
}

CcDevice CcWindowsBoardSupport::createDevice(EDeviceType eDeviceType, size_t uiDeviceNumber)
{
  CcDevice oDevice;
  size_t uiFunctionNr = SIZE_MAX;
  CHwDevice& rHwDevice = getHwDevice(eDeviceType, uiDeviceNumber, uiFunctionNr);
  if (&rHwDevice == &InvalidDevice)
  {
    switch (eDeviceType)
    {
      case EDeviceType::GpioPort:
      {
        break;
      }
      case EDeviceType::Spi:
      {
        break;
      }
      case EDeviceType::I2C:
      {
        break;
      }
      case EDeviceType::Timer:
      {
        if (uiDeviceNumber == 0)
        {
          CCNEWTYPE(pTimer, CcWindowsTimer);
          m_oHwDevices.append(CHwDevice(eDeviceType, uiDeviceNumber, {}, {}, pTimer));
          oDevice.set(pTimer, eDeviceType);
          return CcKernel::addDevice(CcDevice(pTimer, eDeviceType));
        }
        break;
      }
    }
  }
  else
  {
    oDevice.set(rHwDevice.pDevice, rHwDevice.eDevice);
  }
  return oDevice;
}
