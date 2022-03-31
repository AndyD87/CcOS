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
 * @brief     Class CcSpiSimulation
 */
#include "Devices/Simulations/CcBoardSupportSimulation.h"
#include "Devices/Simulations/CcSpiSimulation.h"
#include "Devices/Simulations/CcI2CSimulation.h"

CcBoardSupportSimulation::CcBoardSupportSimulation() :
  IBoardSupport(
    {
      CHwDevice(EDeviceType::GpioPort, 0, {}, {CHwDevice::CPort(0,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15})}),
      CHwDevice(EDeviceType::GpioPort, 1, {}, {CHwDevice::CPort(1,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15})}),
      CHwDevice(EDeviceType::I2C     , 0, {CHwDevice::CPort(0,{0,1})}, {CHwDevice::CPort(1,{0})}),
      CHwDevice(EDeviceType::Spi     , 0, {CHwDevice::CPort(0,{0,1,2})}, {CHwDevice::CPort(1,{0})}),
      CHwDevice(EDeviceType::Uart    , 0, {CHwDevice::CPort(1,{0,1})}, {})
    },
    {
      CHwPort(0, {CHwPin( 0,  0, {0, 2, 3},  "GPIO_I2C_SPI"),
                  CHwPin( 0,  1, {0, 2, 3},  "GPIO_I2C_SPI"),
                  CHwPin( 0,  2, {0, 3},     "GPIO_SPI"),
                  CHwPin( 0,  3, {0},        "GPIO"),
                  CHwPin( 0,  4, {0},        "GPIO"),
                  CHwPin( 0,  5, {0},        "GPIO"),
                  CHwPin( 0,  6, {0},        "GPIO"),
                  CHwPin( 0,  7, {0},        "GPIO"),
                  CHwPin( 0,  8, {0},        "GPIO"),
                  CHwPin( 0,  9, {0},        "GPIO"),
                  CHwPin( 0, 10, {0},        "GPIO"),
                  CHwPin( 0, 11, {0},        "GPIO"),
                  CHwPin( 0, 12, {0},        "GPIO"),
                  CHwPin( 0, 13, {0},        "GPIO"),
                  CHwPin( 0, 14, {0},        "GPIO"),
                  CHwPin( 0, 15, {0},        "GPIO")}),
      CHwPort(1, {CHwPin( 1,  0, {1, 2, 3},  "GPIO_UART"),
                  CHwPin( 1,  1, {1, 2, 3},  "GPIO_UART"),
                  CHwPin( 1,  2, {1},        "GPIO"),
                  CHwPin( 1,  3, {1},        "GPIO"),
                  CHwPin( 1,  4, {1},        "GPIO"),
                  CHwPin( 1,  5, {1},        "GPIO"),
                  CHwPin( 1,  6, {1},        "GPIO"),
                  CHwPin( 1,  7, {1},        "GPIO"),
                  CHwPin( 1,  8, {1},        "GPIO"),
                  CHwPin( 1,  9, {1},        "GPIO"),
                  CHwPin( 1, 10, {1},        "GPIO"),
                  CHwPin( 1, 11, {1},        "GPIO"),
                  CHwPin( 1, 12, {1},        "GPIO"),
                  CHwPin( 1, 13, {1},        "GPIO"),
                  CHwPin( 1, 14, {1},        "GPIO"),
                  CHwPin( 1, 15, {1},        "GPIO")})
    }
    )
{
}

CcBoardSupportSimulation::~CcBoardSupportSimulation()
{
}

CcDevice CcBoardSupportSimulation::createDevice(EDeviceType eDeviceType, size_t uiDeviceNumber)
{
  CcDevice oDevice;
  size_t uiId = SIZE_MAX;
  CHwDevice& rDevice = getHwDevice(eDeviceType, uiDeviceNumber, uiId);
  if(&rDevice != &InvalidDevice &&
     verifyFreePort(rDevice))
  {
    switch (eDeviceType)
    {
      case EDeviceType::Spi:
      {
        if(rDevice.pDevice == nullptr)
        {
          CCNEWTYPE(pSpi, CcSpiSimulation);
          rDevice.pDevice = pSpi;
          oDevice = pSpi;
          setHwDeviceUsage(rDevice, uiId);
        }
        else
        {
          oDevice = rDevice.pDevice;
        }
        break;
      }
      case EDeviceType::I2C:
      {
        if(rDevice.pDevice == nullptr)
        {
          CCNEWTYPE(pSpi, CcI2CSimulation);
          rDevice.pDevice = pSpi;
          oDevice = pSpi;
          setHwDeviceUsage(rDevice, uiId);
        }
        else
        {
          oDevice = rDevice.pDevice;
        }
        break;
      }
    }
  }
  return oDevice;
}
