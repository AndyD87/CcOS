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
 * @brief     Class MCP23017
 **/
#pragma once

#include "CcBase.h"
#include "CcByteArray.h"
#include "Devices/IGpioPort.h"

class II2CClient;
class IGpioPort;
class IGpioPin;

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED MCP23017 : public IGpioPort
{
public:
  /**
   * @brief Constructor
   */
  MCP23017(II2CClient* pI2CDevice);

  /**
   * @brief Destructor
   */
  virtual ~MCP23017();

  virtual CcStatus onState(EState eState) override;

  virtual inline uint8 count() const override
  { return 16; }
  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;
  virtual IGpioPin::EDirection getDirection(size_t uiPin) override;
  virtual bool setPinValue(size_t uiPin, bool bValue) override;
  virtual bool getPinValue(size_t uiPin) override;
  virtual bool setValue(size_t uiValue) override;
private:

private:
  II2CClient* m_pII2CDevice;
};
