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
 * @subpage   CcStatic
 *
 * @page      CcStatic
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStatic
 */
#ifndef CcStatic_H_
#define CcStatic_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Static class common methods like memset.
 */
class CcKernelSHARED CcStatic 
{
public:
  /**
   * @brief Read to a fixed buffer from commandline.
   * @param pBuffer: Target buffer to read to.
   * @param uiSize: Size of pBuffer.
   * @return Number of bytes read from console
   */
  static void* memset(void* pBuffer, int iValue, size_t uiSize);

  static void* memcpy(void* pDestination, const void* pSource, size_t uiSize);
};

#endif /* CcStatic_H_ */
