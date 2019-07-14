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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcMemoryManager
 */
#include "CcMemoryManager.h"
#include "CcKernel.h"

#define CcMemoryManager_castToVoid(VAR) reinterpret_cast<void*>(VAR)
#define CcMemoryManager_castToUint(VAR) reinterpret_cast<uintptr>(CcMemoryManager_castToVoid(VAR))

uintptr CcMemoryManager::s_uiBufferStart       = 0;
uintptr CcMemoryManager::s_uiBufferEnd        = 0;
size_t CcMemoryManager::s_uiSize              = 0;
size_t CcMemoryManager::s_uiBufferAvailable   = 0;
size_t CcMemoryManager::s_uiBufferCount       = 0;
size_t CcMemoryManager::s_uiBufferGranularity = 0;
size_t CcMemoryManager::s_uiBufferUsed        = 0;
bool   CcMemoryManager::s_bMallocInitialized  = false;
CcMemoryManager::CcMemoryItem* CcMemoryManager::s_pMemoryStart;
CcMemoryManager::CcMemoryItem* CcMemoryManager::s_pMemoryEnd;

bool CcMemoryManager::init(void* pBaseAddress, size_t uiSize, size_t uiGranularity)
{
  uintptr uiBufferStart = CcMemoryManager_castToUint(pBaseAddress);
  if(CcMemoryManager_castToUint(uiBufferStart) % uiGranularity)
  {
    uiBufferStart += uiGranularity - (uiBufferStart % uiGranularity);
  }
  s_uiBufferStart  = uiBufferStart;
  s_uiBufferGranularity = uiGranularity;
  s_uiSize = uiSize;
  s_uiBufferEnd   = CcMemoryManager_castToUint(s_uiBufferStart) + s_uiSize;
  s_pMemoryStart  = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(s_uiBufferStart));
  s_pMemoryEnd    = s_pMemoryStart;
  s_pMemoryStart->oBuffer         = 0;
  s_pMemoryStart->oHead.pNext     = nullptr;
  s_pMemoryStart->oHead.uiSize    = granularity(0);
  s_uiBufferAvailable = s_uiSize - s_pMemoryStart->oHead.uiSize;
  return s_bMallocInitialized = true;
}

CcMemoryManager::CcMemoryItem* CcMemoryManager::getOrCreateSlot(size_t uiSize)
{
  CcMemoryItem* pSlot = nullptr;
  size_t uiSizeRequired = granularity(uiSize);
  CcMemoryItem* pMemoryItem = s_pMemoryStart;
  while(pMemoryItem != nullptr)
  {
    uintptr uiCurrentOffset = CcMemoryManager_castToUint(pMemoryItem) + pMemoryItem->oHead.uiSize;
    if(pMemoryItem->oHead.pNext)
    {
      if(uiCurrentOffset + uiSizeRequired < CcMemoryManager_castToUint(pMemoryItem->oHead.pNext))
      {
        pSlot = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(uiCurrentOffset));
        pSlot->oHead.pNext  = pMemoryItem->oHead.pNext;
        pSlot->oHead.uiSize = uiSizeRequired;
        pMemoryItem->oHead.pNext = pSlot;
        break;
      }
    }
    else
    {
      uintptr uiNewOffset = uiCurrentOffset + uiSizeRequired;
      if(uiNewOffset < s_uiBufferEnd)
      {
        pSlot = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(uiCurrentOffset));
        pSlot->oHead.pNext  = pMemoryItem->oHead.pNext;
        pSlot->oHead.uiSize = uiSizeRequired;
        pMemoryItem->oHead.pNext = pSlot;
        s_pMemoryEnd = pSlot;
        break;
      }
      else
      {
        CcKernel::message(EMessage::Warning);
      }
    }
    pMemoryItem = pMemoryItem->oHead.pNext;
  }
  if(pSlot)
  {
    CcMemoryManager::s_uiBufferCount++;
    CcMemoryManager::s_uiBufferUsed     +=uiSizeRequired;
    CcMemoryManager::s_uiBufferAvailable-=uiSizeRequired;
  }
  return pSlot;
}

void CcMemoryManager::removeSlot(void* pBuffer)
{
  CcMemoryItem* pMemoryItemPrv = s_pMemoryStart;
  CcMemoryItem* pMemoryItem = s_pMemoryStart->oHead.pNext;
  uintptr uiPos = CcMemoryManager_castToUint(pBuffer);
  while(pMemoryItem != nullptr &&
        (  CcMemoryManager_castToUint(pMemoryItem)                              >= uiPos ||
          (CcMemoryManager_castToUint(pMemoryItem) + pMemoryItem->oHead.uiSize) <  uiPos )
       )
  {
    pMemoryItemPrv = pMemoryItem;
    pMemoryItem = pMemoryItem->oHead.pNext;
  }
  if(pMemoryItem)
  {
    CcMemoryManager::s_uiBufferCount--;
    CcMemoryManager::s_uiBufferUsed       -= pMemoryItem->oHead.uiSize;
    CcMemoryManager::s_uiBufferAvailable  += pMemoryItem->oHead.uiSize;

    pMemoryItemPrv->oHead.pNext = pMemoryItem->oHead.pNext;
    if(pMemoryItem->oHead.pNext == nullptr)
    {
      s_pMemoryEnd = pMemoryItemPrv;
    }
  }
  else
  {
    CcKernel::message(EMessage::Warning);
  }
}

size_t CcMemoryManager::granularity(size_t uiBase)
{
  size_t uiSizeRequired = sizeof(CcMemoryItem::SHead) + uiBase;
  if(uiSizeRequired%s_uiBufferGranularity)
  {
    uiSizeRequired = uiSizeRequired + (s_uiBufferGranularity - (uiSizeRequired %s_uiBufferGranularity));
  }
  return uiSizeRequired;
}
