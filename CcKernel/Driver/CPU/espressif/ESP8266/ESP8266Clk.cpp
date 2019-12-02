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
 * @brief     Implementation of class ESP8266Clk
 **/

#include "ESP8266Clk.h"
#include "ESP8266GpioPort.h"
#include "ESP8266Driver.h"
#include "CcByteArray.h"
#include "CcStatic.h"

CCEXTERNC_BEGIN
#include "FreeRTOS.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp8266/pin_mux_register.h"
CCEXTERNC_END

ESP8266Clk::ESP8266Clk()
{
}

ESP8266Clk::~ESP8266Clk()
{
}

CcStatus ESP8266Clk::setFrequency(uint64 uiHz)
{
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_CLK_XTAL);
  return true;
}
