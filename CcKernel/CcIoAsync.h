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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIoAsync
 */
#pragma once

#include "CcBase.h"
#include "CcObject.h"
#include "IIo.h"

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED CcIoAsync : private CcObject
{
public:
  /**
   * @brief Create an async interface for specific io device
   * @param pIoDevice: Target io device to make async
   */
  CcIoAsync(IIo *pIoDevice);

  /**
   * @brief Destructor
   */
  virtual ~CcIoAsync();

  /**
   * @brief Async read request for io device
   * @param pBuffer:  Target buffer to write to
   * @param uSize:    Size of pBuffer to write
   * @return True if read is executing
   */
  virtual bool read(char* pBuffer, size_t uSize);

  /**
   * @brief Async write request for io device
   * @param pBuffer:  Target buffer to read from
   * @param uSize:    Size of pBuffer to read
   * @return True if write is executing
   */
  virtual bool write(const char* pBuffer, size_t uSize);

  /**
   * @brief Check if given size of bytes is read
   * @param size: Size of byte to query for
   * @return True if size is reached
   */
  virtual bool onReadDone(size_t size);

  /**
   * @brief Check if given size of bytes is written
   * @param size: Size of byte to query for
   * @return True if size is written
   */
  virtual bool onWriteDone(size_t size);

private:
  IIo *m_Device;
};
