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
 *
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ESP8266Eeprom
 */
#ifndef H_ESP8266Eeprom_H_
#define H_ESP8266Eeprom_H_

#include "CcBase.h"
#include "Devices/IEeprom.h"
#include "ESP8266Cpu.h"
#include "CcVector.h"

class ESP8266Eeprom : public IEeprom
{
public:
  ESP8266Eeprom(ESP8266Cpu* pCpu);
  virtual ~ESP8266Eeprom();
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus setPosition(size_t uiPosition) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  virtual size_t size() const override;
private:
  class CPrivate;
  ESP8266Cpu* m_pCpu;
  size_t      m_uiOffset = 0;
};

#endif // H_ESP8266Eeprom_H_
