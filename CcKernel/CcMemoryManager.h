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
 * @page      Types
 * @subpage   CcMemoryManager
 *
 * @page      CcMemoryManager
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMemoryManager
 **/
#ifndef _CcMemoryManager_H_
#define _CcMemoryManager_H_

#include "CcKernelBase.h"
#include <list>

class CcKernelSHARED CcMemoryManager
{
public:
  class CcKernelSHARED CcMemoryItem
  {
  public:
    typedef struct
    {
      CcMemoryItem*   pNext;
      size_t          uiSize;
    } SHead;
    SHead oHead;
    unsigned char oBuffer;
  };
  static bool init(uintptr uiStartAddress, uintptr uiEndAddress, size_t uiGranularity);


  static CcMemoryItem* getOrCreateSlot(size_t uiSize);
  static void removeSlot(void* pBuffer);
  static size_t granularity(size_t uiBase);

  inline static bool isInitialized()
    { return s_bMallocInitialized; }
  inline static size_t getAvailable()
    { return s_uiBufferAvailable; }
  inline static size_t getCount()
    { return s_uiBufferCount; }
  inline static size_t getSize()
    { return s_uiSize; }
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
  static CcMemoryItem* s_pMemoryEnd;

};

#endif /* _CcMemoryManager_H_ */
