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
 * @subpage   ESP8266Driver
 *
 * @page      ESP8266Driver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef H_ESP8266Driver_H_
#define H_ESP8266Driver_H_

#include "IDriver.h"
#include "CcDeviceList.h"

class CcByteArray;
class IGpioPort;
class ESP8266Wlan;
class ESP8266Cpu;
class ESP8266GpioPort;
class ESP8266Eeprom;
class ESP8266Spi;
class ESP8266Clk;
class ESP8266Timer;

/**
 * @brief Generate SM32F407V CPU Device
 */
class ESP8266Driver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  ESP8266Driver();

  /**
   * @brief Destructor
   */
  virtual ~ESP8266Driver();

  virtual CcStatus entry() override;
  virtual CcStatus unload() override;

  static ESP8266Driver* getInstance()
    { return s_pInstance; }
  void setupDrivers();

  bool event(void* event);

  ESP8266GpioPort* getGpio()
    { return m_pGpio; }

private:
  static ESP8266Driver* s_pInstance;
  ESP8266Wlan*      m_pWlan = nullptr;
  ESP8266Cpu*       m_pCpu = nullptr;
  ESP8266GpioPort*  m_pGpio = nullptr;
  ESP8266Eeprom*    m_pEeprom = nullptr;
  ESP8266Spi*       m_pSpi = nullptr;
  ESP8266Clk*       m_pClk = nullptr;
  ESP8266Timer*     m_pTimer = nullptr;
};

CCEXTERNC void app_main();

#endif // H_ESP8266Driver_H_
