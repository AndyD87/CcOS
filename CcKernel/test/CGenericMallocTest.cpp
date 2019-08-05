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
 * @brief     Implemtation of class CGenericMallocTest
 */
#include "CGenericMallocTest.h"
#include "CcStatic.h"
#include "CcList.h"
#include "CcKernel.h"
#include "CcMemoryManager.h"

// Implementations to get malloc working:
CCEXTERNC_BEGIN
void __malloc_lock( struct _reent *){}
void __malloc_unlock( struct _reent * ){}
char pTestBuffer[16394];
CCEXTERNC_END

#define malloc TestMalloc
#define free  TestFree
#define CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_MALLOC_ONLY
#define __bss_end__ pTestBuffer[0]
#define __data_end__ pTestBuffer[16394]

#include "Platform/Generic/Features/CcOS_malloc.h"
#include "Platform/Generic/Features/CcOS_malloc.cpp"

CGenericMallocTest::CGenericMallocTest() :
  CcTest("CGenericMallocTest")
{
  appendTestMethod("Test basic allocation tests", &CGenericMallocTest::testBasic);
  appendTestMethod("Test allocate more than available", &CGenericMallocTest::testOversize);
  appendTestMethod("Check malloc,free,malloc in same address", &CGenericMallocTest::testSameAddress);
  appendTestMethod("Check little allocations", &CGenericMallocTest::testLittleAllocations);
  appendTestMethod("Check little allocation and invalidate", &CGenericMallocTest::testLittleAllocationInvalidate);
  appendTestMethod("Lock virtual kernel space", &CGenericMallocTest::testKernelLock);
}

CGenericMallocTest::~CGenericMallocTest()
{
  free(m_pKernelSpace);
}

bool CGenericMallocTest::testBasic()
{
  bool bRet = false;
  char* pData = static_cast<char*>(malloc(100));
  if(pData)
  {
    CcStatic::memset(pData, 0, 100);
    free(pData);
    bRet = true;
  }
  return bRet;
}

bool CGenericMallocTest::testOversize()
{
  bool bRet = false;
  size_t uiSize = CcMemoryManager::getAvailable();
  char* pData = static_cast<char*>(malloc(uiSize+1));
  if(pData == nullptr)
  {
    bRet = true;
  }
  else
  {
    free(pData);
  }
  return bRet;
}

bool CGenericMallocTest::testSameAddress()
{
  bool bRet = false;
  char* pData = static_cast<char*>(malloc(100));
  char* pTemp = pData;
  if(pData)
  {
    CcStatic::memset(pData, 0, 100);
    free(pData);
    pData = static_cast<char*>(malloc(100));
    if(pData)
    {
      if(pData == pTemp)
      {
        bRet = true;
      }
      free(pData);
    }
  }
  return bRet;
}

bool CGenericMallocTest::testLittleAllocations()
{
  bool bSuccess = false;
  size_t uiCountBegin = CcMemoryManager::getCount();
  size_t uiBeginAvailable = CcMemoryManager::getAvailable();
  if(uiCountBegin == 0) // Start wich 0 Data
  {
    bSuccess = true;
    size_t uiCurrentCount = 0;
    size_t uiCurrentAvailable = uiBeginAvailable;
    size_t uiCurrentUsed = 0;
    size_t uiAllocSize = 16;
    size_t uiRealSize  = CcMemoryManager::granularity(uiAllocSize);
    size_t uiMaxCount = uiBeginAvailable / uiRealSize;
    CcList<void*> oData;
    for(size_t uiCount = 0; uiCount < uiMaxCount && bSuccess; uiCount++)
    {
      void* pData = malloc(uiAllocSize);
      if(pData)
      {
        oData.append(pData);
        uiCurrentUsed += uiRealSize;
        uiCurrentAvailable -= uiRealSize;
        uiCurrentCount++;
        if(uiCurrentUsed != CcMemoryManager::getUsed())
        {
          bSuccess = false;
        }
        if(uiCurrentAvailable != CcMemoryManager::getAvailable())
        {
          bSuccess = false;
        }
        if(uiCurrentCount != CcMemoryManager::getCount())
        {
          bSuccess = false;
        }
      }
      else
      {
        bSuccess = false;
      }
    }
    while(oData.size() > 0)
    {
      void* pData = oData[0];
      oData.remove(0);
      uiCurrentUsed -= uiRealSize;
      uiCurrentAvailable += uiRealSize;
      uiCurrentCount--;
      free(pData);
      if(uiCurrentUsed != CcMemoryManager::getUsed())
      {
        bSuccess = false;
      }
      if(uiCurrentAvailable != CcMemoryManager::getAvailable())
      {
        bSuccess = false;
      }
      if(uiCurrentCount != CcMemoryManager::getCount())
      {
        bSuccess = false;
      }
    }
  }
  return bSuccess;
}

bool CGenericMallocTest::testLittleAllocationInvalidate()
{
  bool bSuccess = false;
  size_t uiCountBegin = CcMemoryManager::getCount();
  size_t uiBeginAvailable = CcMemoryManager::getAvailable();
  if(uiCountBegin == 0) // Start wich 0 Data
  {
    bSuccess = true;
    size_t uiAllocSize  = 15;
    size_t uiRealSize  = CcMemoryManager::granularity(uiAllocSize);
    size_t uiMaxCount = uiBeginAvailable / uiRealSize;
    CcList<void*> oData;
    for(size_t uiCount = 0; uiCount < uiMaxCount && bSuccess; uiCount++)
    {
      void* pData = malloc(uiAllocSize);
      if(pData)
      {
        oData.append(pData);
      }
      else
      {
        bSuccess = false;
      }
    }
    for(size_t uiCount = 0; uiCount < oData.size() && bSuccess; uiCount++)
    {
      void* pData = oData[uiCount];
      oData.remove(uiCount);
      free(pData);
    }
    void* pInvalidData = malloc(uiAllocSize);
    if(pInvalidData == nullptr)
    {
      bSuccess = false;
    }
    else
    {
      oData.append(pInvalidData);
    }
    pInvalidData = malloc(uiAllocSize*3);
    if(pInvalidData != nullptr)
    {
      oData.append(pInvalidData);
      bSuccess = false;
    }
    while(oData.size() > 0)
    {
      void* pData = oData[0];
      oData.remove(0);
      free(pData);
    }
    size_t uiCount = CcMemoryManager::getCount();
    if(uiCount != 0)
    {
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool CGenericMallocTest::testKernelLock()
{
  bool bRet = true;
  m_pKernelSpace = malloc(1);
  if (m_pKernelSpace)
  {
    bRet = CcMemoryManager::initUserSpace();
  }
  return bRet;
}
