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
 * @page      STM32F207IG
 * @subpage   ESP8266Driver
 *
 * @page      ESP8266Driver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef _ESP8266Driver_H_
#define _ESP8266Driver_H_

#include "IDriver.h"
#include "CcDeviceList.h"

class CcByteArray;
class IGpioPort;

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

private:
  CcDeviceList m_oSystemDevices;
};

#endif /* _ESP8266Driver_H_ */
