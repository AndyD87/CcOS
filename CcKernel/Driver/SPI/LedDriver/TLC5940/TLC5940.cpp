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
 * @brief     Implemtation of class TLC5940
 */
#include "TLC5940.h"
#include "Devices/ISpi.h"
#include "Devices/IGpioPin.h"

#define TLC5940_CHANNELS  16
#define TLC5940_PWM_WIDTH 12
#define TLC5940_DOT_WIDTH  6
#define TLC5940_BYTE_COUNT 8
#define TLC5940_SIZE_PER_CHIP ((TLC5940_CHANNELS*TLC5940_PWM_WIDTH)/TLC5940_BYTE_COUNT)
#define TLC5940_DOT_SIZE_PER_CHIP ((TLC5940_CHANNELS*TLC5940_DOT_WIDTH)/TLC5940_BYTE_COUNT)

TLC5940::TLC5940(ISpi* pSpiDevice) :
  m_pSpiDevice(pSpiDevice)
{
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
  size_t uiChipNr     = uiLedNr % TLC5940_CHANNELS;
  size_t uiChipLedNr  = uiLedNr / TLC5940_CHANNELS;
  if (uiChipNr < m_oData.size())
  {
    uint8* pLedNr = reinterpret_cast<uint8*>(m_oData.getArray());
    pLedNr += (uiChipLedNr >> 1) * 3;
    if (uiChipLedNr & 1)
    {
      pLedNr[1] &= 0x0f;
      pLedNr[1] |= static_cast<uint8>((uiBrightness & 0x00f) << 4);
      pLedNr[2] = static_cast<uint8>((uiBrightness & 0xff0) >> 4);
    }
    else
    {
      pLedNr[0] = static_cast<uint8>(uiBrightness & 0xff);
      pLedNr[1] &= 0xf0;
      pLedNr[1] |= static_cast<uint8>((uiBrightness & 0xf00) >> 8);
    }
  }
}

void TLC5940::setChipCount(size_t uiNumberOfChips)
{
  m_uiChipCount = uiNumberOfChips;
  m_oData.resize(uiNumberOfChips * TLC5940_SIZE_PER_CHIP, 0);
}
