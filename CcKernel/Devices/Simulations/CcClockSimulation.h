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
 * @page      Simulations
 * @subpage   CcClockSimulation
 *
 * @page      CcClockSimulation
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcClockSimulation
 */

#ifndef H_CcClockSimulation_H_
#define H_CcClockSimulation_H_

#include "CcBase.h"
#include "CcBase.h"
#include "Devices/IClock.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcClockSimulation : public IClock
{
public:
  /**
   * @brief Destructor
   */
  virtual ~CcClockSimulation() override = default;

  virtual CcStatus setFrequency(uint64 uiHz) override;
private:
  uint64 m_uiFrequency = 0;
};

#endif // H_CcClockSimulation_H_
