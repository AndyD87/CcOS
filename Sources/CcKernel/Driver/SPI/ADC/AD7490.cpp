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
 * @brief     Implementations for Class AD7490
 **/

#include "AD7490.h"
#include "CcStatic.h"

AD7490::AD7490(ISpi* m_pSpi) :
  m_pSpi(m_pSpi)
{
}

AD7490::~AD7490()
{
}

bool AD7490::Init()
{
  bool bSuccess = false;
  // Set Write
  m_uiTransferValue = 1<<11;
  // Set Powermode Normal
  m_uiTransferValue |= 3<<4;
  setRange(false);
  setWeakOutput(false);
  setCoding(false);
  setShadow(false);

  // Init adc
  if(m_pSpi)
  {
    uchar pucInit[2] = {0xff, 0xff};
    if(2 == m_pSpi->write(pucInit, sizeof(pucInit)))
    {
      //@todo unselect();
      //@todo select();
      if(2 == m_pSpi->write(pucInit, sizeof(pucInit)))
      {
        bSuccess = true;
      }
    }
    //@todo unselect();
  }
  return bSuccess;
}

uint16_t AD7490::readValue(uint8_t &uiAddress)
{
  //@todo select();
  if(m_pSpi)
  {
    uchar ubConvert[2] = {0,0};
    // Move the settings to the correct 12bit MSB first order
    ubConvert[0] = static_cast<uchar>(m_uiTransferValue >> 4);
    ubConvert[1] = static_cast<uchar>(m_uiTransferValue << 4);
    m_pSpi->writeRead(&m_uiResultValue, ubConvert, sizeof(ubConvert));
    //@todo unselect();
    //@todo select();
    // Transfer a dummy 16 bit value to get result value from
    ubConvert[0] = 0;
    ubConvert[1] = 0;
    m_pSpi->writeRead(&m_uiResultValue, ubConvert, sizeof(ubConvert));
    //@todo unselect();
    uint16_t uiSwapped = CcStatic::swapUint16(m_uiResultValue);
    // Top 4 bits is the address value of target input channel
    // The 4th bit of the address is multiplexed with the tristate value depending on settings.
    // No we filter for the lower 3 address bits and put it into output for verifiying
    uiAddress = (uiSwapped & 0x7000) >> 12;
    // Filter the address bits to get the raw value
    m_uiResultValue = 0x0fff & uiSwapped;
  }
  return m_uiResultValue;
}

void AD7490::setAddress(uint16_t uiAddress)
{
  m_uiTransferValue &= 0xfc3f;
  m_uiTransferValue |= uiAddress << 6;
}

void AD7490::setRange(bool bDouble)
{
  if(bDouble)
    m_uiTransferValue &= ~0x2;
  else
    m_uiTransferValue |= 0x2;
}

void AD7490::setCoding(bool bComplement)
{
  if(bComplement)
    m_uiTransferValue &= ~0x1;
  else
    m_uiTransferValue |= 0x1;
}

void AD7490::setWeakOutput(bool bWeak)
{
  if(bWeak)
    m_uiTransferValue |= 0x4;
  else
    m_uiTransferValue &= ~0x4;
}

void AD7490::setShadow(bool bShadow)
{
  if(bShadow)
    m_uiTransferValue |= 0x8;
  else
    m_uiTransferValue &= ~0x8;
}
