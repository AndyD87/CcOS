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
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class ESP8266Spi
 */
#pragma once

#include "CcBase.h"
#include "Devices/ISpi.h"
#include "CcVector.h"
#include "CcMutex.h"

class ESP8266Driver;

/**
 * @brief ESP8266 SPI interface
 */
class ESP8266Spi : public ISpi
{
public:
  /**
   * @brief Create interface and setup ports and pins as required.
   * @param pDriver: Driver with interfaces to other devices for configuration.
   */
  ESP8266Spi(ESP8266Driver* pDriver);
  virtual ~ESP8266Spi();

  virtual CcStatus onState(EState eState) override;
  virtual CcStatus setMode(EMode eMode) override;
  virtual CcStatus setFrequency(uint32 uiFrequency) override;
  virtual uint32 getFrequency() override;

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual size_t writeRead(void* pWriteBuffer, void* pReadBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

  /**
   * @brief Event from ESP8266 has received, like transfer or read is complete.
   * @param event:  Target event type
   * @param arg:    Additional infromations for target event
   */
  static void eventReceived(int event, void *arg);
private:
  void writeNext();

private:
  class CPrivate;
  ESP8266Driver*  m_pDriver;
  size_t          m_uiOffset = 0;
  CcMutex         m_oTransferLock;
  const char*     m_pCurrentTransfer = nullptr;
  const char*     m_pCurrentReceive = nullptr;
  size_t          m_uiCurrentTransferSize = 0;
  static ESP8266Spi* s_pSpi;
};
