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
 *
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcDisplayTarget
 */
#ifndef H_CCDISPLAYTARGET_H_
#define H_CCDISPLAYTARGET_H_

#include "CcBase.h"
#include "STM32.h"
#include "dev/IDisplay.h"
#include "dev/ITimer.h"

/**
 * @brief Should become a hardware Display. But it is not working at the moment
 */
class CcDisplayTarget : public IDisplay
{
public:
  CcDisplayTarget();
  virtual ~CcDisplayTarget();

  /**
   * @brief Open the Device, in this display nothing is to do
   * @param flags: not used
   * @return true if successfully opened.
   */
  bool open(OpenFlags::eOpenFlags flags);

  /**
   * @brief Close the Device
   * @return true if successfully done
   * @todo needs to be implemented
   */
  bool close(){ return true;}

  /**
   * @brief Set cursor position for painting
   * @param x:  New x value
   * @param y:  New y value
   */
  void setCursor( uint16 x, uint16 y );

  /**
   * @brief Set next drawing window
   * @param x:      X value of Start point
   * @param y:      Y value of Start point
   * @param xSize:  Width of window
   * @param ySize:  Height of window
   */
  void setAddress( uint16 x, uint16 y, uint16 xSize, uint16 ySize);

  /**
   * @brief Set brightness of background if possible.
   *        If display has no linear control of brightness, than 0 = off and all other values are on
   *
   * @param uiBrightness: 0 for off and 255 for maximum brightnes
   */
  void setBacklight( uint8 uiBrightness );

  /**
   * @brief Init io pins if required to enable outputs
   */
  void initIO();

  /**
   * @brief Init dma transfer and bus if required for operation.
   */
  void initBus();

  /**
   * @brief Draw pixel at current cursor and move cursor to next pixel
   * @param R:  Red value
   * @param G:  Green value
   * @param B:  Blue value
   */
  void drawPixel(uint8 R, uint8 G, uint8 B);

  /**
   * @brief Write data to LCD
   * @param uiData: data to write
   */
  void writeData(uint16 uiData);

  /**
   * @brief Write command to LCD
   * @param uiCmd: command to write
   */
  void writeCommand(uint16 uiCmd);

  /**
   * @brief Write a command with one data at once;
   * @param uiCommand: command to write
   * @param uiData: data to write
   */
  void writeCommandData(uint16 uiCommand, uint16 uiData);

  /**
   * @brief Read value from display register
   * @param uiAddr: Target register to read from
   * @return Value read from register or 0 if failed.
   */
  uint16 readReg( uint16 uiAddr );

  /**
   * @brief Update data from device by reading values
   * @return Value from display
   */
  uint16 readData();

private: //member
  GPIO_InitTypeDef m_typeDefD, m_typeDefC, m_typeDefE;

  FSMC_NORSRAM_InitTypeDef m_sramInitStructure;
  FSMC_NORSRAM_TimingTypeDef m_sramTimeStruct;
  SRAM_HandleTypeDef m_hSram;
};

#endif // H_CCDISPLAYTARGET_H_
