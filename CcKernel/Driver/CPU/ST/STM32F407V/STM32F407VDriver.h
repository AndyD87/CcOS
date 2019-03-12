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
 * @page      STM32F407V
 * @subpage   STM32F407VDriver
 *
 * @page      STM32F407VDriver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef _STM32F407VDriver_H_
#define _STM32F407VDriver_H_

#include "STM32F407V.h"
#include "IDriver.h"
#include "CcDeviceList.h"

#define SYSTEM_CLOCK_SPEED  168000000 // System clock in Hz

class CcByteArray;

/**
 * @brief Generate SM32F407V CPU Device
 */
class STM32F407VDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  STM32F407VDriver();

  /**
   * @brief Destructor
   */
  virtual ~STM32F407VDriver();

  virtual CcStatus entry() override;
  virtual CcStatus unload() override;
private:
  void setupSystem();
  void setupWatchdog();
private:
  CcList<IDevice*> m_oSystemDevices;
};

#endif /* _STM32F407VDriver_H_ */
