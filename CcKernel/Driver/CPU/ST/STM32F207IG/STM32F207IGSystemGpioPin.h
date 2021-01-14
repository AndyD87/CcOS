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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F207IGSystemGpioPin
 */
#ifndef H_STM32F207IGSystemGpioPin_H_
#define H_STM32F207IGSystemGpioPin_H_

#include "CcBase.h"
#include "Devices/IGpioPin.h"

class STM32F207IGSystemGpioPinPrivate;

class STM32F207IGSystemGpioPin : public IGpioPin
{
public: //methods
  STM32F207IGSystemGpioPin(void* pPort, uint8 uiPinNr);
  virtual ~STM32F207IGSystemGpioPin();

  virtual bool setDirection( EDirection eDirection, size_t uiValue = 0) override;
  virtual EDirection getDirection() override;
  virtual void setValue(bool bValue) override;
  virtual bool getValue() override;
  virtual bool toggle() override;
  virtual bool setSpeedValue(size_t uiValue) override;
  void reconfigure();
private:
private: //member
  STM32F207IGSystemGpioPinPrivate* m_pPrivate;
};

#endif // H_STM32F207IGSystemGpioPin_H_
