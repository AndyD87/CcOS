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
 * @page      STM32F407V
 * @subpage   STM32F407VSystemGpioPin
 * 
 * @page      STM32F407VSystemGpioPin
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407VSystemGpioPin
 */
#ifndef _CCLIB_STM32F407VSystemGpioPin_H_
#define _CCLIB_STM32F407VSystemGpioPin_H_

#include "CcBase.h"
#include "Devices/IGpioPin.h"

#define NUMBER_OF_PINS 32

class STM32F407VSystemGpioPinPrivate;

class STM32F407VSystemGpioPin : public IGpioPin
{
public: //methods
  STM32F407VSystemGpioPin(void* pPort, uint8 uiPinNr);
  virtual ~STM32F407VSystemGpioPin();

  virtual bool setDirection( EDirection eDirection) override;
  virtual EDirection getDirection() override;
  virtual void setValue(bool bValue) override;
  virtual bool getValue() override;
  virtual bool toggle() override;
private:
  void reconfigure();
private: //member
  STM32F407VSystemGpioPinPrivate* m_pPrivate;
};

#endif /* _CCLIB_STM32F407VSystemGpioPin_H_ */
