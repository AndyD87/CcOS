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
  m_oPins.resize(count(), nullptr);
  m_oDirections.resize(count(), IGpioPin::EDirection::Unknown);
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
      setDirection(i, eDirection, uiValue);
    }
  }
  return bSuccess;
}

bool ESP8266GpioPort::setDirection(size_t uiPin, IGpioPin::EDirection eDirection, size_t uiValue)
{
  bool bSuccess = true;
  if(uiPin < count())
  {
    switch(eDirection)
    {
      case IGpioPin::EDirection::Input:
        gpio_set_direction(static_cast<gpio_num_t>(uiPin), gpio_mode_t::GPIO_MODE_INPUT);
        m_oDirections[uiPin] = eDirection;
        break;
      case IGpioPin::EDirection::Output:
        gpio_set_direction(static_cast<gpio_num_t>(uiPin), gpio_mode_t::GPIO_MODE_OUTPUT);
        m_oDirections[uiPin] = eDirection;
        break;
      case IGpioPin::EDirection::Alternate:
        switch(uiPin)
        {
          case 12:
            if(uiValue == static_cast<size_t>(EAlternate::SPI))
              PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2); // configure io to spi MISO
            else
              bSuccess = false;
            break;
          case 13:
            if(uiValue == static_cast<size_t>(EAlternate::SPI))
              PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2); // configure io to spi MOSI
            else
              bSuccess = false;
            break;
          case 14:
            if(uiValue == static_cast<size_t>(EAlternate::SPI))
              PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2); // configure io to spi CLK
            else
              bSuccess = false;
            break;
          case 15:
            if(uiValue == static_cast<size_t>(EAlternate::SPI))
              PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2); // configure io to spi CS
            else
              bSuccess = false;
            break;
          default:
            bSuccess = false;
        }
        if(bSuccess)
        {
          m_oDirections[uiPin] = eDirection;
        }
        break;
      default:
        gpio_set_direction(static_cast<gpio_num_t>(uiPin), gpio_mode_t::GPIO_MODE_DISABLE);
        m_oDirections[uiPin] = IGpioPin::EDirection::Unknown;
        bSuccess = false;
    }
  }
  return bSuccess;
}

IGpioPin::EDirection ESP8266GpioPort::getDirection(size_t uiPin)
{
  IGpioPin::EDirection eDirection = IGpioPin::EDirection::Unknown;
  if(uiPin < count())
  {
    eDirection = m_oDirections[uiPin];
  }
  return eDirection;
}

bool ESP8266GpioPort::setValue(size_t uiPin, bool bValue)
{
  if(uiPin < count())
  {
    return ESP_OK == gpio_set_level(static_cast<gpio_num_t>(uiPin), bValue);
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
