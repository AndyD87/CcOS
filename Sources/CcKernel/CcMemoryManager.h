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
 * @brief     Class CcMemoryManager
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief This class is designed to manage memory allocations on generic devices.
 */
class CcKernelSHARED CcMemoryManager
{
public:
  class CcMemoryItem;
  /**
   * @brief Memory header information of one memory area.
   */
  class CcKernelSHARED CHead
  {
  public:
    CcMemoryItem*   pNext;  //!< Pointer to next memory
    size_t          uiSize; //!< Size of memory
  };

  /**
   * @brief Memory part of memory item.
   *        Size of memory is defined in CHead
   */
  class CcKernelSHARED CcMemoryItem : public CHead
  {
  public:
    unsigned char oBuffer; //!< First byte fo allocated memory
  };

  /**
   * @brief Initialize global memory manager
   * @param uiStartAddress: Start address of memory manager in physical memory
   * @param uiEndAddress:   End address of memory manager in physical memory
   * @param uiGranularity:  Minimum size a buffer can have.
   * @return True if init succeeded.
   */
  static bool init(uintptr uiStartAddress, uintptr uiEndAddress, size_t uiGranularity);

  /**
   * @brief Mark last assigned buffer as kernel space.
   *        This will reset the allocation list for faster searching.
   * @return True if switch to user space succeeded.
   */
  static bool initUserSpace();

  /**
   * @brief Allocate size from memory manager
   * @param uiSize:             Number bytes to allocate
   * @param bForceKernelSpace:  True if kernel space has to be used.
   * @return Allocated memory
   */
  static void* malloc(size_t uiSize, bool bForceKernelSpace = false);

  /**
   * @brief Free memory which was previously allocated by malloc()
   * @param pBuffer: Buffer to free.
   */
  static void free(void* pBuffer);

  /**
   * @brief Test malloc method for unitity tests.
   * @param uiSize: Size of buffer to allocate
   * @return Pointer to allocated memory
   */
  inline static void* TestMalloc(size_t uiSize)
  { return malloc(uiSize); }

  /**
   * @brief Test free method for unitity tests.
   * @param pBuffer: Buffer to free.
   */
  inline static void TestFree(void* pBuffer)
  { free(pBuffer); }

  /**
   * @brief Get number of bytes until next granularity reached.
   * @param uiBase: Size to query
   * @return Size required
   */
  static size_t granularity(size_t uiBase);

  //! @return True if init was done
  inline static bool isInitialized()
  { return s_bMallocInitialized; }
  //! @return Size of buffer available
  inline static size_t getAvailable()
  { return s_uiBufferAvailable; }
  //! @return Number of allocatded memory locations.
  inline static size_t getCount()
  { return s_uiBufferCount; }
  //! @return Full size of ram
  inline static size_t getSize()
  { return s_uiSize; }
  //! @return Size of used buffers.
  inline static size_t getUsed()
  { return s_uiBufferUsed; }

private:
  static uintptr s_uiBufferStart;
  static uintptr s_uiBufferEnd;
  static size_t s_uiSize;
  static size_t s_uiBufferAvailable;
  static size_t s_uiBufferCount;
  static size_t s_uiBufferGranularity;
  static size_t s_uiBufferUsed;
  static bool s_bMallocInitialized;
  static CcMemoryItem* s_pMemoryStart;
  static CcMemoryItem* s_pMemoryUser;
  static CcMemoryItem* s_pMemoryEnd;
};
