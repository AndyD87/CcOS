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
 * @page      Devices
 * @subpage   CcGpioPinSimulation
 *
 * @page      CcGpioPinSimulation
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGpioPinSimulation
 */

#ifndef H_CcGpioPinSimulation_H_
#define H_CcGpioPinSimulation_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Devices/IGpioPin.h"

/**
 * @brief Control for General Purpose Input Output Pins
 */
class CcKernelSHARED CcGpioPinSimulation : public IGpioPin
{
public:
  /**
   * @brief Destructor
   */
  virtual ~CcGpioPinSimulation() = default;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param EDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection( EDirection eDirection) override
    { m_eDirection = eDirection; return true; }

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual EDirection getDirection() override
    { return m_eDirection;}

  /**
   * @brief Set Value of Output pin
   * @param bValue: value to set
   */
  virtual void setValue(bool bValue) override
    { m_bState = bValue; }

  /**
   * @brief Get current value of pin in Input mode. If Output, last set value should be returned.
   * @return Value of pin.
   */
  virtual bool getValue() override
    { return m_bState; }

  virtual bool toggle() override
    { m_bState = !m_bState; return true; }
  virtual void setAlternateValue(size_t uiValue) override
    { CCUNUSED(uiValue); }
  virtual void setSpeedValue(size_t uiValue) override
    { CCUNUSED(uiValue); }
  virtual void reconfigure() override
    {}
private:
  bool       m_bState = 0;
  EDirection m_eDirection = EDirection::Input;
};

#endif /* H_CcGpioPinSimulation_H_ */
