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
 * @page      DP83848
 * @subpage   DP83848Driver
 *
 * @page      DP83848Driver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef _DP83848Driver_H_
#define _DP83848Driver_H_

#include "DP83848.h"
#include "Devices/INetwork.h"
#include "IDriver.h"

#define SYSTEM_CLOCK_SPEED  168000000 // System clock in Hz

class CcByteArray;

/**
 * @brief Generate SM32F407V CPU Device
 */
class DP83848Driver : public IDriver
{
public:
  /**
   * @brief Destructor
   */
  virtual ~DP83848Driver();

  virtual CcStatus entry() override;
  virtual CcStatus unload() override;

private:
  INetwork* m_pNetworkDevice = nullptr;
};

#endif /* _DP83848Driver_H_ */
