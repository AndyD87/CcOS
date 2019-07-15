/*
 * malloc.cpp
 *
 *  Created on: 10.07.2019
 *      Author: Dirmeier
 */

#include "Platform/Generic/Features/malloc.h"
#include "CcKernel.h"
#include "CcMemoryManager.h"

#ifndef MEMORY_GRANULARITY
  #define MEMORY_GRANULARITY  16
#endif
#ifndef __bss_end__
  extern uintptr __bss_end__;
#endif
#ifndef __data_end__
  extern uintptr __data_end__;
#endif

#ifdef MEMORY_SIZE
  size_t g_uRamLength = MEMORY_SIZE;
#else
  size_t g_uRamLength = 0;
#endif

CCEXTERNC void* malloc(size_t uiSize)
{
  __malloc_lock(nullptr);
  void* pBuffer = nullptr;
  if(uiSize > 0 &&
    ( CcMemoryManager::isInitialized() ||
      ( CcMemoryManager::init(reinterpret_cast<uintptr>(&__bss_end__),
                              reinterpret_cast<uintptr>(&__data_end__),
                              MEMORY_GRANULARITY))))
  {
    CcMemoryManager::CcMemoryItem* pObject = CcMemoryManager::getOrCreateSlot(uiSize);
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

#ifndef CCOS_MEMORY_MANAGMENT_MALLOC_ONLY

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

#endif // CCOS_MEMORY_MANAGMENT_MALLOC_ONLY
