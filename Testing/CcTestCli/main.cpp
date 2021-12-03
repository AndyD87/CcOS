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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "Devices/CcBoardSupport.h"
#include "Devices/CcDeviceI2C.h"
#include "Driver/I2C/PortExpander/MCP23017.h"

/**
 * @brief Default application entry point
 * @param iArgc:  Argument count in ppArgv
 * @param ppArgv: Passed arguments from callup
 * @return Exitcode, default 0 if no error occured
 */
int main(int iArgc, char** ppArgv)
{
  CCUNUSED(iArgc);
  CCUNUSED(ppArgv);
  int iRet = 0;
  CcDeviceI2C oI2CDevice        = CcKernel::getDevice(EDeviceType::I2C);
  if(!oI2CDevice.isValid())
  {
    CcBoardSupport oBoardSupport  = CcKernel::getDevice(EDeviceType::BoardSupport);
    if(oBoardSupport.isValid())
    {
      oBoardSupport.getDevice()->getDeviceSize(EDeviceType::I2C);
    }
  }
  if(oI2CDevice.isValid())
  {
    II2CSlave* pInterface = oI2CDevice.getDevice()->createInterface(0x20);
    if(pInterface)
    {
      CCDEBUG("Check interface register");
      char pChar[16];
      pInterface->readRegister8(0x00, pChar, 16);

      for(int i = 0; i < 16; i++)
      {
        CCDEBUG("  Register: " + CcString::fromNumber(i) + " " + CcString::fromNumber(pChar[i], 16).fillBeginUpToLength("0",2));
      }

      CCDEBUG("");
      CCDEBUG("Setup PortExpander");
      MCP23017 oPortexpander(pInterface);
      if(oPortexpander.start())
      {
        CCDEBUG("Setup 8 Pins output");
        if (oPortexpander.setPinsDirection(0xff, IGpioPin::EDirection::Output))
        {
          oPortexpander.setValue(0x5555, true);
          CCDEBUG("Read register again");
          pInterface->readRegister8(0x00, pChar, 16);
          for (int i = 0; i < 16; i++)
          {
            CCDEBUG("  Register: " + CcString::fromNumber(i) + " " + CcString::fromNumber(pChar[i], 16).fillBeginUpToLength("0", 2));
          }
        }

      }


    }
  }
  return iRet;
}
