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
 * @page      STM32F103
 * @subpage   STM32F103Driver
 *
 * @page      STM32F103Driver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef H_STM32F103Driver_H_
#define H_STM32F103Driver_H_

#include "STM32F103.h"
#include "IDriver.h"
#include "CcDeviceList.h"

#define SYSTEM_CLOCK_SPEED  168000000 // System clock in Hz

class CcByteArray;
class IGpioPort;

/**
 * @brief Generate SM32F407V CPU Device
 */
class STM32F103Driver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  STM32F103Driver();

  /**
   * @brief Destructor
   */
  virtual ~STM32F103Driver();

  virtual CcStatus entry() override;
  virtual CcStatus unload() override;

  IGpioPort* getGpioPort(size_t uiNr);
private:
  void setupSystem();
  void setupWatchdog();
private:
  CcDeviceList m_oSystemDevices;
};

#endif // H_STM32F103Driver_H_
