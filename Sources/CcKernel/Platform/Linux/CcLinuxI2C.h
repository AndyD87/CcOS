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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxI2C
 */
#pragma once

#include "CcBase.h"
#include "Devices/II2C.h"
#include "CcByteArray.h"

class OrangePiZeroBoardSupport;

/**
 * @brief Control the Input and Output ports on device
 */
class CcKernelSHARED CcLinuxI2C : public II2C
{
public:
  class CSlave;

  CcLinuxI2C(uint8 uiNr) :
    m_uiNr(uiNr)
  {}

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxI2C() = default;

  virtual CcStatus onState(EState eState) override;

  virtual II2CSlave* createInterface(uint16 uiAddress) override;

private:
  uint8                     m_uiNr;
};
