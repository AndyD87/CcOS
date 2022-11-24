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
 * @brief     Implemtation of class MCP23017
 */
#include "MCP23017.h"
#include "Devices/II2C.h"
#include "Devices/IGpioPin.h"
#include "CcConversionTables.h"

#define REGISTER_IODIRA     0x00
#define REGISTER_IODIRB     0x01
#define REGISTER_IPOLA      0x02
#define REGISTER_IPOLB      0x03
#define REGISTER_IOCON      0x05
#define REGISTER_GPIOA      0x08
#define REGISTER_GPIOB      0x09
#define REGISTER_OLATA      0x14
#define REGISTER_OLATB      0x15

#define REGISTER_IOCON_BANK 0x80

MCP23017::MCP23017(II2CClient* pII2CDevice) :
  m_pII2CDevice(pII2CDevice)
{

}

MCP23017::~MCP23017()
{
}

CcStatus MCP23017::onState(EState eState)
{
  CcStatus oStatus;
  switch(eState)
  {
    case EState::Start:
      if(m_pII2CDevice)
      {
        // Make sure IOCON.BANK is 0
        uint8 uiValue = 0x00;
        m_pII2CDevice->readRegister8(REGISTER_IOCON, &uiValue, 1);
        if(IS_FLAG_SET(uiValue, REGISTER_IOCON_BANK))
        {
          uiValue &= ~REGISTER_IOCON_BANK;
          m_pII2CDevice->writeRegister8(REGISTER_IOCON, &uiValue, 1);
        }
      }
      break;
    default:
      // Unhandled/Not required
      break;
  }
  return oStatus;
}

bool MCP23017::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t)
{
  bool bSuccess = true;
  switch(eDirection)
  {
    case IGpioPin::EDirection::Output:
      if(m_pII2CDevice)
      {
        uint8 uiValue = 0x00;
        if(1 == m_pII2CDevice->readRegister8(REGISTER_IODIRA, &uiValue, 1))
        {
          uiValue &= ~static_cast<uint8>(uiPinMask);
          m_pII2CDevice->writeRegister8(REGISTER_IODIRA, &uiValue, 1);
        }
        if(1 == m_pII2CDevice->readRegister8(REGISTER_IODIRB, &uiValue, 1))
        {
          uiValue &= ~static_cast<uint8>(uiPinMask >> 8);
          m_pII2CDevice->writeRegister8(REGISTER_IODIRB, &uiValue, 1);
        }
      }
      else
      {
        bSuccess = false;
      }
      break;
    case IGpioPin::EDirection::Input:
      if(m_pII2CDevice)
      {
        uint8 uiValue = 0x00;
        if(1 == m_pII2CDevice->readRegister8(REGISTER_IODIRA, &uiValue, 1))
        {
          uiValue |= static_cast<uint8>(uiPinMask);
          m_pII2CDevice->writeRegister8(REGISTER_IODIRA, &uiValue, 1);
        }
        if(1 == m_pII2CDevice->readRegister8(REGISTER_IODIRB, &uiValue, 1))
        {
          uiValue |= static_cast<uint8>(uiPinMask >> 8);
          m_pII2CDevice->writeRegister8(REGISTER_IODIRB, &uiValue, 1);
        }
      }
      else
      {
        bSuccess = false;
      }
      break;
    case IGpioPin::EDirection::Unknown:
    case IGpioPin::EDirection::Analog:
    case IGpioPin::EDirection::Alternate:
    default:
      bSuccess = false;
  }

  return bSuccess;
}

IGpioPin::EDirection MCP23017::getDirection(size_t uiPin)
{
  uint8 uiValue = 0x00;
  if (m_pII2CDevice)
  {
    if (uiPin < 8 && 1 == m_pII2CDevice->readRegister8(REGISTER_IODIRA, &uiValue, 1))
    {
      return (IS_FLAG_SET(1 << uiPin, uiValue)) ? IGpioPin::EDirection::Input : IGpioPin::EDirection::Output;
    }
    else if (uiPin < 16 && 1 == m_pII2CDevice->readRegister8(REGISTER_IODIRB, &uiValue, 1))
    {
      return (IS_FLAG_SET(1 << (uiPin - 8), uiValue)) ? IGpioPin::EDirection::Input : IGpioPin::EDirection::Output;
    }
  }
  return IGpioPin::EDirection::Unknown;
}

bool MCP23017::setPinValue(size_t uiPin, bool bValue)
{
  if(m_pII2CDevice)
  {
    // Make sure IOCON.BANK is 0
    uint8 uiValue = 0x00;
    if(uiPin < 8 && 1 == m_pII2CDevice->readRegister8(REGISTER_OLATA, &uiValue, 1))
    {
      if(bValue)
        uiValue |= static_cast<uint8>(1 << uiPin);
      else
        uiValue &= ~static_cast<uint8>(1 << uiPin);
      m_pII2CDevice->writeRegister8(REGISTER_IODIRA, &uiValue, 1);
    }
    else if(uiPin < 16 && 1 == m_pII2CDevice->readRegister8(REGISTER_OLATB, &uiValue, 1))
    {
      if(bValue)
        uiValue |= static_cast<uint8>(1 << (uiPin-8));
      else
        uiValue &= ~static_cast<uint8>(1 << (uiPin-8));
      m_pII2CDevice->writeRegister8(REGISTER_OLATB, &uiValue, 1);
    }
  }
  return bValue;
}

bool MCP23017::getPinValue(size_t uiPin)
{
  bool bValue = false;
  if(m_pII2CDevice)
  {
    // Make sure IOCON.BANK is 0
    uint8 uiValue = 0x00;
    if(uiPin < 8 && 1 == m_pII2CDevice->readRegister8(REGISTER_OLATA, &uiValue, 1))
    {
      bValue = IS_FLAG_SET(uiValue, (1 << uiPin));
    }
    else if(uiPin < 16 && 1 == m_pII2CDevice->readRegister8(REGISTER_OLATB, &uiValue, 1))
    {
      bValue = IS_FLAG_SET(uiValue, (1 << (uiPin - 8)));
    }
  }
  return bValue;
}

bool MCP23017::setValue(size_t uiValue)
{
  bool bSuccess = true;
  if (m_pII2CDevice)
  {
    // Make sure IOCON.BANK is 0
    uint8 uiNewValue = static_cast<uint8>(uiValue);
    if (1 != m_pII2CDevice->writeRegister8(REGISTER_OLATA, &uiNewValue, sizeof(uiNewValue)))
    {
      bSuccess = false;
    }
    uiNewValue = static_cast<uint8>(uiValue >> 8);
    if (1 != m_pII2CDevice->writeRegister8(REGISTER_OLATB, &uiNewValue, sizeof(uiNewValue)))
    {
      bSuccess = false;
    }
  }
  else
  {
    bSuccess = false;
  }
  return bSuccess;
}
