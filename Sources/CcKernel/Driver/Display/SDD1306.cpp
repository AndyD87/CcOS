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
 * @brief     Implementation of class SDD1306
 **/

#include "SDD1306.h"
#include "CcKernel.h"

 //-------------Write commands and WRITE_DATA definitions-------------------
#define WRITE_COMMAND     0	//!< write command
#define WRITE_DATA        1	//!< write data

#define PAGE_SIZE    8          //page size
#define XLevelL		   0x02       //column low address
#define XLevelH		   0x10       //column high address
#define YLevel       0xB0       //page address
#define	Brightness	 0xFF

#define CMD_SETLOWCOLUMN          0x00
#define CMD_EXTERNALVCC           0x01
#define CMD_SWITCHCAPVCC          0x02
#define CMD_SETHIGHCOLUMN         0x10
#define CMD_MEMORYMODE            0x20
#define CMD_COLUMNADDR            0x21
#define CMD_PAGEADDR              0x22
#define CMD_SETSTARTLINE          0x40
#define CMD_SETCONTRAST           0x81
#define CMD_CHARGEPUMP            0x8D
#define CMD_SEGREMAP              0xA1
#define CMD_DISPLAYALLON_RESUME   0xA4
#define CMD_DISPLAYALLON          0xA5
#define CMD_NORMALDISPLAY         0xA6
#define CMD_INVERTDISPLAY         0xA7
#define CMD_SETMULTIPLEX          0xA8
#define CMD_DISPLAYOFF            0xAE
#define CMD_DISPLAYON             0xAF
#define CMD_PAGESTARTADDRESS      0xB0
#define CMD_COMSCANINC            0xC0
#define CMD_COMSCANDEC            0xC8
#define CMD_SETDISPLAYOFFSET      0xD3
#define CMD_SETDISPLAYCLOCKDIV    0xD5
#define CMD_SETPRECHARGE          0xD9
#define CMD_SETCOMPINS            0xDA
#define CMD_SETVCOMDETECT         0xDB

SDD1306::SDD1306(const CcSize& oSize, IIo& oCommunication, ETransportType eType) :
  IDisplay(oSize),
  m_oCommunication(oCommunication),
  m_eType(eType)
{
  m_oBuffer.resize(getAreaSize() / 8);
}

SDD1306::~SDD1306()
{
}

void SDD1306::setPixel(int32 uiX, int32 uiY, const CcColor& oValue)
{
  setPixel(uiX, uiY, oValue.toMonoChrome());
}

void SDD1306::setPixel(int32 uiX, int32 uiY, uint8 uiGreyScaleValue)
{
  setPixel(uiX, uiY, uiGreyScaleValue > 0);
}

void SDD1306::setPixel(int32 uiX, int32 uiY, bool bMonochromValue)
{
  size_t uiPos = (uiY / PAGE_SIZE) * getWidth() + uiX;
  if (uiPos < m_oBuffer.size())
  {
    if (bMonochromValue)
    {
      m_oBuffer[uiPos] |= (1 << (uiY%PAGE_SIZE)) & 0xff;
    }
    else
    {
      m_oBuffer[uiPos] &= ~((1 << (uiY%PAGE_SIZE)) & 0xff);
    }
  }
}

void SDD1306::draw()
{
  uint8 i, n;
  for (i = 0; i < PAGE_SIZE; i++)
  {
    WriteByte(YLevel + i, WRITE_COMMAND); //Set page address(0~7)
    WriteByte(XLevelL, WRITE_COMMAND);    //Set column low address
    WriteByte(XLevelH, WRITE_COMMAND);    //Set column high address
    for (n = 0; n < getWidth(); n++)
    {
      WriteByte(m_oBuffer[i*getWidth() + n], WRITE_DATA);
    }
  }
}

