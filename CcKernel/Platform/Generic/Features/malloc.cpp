/*
 * malloc.cpp
 *
 *  Created on: 10.07.2019
 *      Author: Dirmeier
 */

#include "Platform/Generic/Features/malloc.h"
#include "CcKernel.h"

#define MEMORY_GRANULARITY 16
extern char __bss_end__;

#define CcMemoryManager_castToSize(VAR) static_cast<size_t>(CCVOIDPTRCAST(char*,VAR) - static_cast<char*>(0))
#define CcMemoryManager_castToVoid(VAR) (CCVOIDPTRCAST(void*,static_cast<char*>(0) + VAR))

class CcMemoryItem
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

class CcMemoryManager
{
public:
  static void Init();

  static CcMemoryItem* getOrCreateSlot(size_t uiSize);
  static void removeSlot(void* pBuffer);

public:
  static void* s_pBufferStart;
  static size_t s_uiBufferAvailable;
  static size_t s_uiBufferCount;
  static size_t s_uiBufferUsed;
  static size_t s_uiBufferEnd;
  static bool s_bMallocInitialized;
  static CcMemoryItem* s_pMemoryStart;
  static CcMemoryItem* s_pMemoryEnd;

};

void*  CcMemoryManager::s_pBufferStart        = &__bss_end__;
size_t CcMemoryManager::s_uiBufferAvailable   = 102400;
size_t CcMemoryManager::s_uiBufferCount       = 0;
size_t CcMemoryManager::s_uiBufferUsed        = 0;
size_t CcMemoryManager::s_uiBufferEnd         = 0;
bool   CcMemoryManager::s_bMallocInitialized  = false;
CcMemoryItem* CcMemoryManager::s_pMemoryStart;
CcMemoryItem* CcMemoryManager::s_pMemoryEnd;

void CcMemoryManager::Init()
{
  char* pBufferStart = static_cast<char*>(s_pBufferStart);
  if(CcMemoryManager_castToSize(s_pBufferStart) % MEMORY_GRANULARITY)
  {
    pBufferStart += MEMORY_GRANULARITY - (CcMemoryManager_castToSize(s_pBufferStart) % MEMORY_GRANULARITY);
    s_pBufferStart = pBufferStart;
  }
  s_uiBufferEnd = CcMemoryManager_castToSize(s_pMemoryStart) + s_uiBufferAvailable;
  s_pMemoryStart  = static_cast<CcMemoryItem*>(s_pBufferStart);
  s_pMemoryEnd    = s_pMemoryStart;
  s_pMemoryStart->oBuffer         = 0;
  s_pMemoryStart->oHead.pNext     = nullptr;
  s_pMemoryStart->oHead.uiSize    = 0;
  s_bMallocInitialized = true;
}

CcMemoryItem* CcMemoryManager::getOrCreateSlot(size_t uiSize)
{
  CcMemoryItem* pSlot = nullptr;
  size_t uiSizeRequired = sizeof(CcMemoryItem::SHead) + uiSize;
  if(uiSizeRequired%MEMORY_GRANULARITY)
  {
    uiSizeRequired = uiSizeRequired + (MEMORY_GRANULARITY - (uiSizeRequired %MEMORY_GRANULARITY));
  }
  CcMemoryItem* pMemoryItem = s_pMemoryStart;
  while(pMemoryItem != nullptr)
  {
    size_t uiCurrentOffset = CcMemoryManager_castToSize(pMemoryItem) + pMemoryItem->oHead.uiSize;
    if(pMemoryItem->oHead.pNext)
    {
      if(uiCurrentOffset + uiSizeRequired < CcMemoryManager_castToSize(pMemoryItem->oHead.pNext))
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
      size_t uiNewOffset = uiCurrentOffset + uiSizeRequired;
      if(uiNewOffset < s_uiBufferEnd)
      {
        pSlot = static_cast<CcMemoryItem*>(CcMemoryManager_castToVoid(uiCurrentOffset));
        s_pMemoryEnd = pSlot;
        break;
      }
      else
      {
        CcKernel::message(EMessage::Error, "FATAL_ERROR Buffer Full");
        break;
      }
    }
    pMemoryItem = pMemoryItem->oHead.pNext;
  }
  if(pSlot)
  {
    CcMemoryManager::s_uiBufferCount++;
    CcMemoryManager::s_uiBufferUsed+=uiSizeRequired;
  }
  return pSlot;
}

void CcMemoryManager::removeSlot(void* pBuffer)
{
  CcMemoryItem* pMemoryItemPrv = s_pMemoryStart;
  CcMemoryItem* pMemoryItem = s_pMemoryStart->oHead.pNext;
  while(pMemoryItem != nullptr &&
        ( &pMemoryItem->oBuffer                               > pBuffer ||
          (&pMemoryItem->oBuffer + pMemoryItem->oHead.uiSize) < pBuffer )
       )
  {
    pMemoryItemPrv = pMemoryItem;
    pMemoryItem = pMemoryItem->oHead.pNext;
  }
  if(pMemoryItemPrv && pMemoryItem)
  {
    CcMemoryManager::s_uiBufferCount--;
    CcMemoryManager::s_uiBufferUsed-=pMemoryItem->oHead.uiSize;

    pMemoryItemPrv->oHead.pNext = pMemoryItem->oHead.pNext;
    if(pMemoryItemPrv->oHead.pNext == nullptr)
    {
      s_pMemoryEnd = pMemoryItemPrv;
    }
  }
}

CCEXTERNC void* malloc(size_t uiSize)
{
  __malloc_lock(nullptr);
  void* pBuffer = nullptr;
  if(!CcMemoryManager::s_bMallocInitialized)
  {
    CcMemoryManager::Init();
  }
  if(uiSize > 0 && CcMemoryManager::s_bMallocInitialized)
  {
    CcMemoryItem* pObject = CcMemoryManager::getOrCreateSlot(uiSize);
    if(pObject != nullptr)
    {
      pBuffer = &pObject->oBuffer;
    }
  }
  __malloc_unlock(nullptr);
  return pBuffer;
}

CCEXTERNC void free(void* pBuffer)
{
  __malloc_lock(nullptr);
  CcMemoryManager::removeSlot(pBuffer);
  __malloc_unlock(nullptr);
}

void* operator new(size_t uiSize)
{
  return malloc(uiSize);
}

void operator delete(void* pBuffer)
{
  free(pBuffer);
}

void* operator new[](size_t uiSize)
{
  return malloc(uiSize);
}

void operator delete[](void* pBuffer)
{
  free(pBuffer);
}
