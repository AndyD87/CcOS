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
 * @page      Driver
 * @subpage   CcHdd
 * 
 * @page      CcHdd
 * @par       Language: C++11
 * @brief     Class CcHdd
 */

#ifndef CcHdd_H_
#define CcHdd_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcDevice.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcHdd : public CcDevice{
public:
  /**
   * @brief Constructor
   */
  CcHdd();

  /**
   * @brief Destructor
   */
  virtual ~CcHdd();

  /**
   * @brief Initialize basic settings for General Purpose Input Output
   */
  virtual void setMaxBirghtness(uint16 brightness) = 0;

  virtual void setToggleTime(uint16 onTime, uint16 offTime) = 0;

  virtual void on(uint16 brightness = 0) = 0;

  virtual void off() = 0;

  virtual void toggle(uint16 brightness = 0) = 0;

protected:
  uint16 m_Brightness;
};

#endif /* CcHdd_H_ */
