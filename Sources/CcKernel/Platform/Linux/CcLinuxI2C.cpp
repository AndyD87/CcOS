/*
 * This m_iDevice is part of CcOS.
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
 * @m_iDevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxI2C
 */

#include "CcLinuxI2C.h"
#include "CcStatic.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "CcFile.h"

class CcLinuxI2C::CSlave : public II2CSlave
{
public:
  CSlave(const CcString& pDevice, uint16 uiAddress):
    II2CSlave(uiAddress)
  {
    // open a channel to the I2C device
    m_iDevice = open(pDevice.getCharString(), O_RDWR);

    if (m_iDevice < 0)
    {
      CCERROR(CcString("Could not open i2c device ") << uiAddress << " " << CcString::fromNumber(errno));
    }
    else
    {
      if (ioctl(m_iDevice, I2C_SLAVE, uiAddress) < 0)
      {
        CCERROR(CcString("Could not open i2c slave device ") << uiAddress << " " << CcString::fromNumber(errno));
      }
    }
  }

  int32 i2c_smbus_access(char read_write, uint8 command, int size, union i2c_smbus_data *data)
  {
    struct i2c_smbus_ioctl_data args;
    int32 err;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;

    err = ioctl(m_iDevice, I2C_SMBUS, &args);
    if (err == -1)
      err = -errno;
    return err;
  }

  size_t readByte(uint8& uiByte)
  {
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      union i2c_smbus_data data;
      int iError = i2c_smbus_access(I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data);
      if(iError != 0)
      {
        CCDEBUG(CcString("Error on I2C read byte: ") << CcString::fromNumber(errno));
      }
      else
      {
        uiByte = data.byte;
        uiReturn = 1;
      }
    }
    return uiReturn;
  }

  size_t writeByte(uint8 uiByte)
  {
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      int iError = i2c_smbus_access(I2C_SMBUS_WRITE, uiByte, I2C_SMBUS_BYTE, nullptr);
      if(iError != 0)
      {
        CCDEBUG(CcString("Error on I2C write byte: ") << CcString::fromNumber(errno));
      }
      else
      {
        uiReturn = 1;
      }
    }
    return uiReturn;
  }

  virtual size_t read(void* pBuffer, size_t uiSize) override
  {
    uchar* puBuffer = static_cast<uchar*>(pBuffer);
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      uiReturn = 0;
      while(uiSize > 0)
      {
        uint8 uiByte;
        if( 1 == readByte(uiByte))
        {
          puBuffer[uiReturn] = uiByte;
          uiSize--;
          uiReturn++;
        }
        else
        {
          uiReturn = SIZE_MAX;
          break;
        }
      }
    }
    return uiReturn;
  }

  virtual size_t write(const void* pBuffer, size_t uiSize) override
  {
    const uchar* puBuffer = static_cast<const uchar*>(pBuffer);
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      uiReturn = 0;
      uiReturn = 0;
      while(uiSize > 0)
      {
        uint8 uiByte = puBuffer[uiReturn];
        if( 1 == writeByte(uiByte))
        {
          uiSize--;
          uiReturn++;
        }
        else
        {
          uiReturn = SIZE_MAX;
          break;
        }
      }
    }
    return uiReturn;
  }

  virtual size_t readRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize) override
  {
    uchar* puBuffer = static_cast<uchar*>(pBuffer);
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      union i2c_smbus_data data;
      uiReturn = 0;
      while(uiSize > 0)
      {
        uint8 uiTransferSize = 0;
        if(uiSize > I2C_SMBUS_BLOCK_MAX)
        {
          uiTransferSize = I2C_SMBUS_BLOCK_MAX;
        }
        else
        {
          uiTransferSize = static_cast<uint8>(uiSize);
        }
        data.block[0] = uiTransferSize;

        int iError = i2c_smbus_access(I2C_SMBUS_READ, uiRegister, I2C_SMBUS_I2C_BLOCK_BROKEN , &data);
        if(iError != 0)
        {
          CCDEBUG(CcString("Error on I2C read block: " + CcString::fromNumber(uiRegister, 16)) << CcString::fromNumber(errno));
          uiReturn = SIZE_MAX;
          break;
        }
        else
        {
          uiTransferSize = CCMIN(uiTransferSize, data.block[0]);
          CcStatic::memcpy(puBuffer, data.block + 1, uiTransferSize);
          puBuffer    += uiTransferSize;
          uiReturn    += uiTransferSize;
          uiSize      -= uiTransferSize;
          uiRegister  += uiTransferSize;
        }
      }
    }
    return uiReturn;
  }

  virtual size_t writeRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize) override
  {
    uchar* puBuffer = static_cast<uchar*>(pBuffer);
    size_t uiReturn = SIZE_MAX;
    if(m_iDevice >= 0)
    {
      union i2c_smbus_data data;
      while(uiSize > 0)
      {
        uint8 uiTransferSize = 0;
        if(uiSize > I2C_SMBUS_BLOCK_MAX)
        {
          uiTransferSize = I2C_SMBUS_BLOCK_MAX;
        }
        else
        {
          uiTransferSize = static_cast<uint8>(uiSize);
        }
        data.block[0] = uiTransferSize;
        CcStatic::memcpy(data.block + 1, puBuffer, uiTransferSize);

        int iError = i2c_smbus_access(I2C_SMBUS_WRITE, uiRegister, I2C_SMBUS_I2C_BLOCK_BROKEN , &data);
        if(iError != 0)
        {
          CCDEBUG(CcString("Error on I2C write block: " + CcString::fromNumber(uiRegister, 16)) << CcString::fromNumber(errno));
          uiReturn = SIZE_MAX;
          break;
        }
        else
        {
          puBuffer    += data.block[0];
          uiReturn    += data.block[0];
          uiSize      -= data.block[0];
          uiRegister  += data.block[0];
        }
      }
    }
    return uiReturn;
  }

private:
  int m_iDevice = -1;
};

CcStatus CcLinuxI2C::onState(EState eState)
{
  CcStatus oReturn;
  switch(eState)
  {
    case EState::Start:
    {
      CcString sPath("/dev/i2c-" + CcString::fromNumber(m_uiNr));
      if(!CcFile::exists(sPath))
      {
        oReturn = EStatus::FSFileNotFound;
      }
      break;
    }
    default:
      break;
  }
  return oReturn;
}

II2CSlave* CcLinuxI2C::createInterface(uint16 uiAddress)
{
  II2CSlave* pSlave = nullptr;
  if(getState() == EState::Running)
  {
    CCNEWTYPE(pOpiSlave, CSlave, "/dev/i2c-" + CcString::fromNumber(m_uiNr), uiAddress);
    pSlave = pOpiSlave;
  }
  return pSlave;
}
