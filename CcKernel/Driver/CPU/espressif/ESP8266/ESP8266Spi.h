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
 * @page      ESP8266
 * @subpage   ESP8266Spi
 *
 * @page      ESP8266Spi
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ESP8266Spi
 */
#ifndef H_ESP8266Spi_H_
#define H_ESP8266Spi_H_

#include "CcBase.h"
#include "Devices/ISpi.h"
#include "CcVector.h"

class ESP8266Driver;

class ESP8266Spi : public ISpi
{
public:
  ESP8266Spi(ESP8266Driver* pDriver);
  virtual ~ESP8266Spi();

  virtual CcStatus setState(EState eState) override;
  virtual CcStatus setMode(EMode eMode) = 0;
  virtual CcStatus setFrequency(uint32 uiFrequency) = 0;
  virtual uint32 getFrequency() = 0;

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual size_t writeRead(void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

private:
  class CPrivate;
  ESP8266Driver*  m_pDriver;
  size_t          m_uiOffset = 0;
};

#endif // H_ESP8266Spi_H_
