/**
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
 * @page      Linux
 * @subpage   CcLinuxLed
 *
 * @page      CcLinuxLed
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxLed
 */

#ifndef _CcLinuxLed_H_
#define _CcLinuxLed_H_

#include "CcBase.h"
#include "CcString.h"
#include "Devices/ILed.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcLinuxLed : public ILed {
public:
  /**
   * @brief Constructor
   */
  CcLinuxLed(const CcString& Path);

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxLed();

  virtual CcStatus setMaxBirghtness(uint16 brightness) override;

  virtual CcStatus setToggleTime(uint16 onTime, uint16 offTime) override;

  virtual CcStatus on(uint16 brightness = 0) override;

  virtual CcStatus off() override;

  virtual CcStatus toggle() override;

  virtual bool IsOn() override;
private:
  CcString m_Path;
  uint16 m_MaxBrightness;
};

#endif /* _CcLinuxLed_H_ */
