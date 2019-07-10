/*
 * malloc.cpp
 *
 *  Created on: 10.07.2019
 *      Author: Dirmeier
 */
#include "Platform/Generic/Features/malloc.h"

class CcMemoryItem
{
public:
  struct
  {
    CcMemoryItem*   pNext;
    size_t          uiSize;
  } oHead;
  unsigned char oBuffer;
};

bool Init_done();

extern char __bss_end__;
static void* __heap_start = &__bss_end__;
static char* __heap_end   = &__bss_end__ + 102400;
static bool g_bMallocInitialized = false;
static CcMemoryItem* pMemoryStart;
static CcMemoryItem* pMemoryEnd;

bool Init_done()
{
  pMemoryStart = static_cast<CcMemoryItem*>(__heap_start);
  pMemoryEnd   = static_cast<CcMemoryItem*>(__heap_start);
  pMemoryStart->oBuffer         = 0;
  pMemoryStart->oHead.pNext     = nullptr;
  pMemoryStart->oHead.uiSize    = 0;
  return true;
}

CCEXTERNC void* malloc(size_t uiSize)
{
  __malloc_lock(nullptr);
  void* pBuffer = nullptr;
  if(!g_bMallocInitialized)
  {
    g_bMallocInitialized = Init_done();
  }
  if(uiSize > 0 && g_bMallocInitialized)
  {
    pBuffer = (&pMemoryEnd->oBuffer)+pMemoryEnd->oHead.uiSize;
    if(pBuffer >= __heap_end)
    {
      pBuffer = nullptr;
    }
    else
    {
      CcMemoryItem* pObject = static_cast<CcMemoryItem*>(pBuffer);
      pObject->oHead.pNext = nullptr;
      pObject->oHead.uiSize = uiSize;
      pMemoryEnd->oHead.pNext = pObject;
      pBuffer = &pObject->oBuffer;
      pMemoryEnd = pObject;
    }
  }
  __malloc_unlock(nullptr);
  return pBuffer;
}

CCEXTERNC void free(void* pBuffer)
{
  __malloc_lock(nullptr);
  CcMemoryItem* pMemoryItemPrv = pMemoryStart;
  CcMemoryItem* pMemoryItem = pMemoryStart->oHead.pNext;
  while(pMemoryItem != nullptr &&
        &pMemoryItem->oBuffer <= pBuffer &&
        (&pMemoryItem->oBuffer + pMemoryItem->oHead.uiSize) > pBuffer)
  {
    pMemoryItemPrv = pMemoryItem;
    pMemoryItem = pMemoryItem->oHead.pNext;
  }
  if(pMemoryItemPrv && pMemoryItem)
  {
    pMemoryItemPrv->oHead.pNext = pMemoryItem->oHead.pNext;
    if(pMemoryItemPrv->oHead.pNext == nullptr)
    {
      pMemoryEnd = pMemoryItemPrv;
    }
  }
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