void SDD1306::Init()
{
  
  Reset();     //reset OLED

	/**************initialise SH1106*****************/
	Off(); //power off
	WriteByte(CMD_DISPLAYOFF,		      WRITE_COMMAND);//--turn off oled panel
	WriteByte(CMD_SWITCHCAPVCC,			  WRITE_COMMAND);//---set low column address
	WriteByte(CMD_SETHIGHCOLUMN,			WRITE_COMMAND);//---set high column address
	WriteByte(CMD_SETSTARTLINE,			  WRITE_COMMAND);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	WriteByte(CMD_SETCONTRAST,			  WRITE_COMMAND);//--set contrast control register
	//WriteByte(0xCF,			WRITE_COMMAND); // Set SEG Output Current Brightness
	//WriteByte(0xA1,			WRITE_COMMAND);//--Set SEG/Column Mapping     0xa0 Left and right inversion 0xa1 normal
	WriteByte(CMD_COMSCANDEC,			    WRITE_COMMAND);//Set COM/Row Scan Direction   0xc0 Up and Down inversion 0xc8 normal
	WriteByte(CMD_NORMALDISPLAY,			WRITE_COMMAND);//--set normal display
	WriteByte(CMD_SETMULTIPLEX,			  WRITE_COMMAND);//--set multiplex ratio(1 to 64)
	WriteByte(0x3f,			              WRITE_COMMAND);//--1/64 duty
	WriteByte(CMD_SETDISPLAYOFFSET,		WRITE_COMMAND);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	WriteByte(0x00,			              WRITE_COMMAND);//-not offset
	WriteByte(CMD_SETDISPLAYCLOCKDIV,	WRITE_COMMAND);//--set display clock divide ratio/oscillator frequency
	WriteByte(0x80,			              WRITE_COMMAND);//--set divide ratio, Set Clock as 100 Frames/Sec
	WriteByte(CMD_SETPRECHARGE,			  WRITE_COMMAND);//--set pre-charge period
	WriteByte(0xF1,			              WRITE_COMMAND);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	WriteByte(CMD_SETCOMPINS,			    WRITE_COMMAND);//--set com pins hardware configuration
	WriteByte(0x12,			              WRITE_COMMAND);
	WriteByte(CMD_SETVCOMDETECT,			WRITE_COMMAND);//--set vcomh
	WriteByte(0x40,			              WRITE_COMMAND);//Set VCOM Deselect Level
	WriteByte(CMD_MEMORYMODE,			    WRITE_COMMAND);//-Set Page Addressing Mode (0x00/0x01/0x02)
	WriteByte(0x02,			              WRITE_COMMAND);//
	WriteByte(CMD_CHARGEPUMP,			    WRITE_COMMAND);//--set Charge Pump enable/disable
	WriteByte(0x14,			              WRITE_COMMAND);//--set(0x10) disable
	WriteByte(CMD_DISPLAYALLON_RESUME,WRITE_COMMAND);// Disable Entire Display On (0xa4/0xa5)
	WriteByte(CMD_NORMALDISPLAY,			WRITE_COMMAND);// Disable Inverse Display On (0xa6/a7)
	WriteByte(CMD_DISPLAYON,          WRITE_COMMAND);//--turn on oled panel
	On();          // power on
}

void SDD1306::Deinit()
{
  Off();
}

void SDD1306::Reset()
{
  // switch reset pin high
  m_oResetPin.setValue(true);
  CcKernel::sleep(10);
  // switch reset pin low
  m_oResetPin.setValue(false);
  CcKernel::sleep(10);
  // switch reset pin high
  m_oResetPin.setValue(true);
}

void SDD1306::On()
{
  WriteByte(CMD_CHARGEPUMP, WRITE_COMMAND); // SET DCDC command
  WriteByte(0X14,           WRITE_COMMAND); // DCDC ON
  WriteByte(CMD_DISPLAYON,  WRITE_COMMAND); // DISPLAY ON
}

void SDD1306::Off()
{
  WriteByte(CMD_CHARGEPUMP, WRITE_COMMAND);	// SET DCDC command
  WriteByte(0X10,           WRITE_COMMAND); // DCDC OFF
  WriteByte(CMD_DISPLAYOFF, WRITE_COMMAND);	// DISPLAY OFF
}

void SDD1306::WriteByte(uint8_t uiData, uint8_t uiCmd)
{
  switch (m_eType)
  {
  case ETransportType::eSpi3Wire:
  {
    uint8 oWRITE_DATA[2];
    oWRITE_DATA[0]  = uiCmd == WRITE_COMMAND ? 0x00 : 0x80;
    oWRITE_DATA[0] |= uiData >> 1;
    oWRITE_DATA[1]  = uiData << 7;
    m_oCommunication.write(oWRITE_DATA, sizeof(oWRITE_DATA));
    break;
  }
  default:
    // @todo switch COMMAND pin low for command otherwise false
    if(uiCmd)
    m_oCommandPin.setValue(true);
    m_oCommunication.write(&uiData, sizeof(uiData));
    break;
  }
}
