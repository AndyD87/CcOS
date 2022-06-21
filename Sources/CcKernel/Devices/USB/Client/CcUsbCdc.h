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
#include "Devices/IIoDevice.h"
#include "IIo.h"
#include "CcBinaryStream.h"

#define UsbCdcDataSize      7

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcUsbCdc : public IIoDevice
{
public:
  /**
   * @brief Create device with handle
   * @param oHandle: Handle to init device
   */
  CcUsbCdc(const CcDeviceUsb& oInterface);
  virtual ~CcUsbCdc();
  
  virtual CcStatus onState(EState eState) override;
  
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;


  void onInterfaceRequest(IUsbDevice::SRequest* pRequest);
  void onInterfaceReceive(IUsbDevice::SRequest* pRequest);
  
  void onDataIn(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest);
  void onDataOut(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest);
  void onControl(IUsbDevice::CConfigDescriptor::CEndpointInfo* pRequest);

private:
  CcStatus onStart();
  CcStatus onStop();

private:
  CcDeviceUsb m_oInterface;
  CcByteArray m_oClassData;
  CcByteArray m_oInputBuffer;

  IUsbDevice* m_pUsbDevice;
  uint8       m_uiCmd = UINT8_MAX;
};
