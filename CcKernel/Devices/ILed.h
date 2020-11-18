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
 * @subpage   ILed
 *
 * @page      ILed
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class ILed
 */

#ifndef H_ILed_H_
#define H_ILed_H_

#include "CcBase.h"
#include "IDevice.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED ILed : public IDevice
{
public:

  virtual ~ILed() = default;

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   */
  virtual CcStatus setMaxBirghtness(uint16 uiBrightness) = 0;

  virtual CcStatus setToggleTime(uint16 uiOnTime, uint16 uiOffTime)
    { CCUNUSED(uiOnTime); CCUNUSED(uiOffTime); return EStatus::NotSupported; }

  virtual CcStatus on(uint16 uiBrightness = 255) = 0;

  virtual CcStatus off() = 0;

  virtual CcStatus toggle() = 0;

  virtual bool IsOn() = 0;

  virtual bool IsOff()
    { return !IsOn();}
};

#endif // H_ILed_H_
