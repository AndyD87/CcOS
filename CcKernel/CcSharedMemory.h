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
 * @brief     Class CcSharedMemory
 */
#pragma once

#include "CcBase.h"
#include "ISharedMemory.h"

/**
 * @brief Shared memory object to exchange data with other processes
 */
class CcKernelSHARED CcSharedMemory : public ISharedMemory
{
public:
  /**
   * @brief Create shared memory by name and define it's size
   * @param sName:  Name of shared memory it must be unique on system.
   * @param uiSize: Size of shared memory in bytes
   */
  CcSharedMemory(const CcString& sName, size_t uiSize = c_uiSize);

  /**
   * @brief Move system interfaces from another object to this.
   * @param oToMove: Object to get interfaces from
   */
  CcSharedMemory(CcSharedMemory&& oToMove)
  { operator=(CCMOVE(oToMove)); }
  virtual ~CcSharedMemory();

  /**
   * @brief Move system interfaces from another object to this.
   * @param oToMove: Object to get interfaces from
   * @return Handle to this
   */
  CcSharedMemory& operator=(CcSharedMemory&& oToMove);

  virtual CcStatus open(EOpenFlags eOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  virtual CcStatus claim(EOpenFlags eOpenFlags) override;
  virtual bool exists() override;

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;

  //! @return Get defined default size for shared memories.
  inline static size_t getDefaultSize()
  { return c_uiSize; }

  virtual void* getBuffer() override;

private:
  //! Disable copy!
  CcSharedMemory(const CcSharedMemory& oToCopy) = delete;
  CcSharedMemory& operator=(const CcSharedMemory& oToCopy) = delete;

  ISharedMemory* m_pSystem = nullptr;
  static const size_t c_uiSize = 1024; //<! Default size is set to 1024byte
};
