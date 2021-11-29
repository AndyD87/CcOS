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
 * @brief     Class OrangePiZeroI2C
 */

#include "OrangePiZeroI2C.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include "CcFile.h"

class OrangePiZeroI2C::CSlave : II2CSlave
{
public:
  CSlave(const CcString& pDevice, uint16 uiAddress):
    II2CSlave(uiAddress)
  {
    // open a channel to the I2C device
    m_iDevice = open(pDevice.getCharString(), uiAddress);

    if (m_iDevice < 0)
    {
      CCERROR(CcString("Could not open i2c device ") << uiAddress << " " << CcString::fromNumber(errno));
    }
  }

  virtual size_t read(void* pBuffer, size_t uiSize) override
  {
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      uiReturn = ::read(m_iDevice, pBuffer, uiSize);
    }
    return uiReturn;
  }

  virtual size_t write(const void* pBuffer, size_t uiSize) override
  {
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      uiReturn = ::write(m_iDevice, pBuffer, uiSize);
    }
    return uiReturn;
  }

//  virtual size_t readRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize) override
//  {
//    size_t uiReturn = SIZE_MAX;
//    if(m_iDevice >= 0)
//    {

//    }
//    return uiReturn;
//  }

//  virtual size_t readRegister16(uint16 uiRegister, void* pBuffer, size_t uiSize) override
//  {
//    size_t uiReturn = SIZE_MAX;
//    if(m_iDevice >= 0)
//    {

//    }
//    return uiReturn;
//  }

//  virtual size_t writeRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize) override
//  {
//    size_t uiReturn = SIZE_MAX;
//    if(m_iDevice >= 0)
//    {

//    }
//    return uiReturn;
//  }

//  virtual size_t writeRegister16(uint16 uiRegister, void* pBuffer, size_t uiSize) override
//  {
//    size_t uiReturn = SIZE_MAX;
//    if(m_iDevice >= 0)
//    {

//    }
//    return uiReturn;
//  }

private:
  int m_iDevice = -1;
};

CcStatus OrangePiZeroI2C::setState(EState eState)
{
  switch(eState)
  {
    case EState::Start:
    {
      CcString sPath("/dev/i2c-" + CcString::fromNumber(m_uiNr));
      if(CcFile::exists(sPath))
      {
        IDevice::setState(eState);
      }
      else
        IDevice::setState(EState::Stopped);
      break;
    }
    default:
      IDevice::setState(eState);
  }
}

I2CSlave* OrangePiZeroI2C::createInterface(uint16 uiAddress)
{
  I2CSlave* pSlave = nullptr;
  if(getState() == EState::Running)
  {
    CCNEWTYPE(pOpiSlave, CSlave, "/dev/i2c-" + CcString::fromNumber(m_uiNr), uiAddress);
  }
  return pSlave;
}
