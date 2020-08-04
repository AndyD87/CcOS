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
 * @page      STM32F407
 * @subpage   STM32F407SystemGpioPin
 *
 * @page      STM32F407SystemGpioPin
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407SystemGpioPin
 */
#ifndef H_STM32F407SystemGpioPin_H_
#define H_STM32F407SystemGpioPin_H_

#include "CcBase.h"
#include "STM32F407.h"
#include "Devices/IGpioPin.h"

class STM32F407SystemGpioPinPrivate;

class STM32F407SystemGpioPin : public IGpioPin
{
public: //methods
  STM32F407SystemGpioPin(void* pPort, uint8 uiPinNr);
  virtual ~STM32F407SystemGpioPin();

  virtual bool setDirection( EDirection eDirection, size_t uiValue = 0) override;
  virtual EDirection getDirection() override;
  virtual void setValue(bool bValue) override;
  virtual bool getValue() override;
  virtual bool toggle() override;
  virtual bool setSpeedValue(size_t uiValue) override;

  void reconfigure();
private: // Member
  GPIO_TypeDef* m_pPort;
  uint32 m_uiPinNr;
  GPIO_InitTypeDef m_oGpioInitStruct;
};

#endif // H_STM32F407SystemGpioPin_H_
