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
 * @brief     Implementation of Class STM32F4DiscoveryLed
 *
 *            Control LED on STM32F4Discovery.
 *            LEDs are connected on PORTD 12-15
 */

#include "STM32F4DiscoveryLed.h"
#include "CcKernel.h"
#include <stm32f4xx_hal.h>
#include "CcStatic.h"

class STM32F4DiscoveryLedPrivate
{
public:
  uint16 uiPinNr          = 0;
};

STM32F4DiscoveryLed::STM32F4DiscoveryLed(uint8 uiLedNr)
{
  m_pPrivate = new STM32F4DiscoveryLedPrivate();
  CCMONITORNEW(m_pPrivate);
  if(uiLedNr < 4)
  {
    m_pPrivate->uiPinNr   = 1 << (12 + uiLedNr);
    configurePin();
  }
}

STM32F4DiscoveryLed::~STM32F4DiscoveryLed()
{
  CCDELETE(m_pPrivate);
}

CcStatus STM32F4DiscoveryLed::setMaxBirghtness(uint16 uiBrightness)
{
  CcStatus oSuccess = false;
  CCUNUSED(uiBrightness);
  return oSuccess;
}

CcStatus STM32F4DiscoveryLed::on(uint16 uiBrightness)
{
  CcStatus oSuccess = false;
  HAL_GPIO_WritePin(GPIOD, m_pPrivate->uiPinNr, GPIO_PIN_SET);
  return oSuccess;
}

CcStatus STM32F4DiscoveryLed::off()
{
  CcStatus oSuccess = false;
  HAL_GPIO_WritePin(GPIOD, m_pPrivate->uiPinNr, GPIO_PIN_RESET);
  return oSuccess;
}

CcStatus STM32F4DiscoveryLed::toggle()
{
  CcStatus oSuccess = true;
  HAL_GPIO_TogglePin(GPIOD, m_pPrivate->uiPinNr);
  return oSuccess;
}

bool STM32F4DiscoveryLed::IsOn()
{
  return HAL_GPIO_ReadPin(GPIOD, m_pPrivate->uiPinNr) == GPIO_PIN_SET;
}

void STM32F4DiscoveryLed::configurePin()
{
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitTypeDef oGpioInitStruct;
  CcStatic_memsetZeroObject(oGpioInitStruct);
  oGpioInitStruct.Alternate = false;
  oGpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  oGpioInitStruct.Pin   = m_pPrivate->uiPinNr;
  oGpioInitStruct.Pull  = GPIO_NOPULL;
  oGpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &oGpioInitStruct);
}
