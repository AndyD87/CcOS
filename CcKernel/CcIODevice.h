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
 * @page      CcKernel
 * @subpage   CcIODevice
 *
 * @page      CcIODevice
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIODevice
 */

#ifndef CCIODEVICE_H_
#define CCIODEVICE_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcDevice.h"

// forward declarations
class CcByteArray;
class CcString;

/**
 * @brief Namespace for EOpenFlags for better readablity
 */
enum class EOpenFlags : uint16
{
  NoFlag    = 0x0000,   //!< Open device with no special settings.
  Read      = 0x0001,   //!< Open Read-Mode of Device.
  Write     = 0x0002,   //!< Open Write-Mode of Device.
  ReadWrite = 0x0003,   //!< Open Read-and-Write-Mode of Device.
  Append    = 0x0004,   //!< Open Appen-Mode of Device.
  Overwrite = 0x0012,   //!< Open Overwrite-Mode of Target Device.
  ShareRead = 0x0100,   //!< Open Read-Mode of Device.
  ShareWrite= 0x0200,   //!< Open Write-Mode of Device.
  Attributes= 0x8000
};

inline EOpenFlags operator|(EOpenFlags leftOp, EOpenFlags rightOp)
{
  return static_cast<EOpenFlags>
    (static_cast<uint16>(leftOp) | static_cast<uint16>(rightOp));
}

inline EOpenFlags operator&(EOpenFlags leftOp, EOpenFlags rightOp)
{
  return static_cast<EOpenFlags>
    (static_cast<uint16>(leftOp) & static_cast<uint16>(rightOp));
}

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED CcIODevice : public CcDevice {
public:
  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param buffer: Buffer to load data to.
   * @param size: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  virtual size_t read(void* buffer, size_t size) = 0;

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  virtual size_t write(const void* buffer, size_t size) = 0;

  /**
   * @brief Open Device in a specific mode.
   *        For more informations lock at: @ref EOpenFlags
   * @return true if Device was opened successfully.
   */
  virtual CcStatus open(EOpenFlags) = 0;

  /**
   * @brief Close the connection to device.
   * @return true if Connection was successfully closed.
   */
  virtual CcStatus close() = 0;

  /**
   * @brief Cancel Current Operation.
   *        It can optionally be Implemented from inheriting Device.
   *        Look at device definintion it it supports canceling.
   * @return true if Opperation was aborted successfully.
   */
  virtual CcStatus cancel() = 0;

  /**
   * @brief Communication to Device with it's IO API if supported.
   * @param cmd: IO-Command as 32bit value.
   * @param argument: pointer to argument-data to pass to the device.
   * @return true if operation was succeeded.
   */
  virtual CcStatus ioControl(uint32 cmd, const void *argument);

  virtual void* getStdFile();
  
  /**
   * @brief Read data from IODevice direct to a ByteArray.
   * @param[in,out] oOutputBuffer: Target preallocated OutputBuffer to write the read data to.
   *                               At the end, buffer will be resized to received data size.
   * @return number of bytes, read from IODevice
   */
  size_t readArray(CcByteArray& oOutputArray, bool bDoResize = true);
  CcByteArray readAll(size_t uiBufSize = 1024);
  CcStatus writeArray(const CcByteArray& oArray);
  CcStatus writeString(const CcString& oArray);
  CcStatus writeLine(const CcString& oArray);
};

#endif /* CCIODEVICE_H_ */
