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
 * @brief     Implementation of Class OrangePiZeroBoardSupport
 */

#include "OrangePiZeroBoardSupport.h"
#include "CcKernel.h"
#include "CcDevice.h"
#include "CcStatic.h"
#include "OrangePiZeroGpioPort.h"

#define NP UINT16_MAX

OrangePiZeroBoardSupport::OrangePiZeroBoardSupport()  :
  // @todo remove copy from simulation
  IBoardSupport(
    {
      CHwDevice(EDeviceType::I2C     , 0, {CHwDevice::CPort(0,{2,4})}, {}),
      CHwDevice(EDeviceType::I2C     , 1, {CHwDevice::CPort(0,{15,17})}, {}),
      CHwDevice(EDeviceType::Spi     , 0, {CHwDevice::CPort(0,{18,20,22,23})}, {}),
      CHwDevice(EDeviceType::Uart    , 0, {CHwDevice::CPort(1,{0,1})}, {}),
      CHwDevice(EDeviceType::Uart    , 1, {CHwDevice::CPort(0,{7,9})}, {}),
      CHwDevice(EDeviceType::Uart    , 2, {CHwDevice::CPort(0,{10,12,14,21})}, {}),
      CHwDevice(EDeviceType::GpioPort, 0, {CHwDevice::CPort(0,{2,4,6,8,10,12,14,18,20,22,7,9,11,15,17,21,23,25})}, {})
    },
    {
      CHwPort(0, {CHwPin(NP, NP, {},         "3V3"),
                  CHwPin(NP, NP, {},         "5V"),
                  CHwPin( 0,  8, {},         "PS12/I2C0_SDA"),
                  CHwPin(NP, NP, {},         "5V"),
                  CHwPin( 0,  9, {},         "PA11/I2C0_SDC"),
                  CHwPin(NP, NP, {},         "GND"),
                  CHwPin( 0,  7, {},         "PA06/PWM1"),
                  CHwPin( 0, 15, {},         "PG06/UART1_TX"),
                  CHwPin(NP, NP, {},         "GND"),
                  CHwPin( 0, 16, {},         "PG07/UART1_RX"),
                  CHwPin( 0,  0, {},         "PA01/UART2_RX"),
                  CHwPin( 0,  1, {},         "PA07/PAEINT7/SIM_CLK"),
                  CHwPin( 0,  2, {},         "PA00/UART2_TX"),
                  CHwPin(NP, NP, {},         "GND"),
                  CHwPin( 0,  3, {},         "PA00/UART2_CTS"),
                  CHwPin( 0,  4, {},         "PA19/I2C1_SDA"),
                  CHwPin(NP, NP, {},         "3V3"),
                  CHwPin( 0,  5, {},         "PA18/I2C1_SDC"),
                  CHwPin( 0, 12, {},         "PA15/SPI_MOSI"),
                  CHwPin(NP, NP, {},         "GND"),
                  CHwPin( 0, 13, {},         "PA16/SPI_MISO"),
                  CHwPin( 0,  6, {},         "PA02/UART2_RTS"),
                  CHwPin( 0, 14, {},         "PA14/SPI_CLK"),
                  CHwPin( 0, 10, {},         "PA13/SPI_CS"),
                  CHwPin(NP, NP, {},         "GND"),
                  CHwPin( 0, 11, {},         "PA10/PA_NINT10/SIM_DET")}),

      CHwPort(1, {CHwPin(NP, NP, {},         "TX"),
                  CHwPin(NP, NP, {},         "RX"),
                  CHwPin(NP, NP, {},         "GND")}),

      CHwPort(2, {CHwPin(NP, NP, {},         "IR-RX"),
                  CHwPin(NP, NP, {},         "MIC1N"),
                  CHwPin(NP, NP, {},         "MIC1P"),
                  CHwPin(NP, NP, {},         "MIC-BIAS"),
                  CHwPin(NP, NP, {},         "TV-OUT"),
                  CHwPin(NP, NP, {},         "LINEOUTL"),
                  CHwPin(NP, NP, {},         "LINEOUTR"),
                  CHwPin(NP, NP, {},         "USB-DP3"),
                  CHwPin(NP, NP, {},         "USB-DM3"),
                  CHwPin(NP, NP, {},         "USB-DP2"),
                  CHwPin(NP, NP, {},         "USB-DM2"),
                  CHwPin(NP, NP, {},         "GND"),
                  CHwPin(NP, NP, {},         "5V")})
    }
  )
{
}

OrangePiZeroBoardSupport::~OrangePiZeroBoardSupport()
{
}

CcStatus OrangePiZeroBoardSupport::onState(EState eState)
{
  CCUNUSED(eState);
  return CcStatus();
}

CcDevice OrangePiZeroBoardSupport::createDevice(EDeviceType eDeviceType, uint32 uiDeviceNumber)
{
  CcDevice oDevice;
  uint32 uiId = UINT32_MAX;
  CHwDevice& rDevice = getHwDevice(eDeviceType, uiDeviceNumber, uiId);
  if(&rDevice != &InvalidDevice &&
     verifyFreePort(rDevice))
  {
    switch (eDeviceType)
    {
      case EDeviceType::GpioPort:
      {
        break;
      }
      case EDeviceType::Spi:
      {
        if(rDevice.pDevice == nullptr)
        {
          //CcSpiSimulation* pSpi = new CcSpiSimulation();
          //rDevice.pDevice = pSpi;
          //oDevice = pSpi;
          //setHwDeviceUsage(rDevice, uiId);
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
          //CcI2CSimulation* pSpi = new CcI2CSimulation();
          //rDevice.pDevice = pSpi;
          //oDevice = pSpi;
          //setHwDeviceUsage(rDevice, uiId);
        }
        else
        {
          oDevice = rDevice.pDevice;
        }
        break;
      }
      default:
      {
        CCDEBUG("Requested devie not found");
      }
    }
  }
  return oDevice;
}
