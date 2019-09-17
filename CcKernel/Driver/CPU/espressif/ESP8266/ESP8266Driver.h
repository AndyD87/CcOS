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

private:
  static ESP8266Driver* s_pInstance;
  ESP8266Wlan*      m_pWlan;
  ESP8266Cpu*       m_pCpu;
  ESP8266GpioPort*  m_pGpio;
};

#endif // H_ESP8266Driver_H_
