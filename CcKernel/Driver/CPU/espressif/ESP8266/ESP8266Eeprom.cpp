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
 * @brief     Implementation of class ESP8266Eeprom
 **/

#include <Driver/CPU/espressif/ESP8266/ESP8266Eeprom.h>
#include "CcByteArray.h"
#include "CcStatic.h"
CCEXTERNC_BEGIN
#include <spi_flash.h>
CCEXTERNC_END

//! Size of block on eeprom
#define EEPROM_SIZE SPI_FLASH_SEC_SIZE

//! Flash offset of EEPROM
const uintptr _FLASH_start = 0x40200000;
//! Reserve size by assigning static variable on it for reservation
const char ESP8266Eeprom_Space[EEPROM_SIZE] __attribute__((section(".ESP8266Eeprom_Section"))) = {60};

class ESP8266Eeprom::CPrivate
{
public:
  static const uint32 uiEepromSector;
};

const uintptr ESP8266Eeprom::CPrivate::uiEepromSector = ((reinterpret_cast<uintptr>(ESP8266Eeprom_Space) - _FLASH_start) / SPI_FLASH_SEC_SIZE);

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
  if(m_uiOffset + uSize <= EEPROM_SIZE)
  {
    esp_err_t iRet = spi_flash_read(
          (CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE) + m_uiOffset,
          pBuffer,
          uSize);
    if(iRet == ESP_OK)
    {
      uiRet = uSize;
      m_uiOffset += uSize;
    }
    else
    {
      CCERROR("Read failed with: " + CcString::fromNumber(iRet));
      CCERROR("     Buffer     : " + CcString::fromNumber(reinterpret_cast<uintptr>(pBuffer), 16));
      CCERROR("     Size       : " + CcString::fromNumber(uSize, 16));
      CCERROR("     Address    : " + CcString::fromNumber((CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE) + m_uiOffset, 16));
      CCERROR("     SectorStart: " + CcString::fromNumber(CPrivate::uiEepromSector, 16));
      CCERROR("     FlashStart : " + CcString::fromNumber(reinterpret_cast<uintptr>(_FLASH_start), 16));
      CCERROR("     EepromStart: " + CcString::fromNumber(reinterpret_cast<uintptr>(ESP8266Eeprom_Space), 16));
      CCERROR(esp_err_to_name(iRet));
    }
  }
  else if(m_uiOffset < size())
  {
    size_t uiReadSize = size() - m_uiOffset;
    esp_err_t iRet = spi_flash_read(
          (CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE) + m_uiOffset,
          pBuffer,
          uiReadSize);
    if(iRet == ESP_OK)
    {
      uiRet = uiReadSize;
      m_uiOffset += uiReadSize;
    }
    else
    {
      CCERROR("Read failed with: " + CcString::fromNumber(iRet));
      CCERROR(esp_err_to_name(iRet));
    }
  }
  m_pCpu->leaveCriticalSection();
  return uiRet;
}

size_t ESP8266Eeprom::write(const void* pBuffer, size_t uSize)
{
  size_t uiRet = SIZE_MAX;
  if(m_uiOffset + uSize <= EEPROM_SIZE)
  {
    CCERROR("Write EEPROM at: " + CcString::fromSize(m_uiOffset));
    // Make a copy from full sector before delete
    CCNEWARRAYTYPE(pData, char, EEPROM_SIZE);

    esp_err_t iRet = spi_flash_read(
          CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE,
          pData,
          EEPROM_SIZE);
    if(iRet == ESP_OK)
    {
      CcStatic::memcpy(static_cast<char*>(pData) + m_uiOffset, pBuffer, uSize);
      m_uiOffset += uSize;

      esp_err_t iRet = spi_flash_erase_sector(CPrivate::uiEepromSector);
      if(iRet == ESP_OK)
      {
        m_pCpu->enterCriticalSection();
        iRet = spi_flash_write(CPrivate::uiEepromSector * SPI_FLASH_SEC_SIZE, pData, EEPROM_SIZE);
        m_pCpu->leaveCriticalSection();
        if(iRet == ESP_OK)
          uiRet = uSize;
        else
        {
          CCERROR("Write failed with: " + CcString::fromNumber(iRet));
          CCERROR(esp_err_to_name(iRet));
        }
      }
      else
      {
        CCERROR("Erase failed with: " + CcString::fromNumber(iRet));
        CCERROR(esp_err_to_name(iRet));
      }
      CCDELETEARR(pData);
    }
  }
  else
  {
    CCERROR("Out of EEPROM memory");
  }
  return uiRet;
}

CcStatus ESP8266Eeprom::setPosition(size_t uiPosition)
{
  CcStatus oStatus(EStatus::OutOfRange);
  if(uiPosition < size())
  {
    m_uiOffset = uiPosition;
    oStatus = true;
  }
  return oStatus;
}

CcStatus ESP8266Eeprom::open(EOpenFlags)
{
  CcStatus oStatus(true);
  m_uiOffset = 0;
  return oStatus;
}

CcStatus ESP8266Eeprom::close()
{
  CcStatus oStatus(true);
  return oStatus;
}

CcStatus ESP8266Eeprom::cancel()
{
  CcStatus oStatus(false);
  return oStatus;
}

size_t ESP8266Eeprom::size() const
{
  return EEPROM_SIZE;
}
