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
 * @brief     Class CcByteArray
 **/
#pragma once

#include "CcBase.h"

/**
 * @brief Dynamic Storage for char's
 */
class CcKernelSHARED CcBufferInfo
{
public:
  CcBufferInfo() = default;

  /**
   * @brief Constructor wich can append a 0 terminated char array
   * @param pToAppend: pointer to constant /0 terminiated char array
   */
  CcBufferInfo(char* pBuffer, size_t uiSize) :
    m_pBuffer(pBuffer),
    m_uiSize(uiSize)
  {}

  char* getBuffer()
  { return m_pBuffer; }
  
  size_t getSize()
  { return m_uiSize; }

private:
  char*   m_pBuffer;
  size_t  m_uiSize;
};
