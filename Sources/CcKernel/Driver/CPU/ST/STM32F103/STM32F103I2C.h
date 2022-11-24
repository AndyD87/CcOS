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
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class STM32F103I2C
 */
#pragma once

#include "CcBase.h"
#include "Devices/II2C.h"
#include "STM32F103.h"

/**
 * @brief Timer device for STM32F103
 */
class STM32F103I2C : public II2C
{
public: //methods
  STM32F103I2C();
  virtual ~STM32F103I2C();

  virtual CcStatus onState(EState eState) override;
  
  virtual II2CClient* createInterface(uint16 uiAddress) override;
  virtual void removeInterface(II2CClient* pInterface) override;
private: //member
  I2C_HandleTypeDef m_hI2C1;
};
