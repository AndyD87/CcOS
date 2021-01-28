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
 * @brief     Implemtation of class CcMemoryManager
 */
#include "CcMemoryManager.h"
#include "CcMemoryMonitor.h"
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
CcMemoryManager::CcMemoryItem* CcMemoryManager::s_pMemoryUser;
CcMemoryManager::CcMemoryItem* CcMemoryManager::s_pMemoryEnd;

bool CcMemoryManager::init(uintptr uiStartAddress, uintptr uiEndAddress, size_t uiGranularity)
{
  if(CcMemoryManager_castToUint(uiStartAddress) % uiGranularity)
  {
    uiStartAddress += uiGranularity - (uiStartAddress % uiGranularity);
  }
  s_uiBufferStart  = uiStartAddress;
  s_uiBufferGranularity = uiGranularity;
  s_uiSize          = uiEndAddress - uiStartAddress;
  s_uiBufferEnd   = CcMemoryManager_castToUint(s_uiBufferStart) + s_uiSize;
  s_pMemoryStart  = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(s_uiBufferStart));
  s_pMemoryUser   = nullptr;
  s_pMemoryEnd    = s_pMemoryStart;
  s_pMemoryStart->pNext     = nullptr;
  s_pMemoryStart->uiSize    = sizeof(CcMemoryItem);
  s_uiBufferAvailable = s_uiSize - s_pMemoryStart->uiSize;
  return s_bMallocInitialized = true;
}

bool CcMemoryManager::initUserSpace()
{
  if(s_bMallocInitialized)
  {
    uintptr uiCurrentOffset = CcMemoryManager_castToUint(s_pMemoryEnd) + s_pMemoryEnd->uiSize;
    s_pMemoryUser = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(uiCurrentOffset));
    s_pMemoryUser->pNext = nullptr;
    s_pMemoryUser->uiSize   = sizeof(CcMemoryItem);
    s_pMemoryEnd = s_pMemoryUser;
    s_uiBufferAvailable = s_uiSize - s_pMemoryUser->uiSize;
    return true;
  }
  return false;
}

void* CcMemoryManager::malloc(size_t uiSize, bool bForceKernelSpace)
{
  void* pBuffer = nullptr;
  CcMemoryItem* pSlot = nullptr;
  size_t uiSizeRequired = granularity(uiSize);
  CcMemoryItem* pMemoryItem = s_pMemoryUser;
  if( bForceKernelSpace == true ||
      s_pMemoryUser == nullptr)
  {
    pMemoryItem = s_pMemoryStart;
  }
  while(pMemoryItem != nullptr)
  {
    uintptr uiCurrentOffset = CcMemoryManager_castToUint(pMemoryItem) + pMemoryItem->uiSize;
    if(pMemoryItem->pNext)
    {
      if(uiCurrentOffset + uiSizeRequired <= CcMemoryManager_castToUint(pMemoryItem->pNext))
      {
        pSlot = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(uiCurrentOffset));
        pSlot->pNext  = pMemoryItem->pNext;
        pSlot->uiSize = uiSizeRequired;
        pMemoryItem->pNext = pSlot;
        break;
      }
    }
    else
    {
      uintptr uiNewOffset = uiCurrentOffset + uiSizeRequired;
      if(uiNewOffset < s_uiBufferEnd)
      {
        pSlot = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(uiCurrentOffset));
        pSlot->pNext  = pMemoryItem->pNext;
        pSlot->uiSize = uiSizeRequired;
        pMemoryItem->pNext = pSlot;
        s_pMemoryEnd = pSlot;
        if(uiCurrentOffset < s_uiBufferStart)
          CcKernel::message(EMessage::Warning);
        break;
      }
      else
      {
        if( uiCurrentOffset < s_uiBufferStart ||
            uiCurrentOffset > s_uiBufferEnd)
        {
          // Heap corruption?
          CcKernel::message(EMessage::Error);
        }
        else
        {
          CcKernel::message(EMessage::Warning);
        }
      }
    }
    pMemoryItem = pMemoryItem->pNext;
  }
  if(pSlot)
  {
    pBuffer = &pSlot->oBuffer;
    CcMemoryManager::s_uiBufferCount++;
    CcMemoryManager::s_uiBufferUsed     +=uiSizeRequired;
    CcMemoryManager::s_uiBufferAvailable-=uiSizeRequired;
  }
  return pBuffer;
}

void CcMemoryManager::free(void* pBuffer)
{
  CcMemoryItem* pMemoryItemPrv = nullptr;
  CcMemoryItem* pMemoryItem    = nullptr;
  if( nullptr == s_pMemoryUser ||
      pBuffer < s_pMemoryUser)
  {
    pMemoryItemPrv = s_pMemoryStart;
  }
  else
  {
    pMemoryItemPrv = s_pMemoryUser;
  }
  pMemoryItem    = pMemoryItemPrv->pNext;
  uintptr uiPos = CcMemoryManager_castToUint(pBuffer);
  while(pMemoryItem != nullptr &&
        (  CcMemoryManager_castToUint(pMemoryItem)                              >= uiPos ||
          (CcMemoryManager_castToUint(pMemoryItem) + pMemoryItem->uiSize) <  uiPos )
       )
  {
    pMemoryItemPrv = pMemoryItem;
    pMemoryItem = pMemoryItem->pNext;
  }
  if(pMemoryItem)
  {
    CcMemoryManager::s_uiBufferCount--;
    CcMemoryManager::s_uiBufferUsed       -= pMemoryItem->uiSize;
    CcMemoryManager::s_uiBufferAvailable  += pMemoryItem->uiSize;

    pMemoryItemPrv->pNext = pMemoryItem->pNext;
    if(pMemoryItem->pNext == nullptr)
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
