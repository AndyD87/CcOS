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
 * @brief     Class ISharedMemory
 *            Abstract Class to define an interface to System own Shared Memory
 */
#pragma once

#include "CcBase.h"
#include "IIo.h"
#include "CcReferenceCount.h"

/**
 * @brief Shared memory interface for different systems
 */
class CcKernelSHARED ISharedMemory : public IIo , public CcReferenceCount
{
public:
  virtual ~ISharedMemory() = default;

  virtual CcStatus open(EOpenFlags eOpenFlags) = 0;
  virtual CcStatus close() = 0;
  virtual CcStatus cancel() = 0;
  virtual CcStatus claim(EOpenFlags eOpenFlags) = 0;
  virtual bool exists() = 0;

  virtual size_t read(void* pBuffer, size_t uSize) = 0;
  virtual size_t write(const void* pBuffer, size_t uSize) = 0;

  /**
   * @brief Get current connected buffer
   * @return If Memory is connected successfully this method will return a non null pointer to buffer.
   */
  virtual void* getBuffer() = 0;

};
