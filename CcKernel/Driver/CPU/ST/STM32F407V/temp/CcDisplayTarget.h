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
 * @page      STM32F407V
 * @subpage   CcDisplayTarget
 * 
 * @page      CcDisplayTarget
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcDisplayTarget
 */
#ifndef _CCDISPLAYTARGET_H_
#define _CCDISPLAYTARGET_H_

#include "CcBase.h"
#include "STM32.h"
#include "dev/CcDisplay.h"
#include "dev/CcTimer.h"

/**
 * @brief Should become a hardware Display. But it is not working at the moment
 */
class CcDisplayTarget : public CcDisplay
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
  void setCursor( uint16 x, uint16 y );
  void setAddress( uint16 x, uint16 y, uint16 xSize, uint16 ySize);
  void setBacklight( uint8 uiBrightness );
  void initIO( void );
  void initBus( void );
  void drawPixel(uchar R, uchar G, uchar B);
  /**
   * @brief Write data to LCD
   * @param uiData: data to write
   */

  void writeData(uint16 uiData);
  /**
   * @brief Write command to LCD
   * @param uiData: command to write
   */
  void writeCommand(uint16 uiCmd);

  /**
   * @brief Write a command with one data at once;
   * @param uiCommand: command to write
   * @param uiData: data to write
   */
  void writeCommandData(uint16 uiCommand, uint16 Data);
  uint16 readReg( uint16 uiAddr );
  uint16 readData( void );

private: //member
  GPIO_InitTypeDef m_typeDefD, m_typeDefC, m_typeDefE;

  FSMC_NORSRAM_InitTypeDef m_sramInitStructure;
  FSMC_NORSRAM_TimingTypeDef m_sramTimeStruct;
  SRAM_HandleTypeDef m_hSram;
};

#endif /* _CCDISPLAYTARGET_H_ */
