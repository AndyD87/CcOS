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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class SDD1306
 */
#pragma once

#include "CcBase.h"
#include "Devices/IDisplay.h"
#include "Devices/CcGpioPortPin.h"
#include "IIo.h"
#include "CcByteArray.h"

/**
 * @brief Display interface to SDD1306 Display driver.
 *        Currently not in use.
 */
class CcKernelSHARED SDD1306 : public IDisplay
{
public: //methods
  typedef enum
  {
    eSpi3Wire,
    eSpi4Wire,
    eI2C,
    eParallel
  } ETransportType;

  SDD1306(const CcSize& oSize, IIo& oCommunication, ETransportType eType);
  virtual ~SDD1306();
  
  virtual CcStatus onState(EState eState) override;

  /**
   * @brief not available in OLED
   */
  virtual void setBacklight(uint8) override
  {}

  virtual EType getType() const override
  { return EType::MonoChrome; }
  
  virtual void setPixel(int32 uiX, int32 uiY, const CcColor& oValue) override;
  virtual void setPixel(int32 uiX, int32 uiY, uint8 uiGreyScaleValue) override;
  virtual void setPixel(int32 uiX, int32 uiY, bool bMonochromValue) override;

  virtual void draw() override;
  virtual void fill(bool bOnOff) override;

  void setResetPin(const CcGpioPortPin oResetPin)
  { m_oResetPin = oResetPin; }
  void setCommandPin(const CcGpioPortPin oCommandPin)
  { m_oCommandPin = oCommandPin; }

private:
  void init();
  void deinit();
  void reset();
  void on(void);
  void off(void);
  void writeByte(uint8_t dat, uint8_t cmd);
  void write(void* pData, size_t uiDataSize, uint8_t uiCmd);

private:
  IIo&                m_oCommunication;
  ETransportType      m_eType;
  CcByteArray         m_oBuffer;
  CcGpioPortPin       m_oResetPin;
  CcGpioPortPin       m_oCommandPin;
};
