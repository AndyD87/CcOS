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
 * @brief     Class TLC5947
 **/
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "CcByteArray.h"

class ISpi;
class IGpioPin;

/**
 * @brief Class impelmentation
 */
class TLC5947 : public IDevice
{
public:
  /**
   * @brief Constructor
   */
  TLC5947(ISpi* pSpiDevice);

  /**
   * @brief Destructor
   */
  virtual ~TLC5947();

  void write();
  void flush();

  /**
   * @brief Turn off all LEDs or reset to set values
   * @param bOnOff: true for turn off all LEDs
   */
  void blank(bool bOnOff);

  size_t getLedCount();

  void setChipCount(size_t uiNumberOfChips);

  void setCSPin(IGpioPin* pChipSelect)
    { m_pChipSelect = pChipSelect; }
  void setXlatPin(IGpioPin* pXlat)
    { m_pChipSelect = pXlat; }

  void setBlankPin(IGpioPin* pBlank)
    { m_pBlank = pBlank; }
  void setLedBrightness(size_t uiLedNr, uint16 uiBrightness);
  void setLedColorValue(size_t uiLedNr, uint8 uiColor);

  void onTransferComplete(void* pData);
  static void setMinSize(size_t uiMinSize);

private:
  ISpi*       m_pSpiDevice;
  IGpioPin*   m_pChipSelect = nullptr;
  IGpioPin*   m_pBlank      = nullptr;
  IGpioPin*   m_pVprg       = nullptr;
  size_t      m_uiChipCount = 0;
  CcByteArray m_oData;
  static size_t s_uiMinSize;
};
