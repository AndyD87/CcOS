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
 * @subpage   CcGpioPortSimulation
 *
 * @page      CcGpioPortSimulation
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGpioPortSimulation
 */

#ifndef H_CcGpioPortSimulation_H_
#define H_CcGpioPortSimulation_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "Devices/IGpioPort.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcGpioPortSimulation : public IGpioPort
{
public:
  /**
   * @brief Destructor
   */
  virtual ~CcGpioPortSimulation() = default;

  virtual uint8 count() const
    { return 32; }

  virtual bool setPinsDirection(size_t uiPinMask, IGpioPin::EDirection eDirection, size_t uiValue = 0) override;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   * @param EDirection: New configuration for pin to set.
   * @return true if Configuration was set successfully.
   */
  virtual bool setDirection(size_t uiPin, IGpioPin::EDirection eDirection) override;

  /**
   * @brief Get current Configuration of Pin
   * @return Current configuration of Pin.
   */
  virtual IGpioPin::EDirection getDirection(size_t uiPin) override;

  virtual void setValue(size_t uiPin, bool bValue) override;
  virtual bool getValue(size_t uiPin) override;
private:
  bool                  m_aValues[32] = {0};
  IGpioPin::EDirection  m_eDirections[32] = {IGpioPin::EDirection::Input};
};

#endif /* H_CcGpioPortSimulation_H_ */
