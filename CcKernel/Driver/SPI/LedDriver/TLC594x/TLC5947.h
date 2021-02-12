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

  /**
   * @brief Write current led state to Chip
   */
  void write();

  /**
   * @brief Flush data, do not cache
   */
  void flush();

  /**
   * @brief Turn off all LEDs or reset to set values
   * @param bOnOff: true for turn off all LEDs
   */
  void blank(bool bOnOff);

  //! @return Get number of LEDs the chip controls
  size_t getLedCount();
  //! @return Get number of chips in row
  void setChipCount(size_t uiNumberOfChips);

  //! @param pChipSelect: Set handle for ChipSelect pin
  void setCSPin(IGpioPin* pChipSelect)
  { m_pChipSelect = pChipSelect; }
  //! @param pXlat: Set handle for Xlat pin
  void setXlatPin(IGpioPin* pXlat)
  { m_pChipSelect = pXlat; }

  //! @param pBlank: Set handle for Blank pin
  void setBlankPin(IGpioPin* pBlank)
  { m_pBlank = pBlank; }

  /**
   * @brief Set brightnes of specific LED as PWM
   * @param uiLedNr:      Indes of led for brightness
   * @param uiBrightness: PWM value between 0x0 and 0xffff for brigthness
   */
  void setLedBrightness(size_t uiLedNr, uint16 uiBrightness);

  /**
   * @brief Set brightnes from linearic color value 0x0 to 0xff
   * @param uiLedNr: Index of LED to set
   * @param uiColor:  Color value to convert to brightness
   */
  void setLedColorValue(size_t uiLedNr, uint8 uiColor);

  /**
   * @brief Will be called on transfer complete
   * @param pData: Context for transfer complete
   */
  void onTransferComplete(void* pData);

  /**
   * @brief Set minimum size of LED's per chip
   * @param uiMinSize: Number of LEDs
   */
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
