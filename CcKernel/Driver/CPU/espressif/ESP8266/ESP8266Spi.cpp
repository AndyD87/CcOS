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
 * @brief     Implementation of class ESP8266Spi
 **/

#include "ESP8266Spi.h"
#include "ESP8266GpioPort.h"
#include "ESP8266Driver.h"
#include "CcByteArray.h"
#include "CcStatic.h"

CCEXTERNC_BEGIN
#include "spi_flash.h"
#include "driver/gpio.h"
#include "driver/spi.h"
#include "esp8266/spi_struct.h"
#include "esp8266/gpio_struct.h"
#include "FreeRTOS.h"
CCEXTERNC_END

void spi_event_callback(int event, void *arg)
{
  CCUNUSED(arg);
  switch (event)
  {
    case SPI_INIT_EVENT:
    {
      CCDEBUG("SPI_INIT_EVENT");
      break;
    }
    case SPI_TRANS_START_EVENT:
    {
      CCDEBUG("SPI_TRANS_START_EVENT");
      break;
    }
    case SPI_TRANS_DONE_EVENT:
    {
      CCDEBUG("SPI_TRANS_DONE_EVENT");
      break;
    }
    case SPI_DEINIT_EVENT:
    {
      CCDEBUG("SPI_DEINIT_EVENT");
      break;
    }
    default:
      CCDEBUG("SPI_EVENT unkown");
      break;
  }
}

ESP8266Spi::ESP8266Spi(ESP8266Driver* pDriver) :
  m_pDriver(pDriver)
{
}

ESP8266Spi::~ESP8266Spi()
{
}

CcStatus ESP8266Spi::setState(EState eState)
{
  switch(eState)
  {
    case EState::Start:
    {
      ESP8266GpioPort* pPort = m_pDriver->getGpio();
      pPort->setPinsDirection(IGpioPort::Pin12 | IGpioPort::Pin13 | IGpioPort::Pin14,
                              IGpioPin::EDirection::Alternate,
                              static_cast<size_t>(ESP8266GpioPort::EAlternate::SPI));
      if(m_eMode == EMode::Slave)
      {
        // Enable CS too
        pPort->setPinsDirection(IGpioPort::Pin15,
                                IGpioPin::EDirection::Alternate,
                                static_cast<size_t>(ESP8266GpioPort::EAlternate::SPI));
      }
      break;
    }
    case EState::Stop:
    {
      ESP8266GpioPort* pPort = m_pDriver->getGpio();
      pPort->setPinsDirection(IGpioPort::Pin12 | IGpioPort::Pin13 | IGpioPort::Pin14,
                              IGpioPin::EDirection::Input,
                              0);
      if(m_eMode == EMode::Slave)
      {
        // Enable CS too
        pPort->setPinsDirection(IGpioPort::Pin15,
                                IGpioPin::EDirection::Alternate,
                                0);
      }
      break;
    }
    default:
      // Do nothing
      break;
  }
  return ISpi::setState(eState);
}

