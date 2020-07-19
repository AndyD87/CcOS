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
 * @subpage   STM32F407Usb
 *
 * @page      STM32F407Usb
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class STM32F407Usb
 */
#ifndef H_STM32F407Usb_H_
#define H_STM32F407Usb_H_

#include "CcBase.h"
#include "Devices/IUsb.h"
#include <stm32f4xx_hal_hcd.h>
#include <stm32f4xx_hal_pcd.h>

class STM32F407Usb : public IUsb
{
public: //methods
  STM32F407Usb();
  virtual ~STM32F407Usb();

  virtual CcStatus setState(EState eState);
  virtual bool setType(EType eType);
  virtual EType getType();
private:
  EType m_eType;
  union
  {
    HCD_HandleTypeDef oHcd;
    PCD_HandleTypeDef oPcd;
  } m_oData;
};

#endif // H_STM32F407Usb_H_
