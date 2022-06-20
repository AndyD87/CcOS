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
 * @par       Language: C++11
 * @brief     Class CcUsbCdc
 */
#pragma once

#include "CcBase.h"
#include "Devices/CcDeviceUsb.h"
#include "IDevice.h"
#include "IIo.h"
#include "CcBinaryStream.h"

#define UsbCdcDataSize      7

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcUsbCdc : public IDevice
{
public:
  /**
   * @brief Create device with handle
   * @param oHandle: Handle to init device
   */
  CcUsbCdc(const CcDeviceUsb& oInterface);
  virtual ~CcUsbCdc();
  
  virtual CcStatus onState(EState eState) override;

  void onInterfaceRequest(IUsbDevice::SRequest* pRequest);
  void onInterfaceReceive(IUsbDevice::SRequest* pRequest);

private:
  CcStatus onStart();

private:
  CcDeviceUsb m_oInterface;
  CcByteArray m_oCtrlInput;
  CcByteArray m_oInOutput;
  CcBinaryStream m_oCtrlStream;
  CcBinaryStream m_oIoStream;

  CcByteArray m_oClassData;
  IUsbDevice* m_pUsbDevice;
  uint8       m_uiCmd = UINT8_MAX;
};
