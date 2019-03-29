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
 * @page      STM32F303VCT6
 * @subpage   STM32F303VCT6SystemGpioPort
 * 
 * @page      STM32F303VCT6SystemGpioPort
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F303VCT6SystemGpioPort
 */
#ifndef _CCLIB_STM32F303VCT6SystemGpioPort_H_
#define _CCLIB_STM32F303VCT6SystemGpioPort_H_

#include "CcBase.h"
#include "Devices/IGpioPort.h"

#define NUMBER_OF_PINS 32

class STM32F303VCT6SystemGpioPortPrivate;

class STM32F303VCT6SystemGpioPort : public IGpioPort
{
public: //methods
  STM32F303VCT6SystemGpioPort(uint8 uiPort);
  virtual ~STM32F303VCT6SystemGpioPort();

  virtual uint8 count() const
    { return NUMBER_OF_PINS;}
  virtual IGpioPin* getPin(uint8 uiNr);

private: //member
  STM32F303VCT6SystemGpioPortPrivate* m_pPrivate;
};

#endif /* _CCLIB_STM32F303VCT6SystemGpioPort_H_ */
