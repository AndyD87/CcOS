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
 * @subpage   CcGpioPortPin
 *
 * @page      CcGpioPortPin
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGpioPortPin
 */

#ifndef H_CcGpioPortPin_H_
#define H_CcGpioPortPin_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IDevice.h"
#include "IGpioPin.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcGpioPortPin : public IGpioPin
{
public:
  /**
   * @brief Constructor
   */
  CcGpioPortPin(IGpioPort* pPort, uint8 uiPinNr) :
    m_pPort(pPort),
    m_uiPinNr(uiPinNr)
  {}
  /**
   * @brief Destructor
   */
  virtual ~CcGpioPortPin() = default;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param EDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection(EDirection eDirection) override;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual EDirection getDirection() override;

  /**
   * @brief Set Value of Output pin
   * @param bValue: value to set
   */
  virtual void setValue(bool bValue) override;

  /**
   * @brief Get current value of pin in Input mode. If Output, last set value should be returned.
   * @return Value of pin.
   */
  virtual bool getValue() override;
  virtual bool setAlternateValue(size_t uiValue) override;
  virtual bool setSpeedValue(size_t uiValue) override;
private:
  IGpioPort *m_pPort   = nullptr;
  uint8      m_uiPinNr = 0;
};

#endif // H_CcGpioPortPin_H_
