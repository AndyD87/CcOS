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
 * @brief     Implemtation of class TLC5947
 */
#include "TLC5947.h"
#include "Devices/ISpi.h"
#include "Devices/IGpioPin.h"
#include "CcConversionTables.h"

//! Number of channels available on TLC5947
#define TLC5947_CHANNELS            24
//! Pwm width in bits on TLC5947
#define TLC5947_PWM_WIDTH           12
//! Number of Bits per byte
#define TLC5947_BYTE_COUNT           8
//! Number of Bytes to save per chip
#define TLC5947_BYTES_PER_CHIP      (TLC5947_CHANNELS/2)*3
//! Number of Bytes per chip required to send all channels
#define TLC5947_SIZE_PER_CHIP       ((TLC5947_CHANNELS*TLC5947_PWM_WIDTH)/TLC5947_BYTE_COUNT)

size_t TLC5947::s_uiMinSize = 0;

TLC5947::TLC5947(ISpi* pSpiDevice) :
  m_pSpiDevice(pSpiDevice)
{
  m_pSpiDevice->registerOnTransferComplete(NewCcEventType(TLC5947,void,this,TLC5947::onTransferComplete));
}

TLC5947::~TLC5947()
{
}

void TLC5947::write()
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

void TLC5947::flush()
{
  if (m_pChipSelect)
  {
    m_pChipSelect->setValue(1);
    m_pChipSelect->setValue(0);
  }
}

void TLC5947::blank(bool bOnOff)
{
  if (m_pBlank)
  {
    m_pBlank->setValue(bOnOff);
  }
}

void TLC5947::setLedBrightness(size_t uiLedNr, uint16 uiBrightness)
{
  size_t uiChipNr     = uiLedNr / TLC5947_CHANNELS;
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

void TLC5947::setLedColorValue(size_t uiLedNr, uint8 uiColor)
{
  setLedBrightness(uiLedNr, CcConversionTables::convertU8LogToU12(uiColor));
}

void TLC5947::onTransferComplete(void* pData)
{
  CCUNUSED(pData);
  flush();
}

void TLC5947::setMinSize(size_t uiMinSize)
{
  s_uiMinSize = uiMinSize;
}

size_t TLC5947::getLedCount()
{
  return m_uiChipCount * TLC5947_CHANNELS;
}

void TLC5947::setChipCount(size_t uiNumberOfChips)
{
  m_uiChipCount = uiNumberOfChips;
  size_t uiNewSize = uiNumberOfChips * TLC5947_BYTES_PER_CHIP;
  if(s_uiMinSize < uiNewSize)
    m_oData.resize(uiNewSize, 0);
}
