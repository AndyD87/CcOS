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
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Declarations for malloc and new
 */
#include "CcBase.h"

CCEXTERNC_BEGIN
/**
 * @brief Malloc method for kernel module
 * @param uiSize: Number of bytes to allocate
 * @return Allocated buffer
 */
extern void* CcMalloc_malloc(size_t uiSize);

/**
 * @brief Free allocated buffer of CcMalloc_malloc.
 * @param pBuffer: Buffer to free
 */
extern void CcMalloc_free(void* pBuffer);

/**
 * @brief Printf on kernel module
 * @param pFormat: Target format to print
 */
extern void CcMalloc_print(const char* pFormat, ...);
CCEXTERNC_END

#ifdef __cplusplus
  class CcString;
  /**
   * @brief Printf on kernel moduleCcMalloc_print
   * @param sPrint: Message to print
   */
  void CcMalloc_print(const CcString& sPrint);
#endif
