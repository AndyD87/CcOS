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
 * @brief     Implemtation of class TLC5940
 */
#include "TLC5940.h"
#include "Devices/ISpi.h"
#include "Devices/IGpioPin.h"
#include "CcConversionTables.h"

//! TLC5940 has 16 Outputs
#define TLC5940_CHANNELS            16
//! TLC5940 pwm width is 12 bit
#define TLC5940_PWM_WIDTH           12
//! TLC5940 dot correction width is 6 bit
#define TLC5940_DOT_WIDTH           6
//! Number of bits a byte have
#define TLC5940_BYTE_COUNT          8
//! Number of bytes for all channels
#define TLC5940_BYTES_PER_CHIP      (TLC5940_CHANNELS/2)*3
//! Number of Bytes for pwm settings
#define TLC5940_SIZE_PER_CHIP       ((TLC5940_CHANNELS*TLC5940_PWM_WIDTH)/TLC5940_BYTE_COUNT)
//! Number of Bytes for dot settings
#define TLC5940_DOT_SIZE_PER_CHIP   ((TLC5940_CHANNELS*TLC5940_DOT_WIDTH)/TLC5940_BYTE_COUNT)

size_t TLC5940::s_uiMinSize = 0;

TLC5940::TLC5940(ISpi* pSpiDevice) :
  m_pSpiDevice(pSpiDevice)
{
  m_pSpiDevice->registerOnTransferComplete(NewCcEventType(TLC5940,void,this,TLC5940::onTransferComplete));
}

TLC5940::~TLC5940()
{
}

void TLC5940::writeDotCorrection(bool bEeprom, const void* pData, size_t uiSize)
{
  if (uiSize >= m_uiChipCount * TLC5940_DOT_SIZE_PER_CHIP)
  {
    if (bEeprom)
    {
      m_pDcprg->setValue(false);
    }
    else
    {
      m_pDcprg->setValue(true);
    }
    // Set high
    m_pVprg->setValue(true);
    m_pSpiDevice->write(pData, uiSize);
  }
}

void TLC5940::write()
{
  if (m_pSpiDevice)
  {
    if (m_pVprg)
    {
      // Set low
      m_pVprg->setValue(false);
    }
    m_pSpiDevice->writeArray(m_oData);
  }
}

void TLC5940::flush()
{
  if (m_pChipSelect)
  {
    m_pChipSelect->setValue(1);
    m_pChipSelect->setValue(0);
  }
}

void TLC5940::blank(bool bOnOff)
{
  if (m_pBlank)
  {
    m_pBlank->setValue(bOnOff);
  }
}

void TLC5940::setLedBrightness(size_t uiLedNr, uint16 uiBrightness)
{
  size_t uiChipNr     = uiLedNr / TLC5940_CHANNELS;
  size_t uiLast       = m_oData.size()-1;
  if (uiChipNr < m_uiChipCount)
  {
    uiLast -= (uiLedNr >> 1) * 3;
    if (uiLedNr & 1)
    {
      m_oData[uiLast-2] = static_cast<uint8>((uiBrightness & 0xff0) >> 4);
      m_oData[uiLast-1] &= 0x0f;
      m_oData[uiLast-1] |= static_cast<uint8>((uiBrightness & 0xf) << 4);
    }
    else
    {
      m_oData[uiLast-1] &= 0xf0;
      m_oData[uiLast-1] |= static_cast<uint8>((uiBrightness & 0xf00) >> 8);
      m_oData[uiLast-0] = static_cast<uint8>(uiBrightness & 0xff);
    }
  }
}

void TLC5940::setLedColorValue(size_t uiLedNr, uint8 uiColor)
{
  setLedBrightness(uiLedNr, CcConversionTables::convertU8LogToU12(uiColor));
}

void TLC5940::onTransferComplete(void* pData)
{
  CCUNUSED(pData);
  flush();
}

void TLC5940::setMinSize(size_t uiMinSize)
{
  s_uiMinSize = uiMinSize;
}

size_t TLC5940::getLedCount()
{
  return m_uiChipCount * TLC5940_CHANNELS;
}

void TLC5940::setChipCount(size_t uiNumberOfChips)
{
  m_uiChipCount = uiNumberOfChips;
  size_t uiNewSize = uiNumberOfChips * TLC5940_BYTES_PER_CHIP;
  if(s_uiMinSize < uiNewSize)
    m_oData.resize(uiNewSize, 0);
}
