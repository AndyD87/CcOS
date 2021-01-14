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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Create Access to an USB-Hid-Device
 */
#pragma once

#include "CcBase.h"
#include "IUsb.h"
#include "IIo.h"

/**
 * @brief Class for communication with a USB-HIDevice
 */
class CcKernelSHARED IUsbHid : public IUsb, public IIo
{
public:
  /**
   * @brief Constructor
   */
  IUsbHid();
  /**
   * @brief Destructor
   */
  virtual ~IUsbHid();

  /**
   * @brief Set VID & PID of Device for connecting to
   * @param iVid:       Vendor-ID of HID-Device
   * @param iPid:       Product-ID of HID-Device
   * @param iUsbUsage:  USB-Page and Usage in Format 0xPPUU,
   *                    default: 0 (parameter is ignored)
   * @return true if connection successfully, false if device not available
   */
  bool setDevice(uint32 vid, uint32 pid, uint32 usage = 0);

  /**
   * @brief Write Buffer to Device
   * @param[out] cBuffer: Buffer with containing data for writing to device
   * @param[in] iLength: Size of Buffer
   */
  virtual size_t write(const void* cBuffer, size_t iLength) override = 0;

  /**
   * @brief Read Buffer from Device
   * @param[in] cBuffer: Buffer get filled with data of device
   * @param[in] iLength: Max read size to Buffer
   */
  virtual size_t read(void* cBuffer, size_t iLength) override = 0;

  /**
   * @brief Readbuffer size for device;
   * @return Returns the Size in Bytes, have to be read from device
   */
  size_t getReportInputSize();

  /**
   * @brief Writebuffer size for device;
   * @return Returns the Size in Bytes, have to be written to device
   */
  size_t getReportOutputSize();


protected:
  /**
   * @brief Storage for all infromations for a USB Hid Device
   */
  class IUsbInfo
  {
  public:
    uint32    vid;   //!< Vendor-ID of HID-Device
    uint32    pid;   //!< Product-ID of HID-Device
    uint32    usage; //!< Usb-Usage+Page for type of HID
    uint32    m_uiReportInputSize; //!< Size of Buffer for Sending Data
    uint32    m_uiReportOutputSize;//!< Size of Buffer for Receiving Data
    CcString  m_sVendorString;     //!< Vendor String read from Device
    CcString  m_sProductString;    //!< Product String read from Device
    CcString  m_sSerialString;     //!< Serial-Number read from Device
  };

  /**
   * @brief start connecting to device previously set.
   * @return true if connection successfully established
   */
  virtual bool connect() = 0;

  IUsbInfo m_Info; //!< Info of connected Device
};
