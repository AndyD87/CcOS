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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IBoardSupport
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "CcPoint.h"
#include "CcArray.h"
#include "CcVector.h"
#include "CcString.h"

/**
 * @brief Communication Device for I2C
 */
class CcKernelSHARED IBoardSupport : public IDevice
{
public:
  /**
   * @brief Constructor
   */
  IBoardSupport();

  /**
   * @brief Destructor
   */
  virtual ~IBoardSupport();

  class CPortPin
  {
  public:
    uint16 uiPort;
    uint16 uiPin;
  };

  class CFunction
  {
  public:
    EDeviceType           eFunction;
    uint16                uiFunctionNr;
    CcVector<CPortPin>    oRequiredPins;
    CcVector<CPortPin>    oOptinalPins;
  };

  class CHwPin
  {
  public:
    uint32 uiFunctionNr   = UINT32_MAX;           //!< Number of function in board support list this pin is mapped to, or UINT32_MAX if not.
    CcVector<EDeviceType> oSupportedFunctions;
    uint16 uiSoftwarePort = UINT16_MAX;
    uint16 uiSoftwarePin  = UINT16_MAX;
    CcString sDescription;
  };

  class CHwPort
  {
    CcVector<CHwPin> oHwPins;
  };

protected:
  CcVector<CFunction>    m_oFunctions;
  CcVector<CHwPort>      m_oPorts;
};
