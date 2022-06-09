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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class STM32F407UsbDevice
 */
#pragma once

#include "CcBase.h"
#include "Devices/IUsbDevice.h"
#include <stm32f4xx_ll_usb.h>
#include <stm32f4xx_hal_pcd.h>

/**
 * @brief First test of an USB Implementation on STM32F4
 */
class STM32F407UsbDevice : public IUsbDevice
{
public: //methods
  STM32F407UsbDevice();
  virtual ~STM32F407UsbDevice();

  virtual CcStatus onState(EState eState) override;
  virtual CcStatus loadDeviceDescriptor(const CDeviceDescriptor& oDescriptor) override;

public:
  void ISR();

private:
  PCD_HandleTypeDef     m_oPcdHandle;
  USB_OTG_GlobalTypeDef m_oGlobalDef;
  USB_OTG_CfgTypeDef    m_oConfigDef;
};
