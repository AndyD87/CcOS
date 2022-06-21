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
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtations for malloc and new
 */

#include "Platform/Generic/Features/CcOS_malloc.h"
#include "CcKernel.h"
#include "CcMemoryManager.h"

/**
 * @brief Create memory from free memory pool
 * @param uiSize: Number of bytes to allocate
 * @return Allocated buffer or nullptr
 */
CCEXTERNC void* malloc(size_t uiSize)
{
   __malloc_lock(nullptr);
  void* pBuffer = nullptr;
  pBuffer = CcMemoryManager::malloc(uiSize);
  __malloc_unlock(nullptr);
  return pBuffer;
}

/**
 * @brief Free previously with malloc allocated buffer.
 * @param pBuffer: Buffer to remove
 */
CCEXTERNC void free(void* pBuffer)
{
  __malloc_lock(nullptr);
  CcMemoryManager::free(pBuffer);
  __malloc_unlock(nullptr);
}

#ifndef CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_MALLOC_ONLY

#endif // CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_MALLOC_ONLY
