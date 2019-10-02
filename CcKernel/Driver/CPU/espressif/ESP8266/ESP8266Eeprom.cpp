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
 * @brief     Implementation of class ESP8266Eeprom
 **/

#include <Driver/CPU/espressif/ESP8266/ESP8266Eeprom.h>
#include "CcByteArray.h"
#include "CcStatic.h"
CCEXTERNC_BEGIN
#include <spi_flash.h>
CCEXTERNC_END

extern "C" uint32_t _EEPROM_start;

class ESP8266Eeprom::CPrivate
{
public:
  static uint32 uiEepromSector;
};

uint32 ESP8266Eeprom::CPrivate::uiEepromSector = (((uint32)&_EEPROM_start - 0x40200000) / SPI_FLASH_SEC_SIZE);

ESP8266Eeprom::ESP8266Eeprom(ESP8266Cpu* pCpu) :
  m_pCpu(pCpu)
{
}

ESP8266Eeprom::~ESP8266Eeprom()
{
}

size_t ESP8266Eeprom::read(void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  m_pCpu->enterCriticalSection();
  if(uSize <= SPI_FLASH_SEC_SIZE)
  {
    esp_err_t iRet = spi_flash_read(CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE, pBuffer, uSize);
    if(iRet == ESP_OK)
      uiRet = uSize;
  }
  m_pCpu->leaveCriticalSection();
  return uiRet;
}

size_t ESP8266Eeprom::write(const void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  char* pData = nullptr;
  if(uSize <= SPI_FLASH_SEC_SIZE)
  {
    if(uSize < SPI_FLASH_SEC_SIZE)
    {
      // Make a copy from full sector before delete
      CCNEWARRAY(pData, char, SPI_FLASH_SEC_SIZE);
      read(pData, SPI_FLASH_SEC_SIZE);
      CcStatic::memcpy(pData, pBuffer, uSize);
    }
    esp_err_t iRet = spi_flash_erase_sector(CPrivate::uiEepromSector);
    if(iRet == ESP_OK)
    {
      m_pCpu->enterCriticalSection();
      if(pData == nullptr)
      {
        iRet = spi_flash_write(CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE, pBuffer, SPI_FLASH_SEC_SIZE);
      }
      else
      {
        iRet = spi_flash_write(CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE, pData, SPI_FLASH_SEC_SIZE);
      }
      m_pCpu->leaveCriticalSection();
      if(iRet == ESP_OK)
        uiRet = uSize;
    }
  }
  CCDELETEARR(pData);
  return uiRet;
}

CcStatus ESP8266Eeprom::open(EOpenFlags)
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus ESP8266Eeprom::close()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus ESP8266Eeprom::cancel()
{
  CcStatus oStatus(false);
  return oStatus;
}

size_t ESP8266Eeprom::size() const
{
  return SPI_FLASH_SEC_SIZE;
}
