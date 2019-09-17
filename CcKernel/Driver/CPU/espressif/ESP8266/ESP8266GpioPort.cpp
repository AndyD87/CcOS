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
 * @brief     Implementation of class ESP8266GpioPort
 **/

#include <Driver/CPU/espressif/ESP8266/ESP8266GpioPort.h>
CCEXTERNC_BEGIN
#include <driver/gpio.h>
CCEXTERNC_END


ESP8266GpioPort::ESP8266GpioPort()
{
  m_pPins.resize(count(), nullptr);
}

ESP8266GpioPort::~ESP8266GpioPort()
{
}

uint8 ESP8266GpioPort::count() const
{
  return 17;
}

bool ESP8266GpioPort::setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue)
{
  CCUNUSED(uiValue);
  bool bSuccess = true;
  for(size_t i = 0; i < count(); i++)
  {
    if((1 << i) | uiPinMask)
    {
      setDirection(i, eDirection);
      setAlternateValue(i, uiValue);
    }
  }
  return bSuccess;
}

bool ESP8266GpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection)
{
  bool bSuccess = true;
  switch(eDirection)
  {
    case IGpioPin::EDirection::Input:
      gpio_set_direction(static_cast<gpio_num_t>(uiPin), gpio_mode_t::GPIO_MODE_INPUT);
      break;
    case IGpioPin::EDirection::Output:
      gpio_set_direction(static_cast<gpio_num_t>(uiPin), gpio_mode_t::GPIO_MODE_OUTPUT);
      break;
    default:
      gpio_set_direction(static_cast<gpio_num_t>(uiPin), gpio_mode_t::GPIO_MODE_DISABLE);
      bSuccess = false;
  }
  return bSuccess;
}

IGpioPin::EDirection ESP8266GpioPort::getDirection(size_t uiPin)
{
  CCUNUSED(uiPin);
  return IGpioPin::EDirection::Unknown;
}

bool ESP8266GpioPort::setValue(size_t uiPin, bool bValue)
{
  if(uiPin < count())
  {
    gpio_set_level(static_cast<gpio_num_t>(uiPin), bValue);
    return true;
  }
  else
  {
    return false;
  }
}

bool ESP8266GpioPort::getValue(size_t uiPin)
{
  return gpio_get_level(static_cast<gpio_num_t>(uiPin)) != 0;
}
