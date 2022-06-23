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
 * @brief     Class STM32Usb
 */
#pragma once

#include "CcBase.h"
#include "Devices/IUsb.h"
#include "Devices/IGpioPort.h"
#include "CcDeviceList.h"
#include "STM32UsbDevice.h"

/**
 * @brief First test of an USB Implementation on STM32F4
 */
class STM32Usb : public IUsb
{
public: //methods
  STM32Usb();
  virtual ~STM32Usb();

  virtual CcStatus onState(EState eState) override;

  virtual IUsbDevice* createDevice(const IUsbDevice::CDeviceDescriptor& oConfig) override;
  virtual void removeDevice() override;

private:
  EType m_eType;
  union
  {
    IDevice*            pBasicDevice;
    STM32UsbDevice* pDevice;
  } m_pType;
};
