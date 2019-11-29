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
 * @brief     Implemtation of class TLC594xDotCorrection
 */
#include "TLC594xDotCorrection.h"
#include "Devices/ISpi.h"
#include "Devices/IGpioPin.h"

#define TLC594xDotCorrection_CHANNELS  16
#define TLC594xDotCorrection_DOT_WIDTH  6
#define TLC594xDotCorrection_BYTE_COUNT 8
#define TLC594xDotCorrection_DOT_SIZE_PER_CHIP ((TLC594xDotCorrection_CHANNELS*TLC594xDotCorrection_DOT_WIDTH)/TLC594xDotCorrection_BYTE_COUNT)

void TLC594xDotCorrection::setLedBrightness(size_t uiLedNr, uint16 uiBrightness)
{
  size_t uiChipNr     = uiLedNr % TLC594xDotCorrection_CHANNELS;
  size_t uiChipLedNr  = uiLedNr / TLC594xDotCorrection_CHANNELS;
  if (uiChipNr < m_oData.size())
  {
    uint8* pLedNr = reinterpret_cast<uint8*>(m_oData.getArray());
    pLedNr += uiChipLedNr/4;
    switch (uiChipLedNr % 4)
    {
      case 0:
      {
        pLedNr[0] &= 0x3f;
        pLedNr[0] |= static_cast<uint8>(uiBrightness & 0x3f);
        break;
      }
      case 1:
      {
        pLedNr[0] &= 0xc0;
        pLedNr[0] |= static_cast<uint8>((uiBrightness & 0x03) << 6);

        pLedNr[1] &= 0x0f;
        pLedNr[1] |= static_cast<uint8>((uiBrightness & 0x03c) >> 2);
        break;
      }
      case 2:
      {
        pLedNr[1] &= 0xf0;
        pLedNr[1] |= static_cast<uint8>((uiBrightness & 0x0f) << 4);

        pLedNr[2] &= 0x03;
        pLedNr[2] |= static_cast<uint8>((uiBrightness & 0x30) >> 4);
        break;
      }
      case 3:
      {
        pLedNr[2] &= 0xfc;
        pLedNr[2] |= static_cast<uint8>((uiBrightness & 0x3f) << 2);
        break;
        break;
      }
    }
  }
}

void TLC594xDotCorrection::setChipCount(size_t uiNumberOfChips)
{
  m_uiChipCount = uiNumberOfChips;
  m_oData.resize(uiNumberOfChips * TLC594xDotCorrection_DOT_SIZE_PER_CHIP, 0);
}