CcStatus ESP8266Spi::setMode(EMode eMode)
{
  CCDEBUG("Start ESP8266Spi");
  CcStatus oStatus;
  if(eMode == EMode::Slave)
  {
    spi_config_t spi_config;
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:1, BYTE_TX_ORDER:1, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    // Load default interrupt enable
    // TRANS_DONE: false, WRITE_STATUS: true, READ_STATUS: true, WRITE_BUFFER: true, READ_BUFFER: ture
    spi_config.intr_enable.val = SPI_SLAVE_DEFAULT_INTR_ENABLE;
    // Set SPI to slave mode
    spi_config.mode = SPI_SLAVE_MODE;
    // Register SPI event callback function
    spi_config.event_cb = spi_event_callback;

    if(ESP_OK == spi_init(HSPI_HOST, &spi_config))
    {
      CCDEBUG("spi_init succeeded");
    }
    else
    {
      CCDEBUG("spi_init failed");
    }

    // Enable CS too
    if(m_eMode != EMode::Slave)
    {
      ESP8266GpioPort* pPort = m_pDriver->getGpio();
      pPort->setPinsDirection(IGpioPort::Pin15,
                              IGpioPin::EDirection::Alternate,
                              static_cast<size_t>(ESP8266GpioPort::EAlternate::SPI));
    }
    m_eMode = eMode;
  }
  else if(eMode == EMode::Master)
  {
    spi_config_t spi_config;
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:1, BYTE_TX_ORDER:1, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    // Load default interrupt enable
    // TRANS_DONE: true, WRITE_STATUS: false, READ_STATUS: false, WRITE_BUFFER: false, READ_BUFFER: false
    spi_config.intr_enable.val = 0;
    // Set SPI to master mode
    // ESP8266 Only support half-duplex
    spi_config.mode = SPI_MASTER_MODE;
    // Set the SPI clock frequency division factor
    spi_config.clk_div = SPI_10MHz_DIV;
    // Register SPI event callback function
    spi_config.event_cb = spi_event_callback;
    if(ESP_OK == spi_init(HSPI_HOST, &spi_config))
    {
      CCDEBUG("spi_init succeeded");
    }
    else
    {
      CCDEBUG("spi_init failed");
    }

    if(m_eMode == EMode::Slave)
    {
      ESP8266GpioPort* pPort = m_pDriver->getGpio();
      pPort->setPinsDirection(IGpioPort::Pin15,
                              IGpioPin::EDirection::Input,
                              0);
    }
    m_eMode = eMode;
  }
  return oStatus;
}

CcStatus ESP8266Spi::setFrequency(uint32 uiFrequency)
{
  CcStatus oStatus(false);
  return oStatus;
}

uint32 ESP8266Spi::getFrequency()
{
  return 26000000;
}

size_t ESP8266Spi::read(void* pBuffer, size_t uSize)
{
  CCDEBUG("spi read");
  spi_trans_t trans;
  CcStatic_memsetZeroObject(trans);
  uint16_t cmd = SPI_MASTER_WRITE_DATA_TO_SLAVE_CMD;
  uint32_t addr = 0;
  trans.cmd = &cmd;
  trans.addr = &addr;
  trans.mosi = nullptr;
  trans.miso = static_cast<uint32*>(pBuffer);
  trans.bits.val = 0;
  trans.bits.cmd = 8;
  trans.bits.mosi = uSize * 8;
  spi_trans(HSPI_HOST, trans);
  return uSize;
}

size_t ESP8266Spi::write(const void* pBuffer, size_t uSize)
{
  CCDEBUG("spi write");
  spi_trans_t trans;
  CcStatic_memsetZeroObject(trans);
  uint16_t cmd = SPI_MASTER_WRITE_DATA_TO_SLAVE_CMD;
  uint32_t addr = 0;
  trans.cmd = &cmd;
  trans.addr = &addr;
  trans.mosi = const_cast<uint32*>(static_cast<const uint32*>(pBuffer));
  trans.miso = nullptr;
  trans.bits.val = 0;
  trans.bits.cmd = 16;
  trans.bits.mosi = uSize;
  trans.bits.miso = 0;
  spi_trans(HSPI_HOST, trans);
  CCDEBUG("spi write done");
  return uSize;
}

size_t ESP8266Spi::writeRead(void* pBuffer, size_t uSize)
{
  CCDEBUG("spi write/read");
  spi_trans_t trans;
  CcStatic_memsetZeroObject(trans);
  uint16_t cmd = SPI_MASTER_READ_DATA_FROM_SLAVE_CMD;
  uint32_t addr = 0;
  trans.cmd = &cmd;
  trans.addr = &addr;
  trans.mosi = static_cast<uint32*>(pBuffer);
  trans.miso = static_cast<uint32*>(pBuffer);
  trans.bits.val = 0;
  trans.bits.cmd = 8;
  trans.bits.mosi = uSize * 8;
  spi_trans(HSPI_HOST, trans);
  return uSize;
}

CcStatus ESP8266Spi::open(EOpenFlags)
{
  CcStatus oStatus(true);
  return oStatus;
}

CcStatus ESP8266Spi::close()
{
  CcStatus oStatus(true);
  return oStatus;
}

CcStatus ESP8266Spi::cancel()
{
  CcStatus oStatus(false);
  return oStatus;
}
