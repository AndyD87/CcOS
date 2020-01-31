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
 * @page      Driver
 * @subpage   WinUSBHid
 *
 * @page      WinUSBHid
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Create Access to an USB-Hid-Device through Windows API
 */
#pragma once

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "Devices/IUsbHid.h"
CCEXTERNC_BEGIN
#include <basetyps.h>
#include <hidsdi.h>
#include <basetyps.h>
#include <windef.h>
#include <wtypesbase.h>
#include <wincrypt.h>
#include <shlobj.h>
#include <setupapi.h>
CCEXTERNC_END

#define HIDDEVICE_DEFAULT_STRING_SIZE  32 //!< Default Stringsize for HID

/**
 * @brief Class for communication with a USB-HIDevice
 */
class WinUSBHid : private IUsbHid
{
public:
  /**
   * @brief Constructor
   */
  WinUSBHid();
  /**
   * @brief Destructor
   */
  virtual ~WinUSBHid();

  bool connect() override;

  /**
   *  @brief Get Device-Settings from Preparsed Data
   */
  void GetDeviceCapabilities();

  /**
   * @brief Write Buffer to USBDevice.
   *    Implementation of abstract function form IIo
   * @param[out] cBuffer: Buffer with containing data for writing to device
   * @param[in] iLength: Size of Buffer
   */
  size_t write(const void* cBuffer, size_t iLength) override;

  /**
   * @brief Read Buffer from Device
   *    Implementation of abstract function form IIo
   * @param[in] cBuffer: Buffer get filled with data of device
   * @param[in] iLength: Max read size to Buffer
   */
  size_t read(void* cBuffer, size_t iLength) override;

private:
  HANDLE      m_DeviceHandle; //!< Handle to connected HID-Device
  HANDLE      m_WriteHandle;  //!< Write-Handle to write to device
  HANDLE      m_ReadHandle;   //!< Read--Handle to read from device
  GUID        m_HidGuid;      //!< GUID for Comunication with HID-Devices
};
