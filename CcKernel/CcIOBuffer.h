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
 * @subpage   CcIOBuffer
 *
 * @page      CcIOBuffer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIOBuffer
 */

#ifndef CcIOBuffer_H_
#define CcIOBuffer_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "CcByteArray.h"

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED CcIOBuffer : public CcIODevice 
{
public:

  CcIOBuffer();
  virtual ~CcIOBuffer();
  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param buffer: Buffer to load data to.
   * @param size: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  size_t read(char* pBuffer, size_t uSize);

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  size_t write( const char* pBuffer, size_t uSize);

  /**
   * @brief Open Device in a specific mode.
   *        For more informations lock at: @ref EOpenFlags
   * @return true if Device was opened successfully.
   */
  CcStatus open(EOpenFlags);

  /**
   * @brief Close the connection to device.
   * @return true if Connection was successfully closed.
   */
  CcStatus close();

  /**
   * @brief Cancel Current Operation.
   *        It can optionally be Implemented from inheriting Device.
   *        Look at device definintion it it supports canceling.
   * @return true if Opperation was aborted successfully.
   */
  CcStatus cancel();

  /**
   * @brief Communication to Device with it's IO API if supported.
   * @param cmd: IO-Command as 32bit value.
   * @param argument: pointer to argument-data to pass to the device.
   * @return true if operation was succeeded.
   */
  CcStatus ioControl(uint32 cmd, const void *argument)
  {
    CCUNUSED(cmd);
    CCUNUSED(argument);
    return false;
  }

private:
  size_t m_uiCurrentReadPos = 0;
  CcByteArray m_oBuffer;
};

#endif /* CcIOBuffer_H_ */
