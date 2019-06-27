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
 * @brief     Implemtation of class CcMemoryMonitor
 */
#include "CcMemoryMonitor.h"
#include "CcKernel.h"
#include "IThread.h"
#include <cstdlib>
#include <list>
#include <map>
#include "CcMutex.h"
#include "CcKernel.h"
#include "Devices/ICpu.h"
#include "IIoDevice.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"

static std::list<CcMemoryMonitor::CItem>* g_pMemoryList = nullptr;
static ICpu* g_pCpu = nullptr;
bool g_bMemoryEnabled = false;

#ifdef WINDOWS
#include <Windows.h>
CRITICAL_SECTION g_oCriticalSection;
#else
static CcMutex g_oMutex;
#endif

void CcMemoryMonitor::enable()
{
  g_bMemoryEnabled = true;
  if(g_pMemoryList == nullptr)
  {
    init();
  }
}

void CcMemoryMonitor::disable()
{
  g_bMemoryEnabled = false;
}

bool CcMemoryMonitor::isEnabled()
{
  return g_bMemoryEnabled;
}

void CcMemoryMonitor::init()
{
#ifdef WINDOWS
  InitializeCriticalSection(&g_oCriticalSection);
#endif
  deinit();
  g_pMemoryList = new std::list<CcMemoryMonitor::CItem>;
  g_pCpu = CcKernel::getDevice(EDeviceType::Cpu, 0).cast<ICpu>().ptr();
}

void CcMemoryMonitor::deinit()
{
  if(g_pMemoryList == nullptr)
  {
    delete g_pMemoryList;
    g_pMemoryList = nullptr;
  }
}

void CcMemoryMonitor::lock()
{
  if (g_bMemoryEnabled)
  {
#ifdef WINDOWS
    EnterCriticalSection(&g_oCriticalSection);
#else
    if (g_pCpu &&
        g_pCpu->isInIsr() == false)
    {
      g_pCpu->enterCriticalSection();
    }
    g_oMutex.lock();
#endif
  }
}

void CcMemoryMonitor::unlock()
{
  if (g_bMemoryEnabled)
  {
#ifdef WINDOWS
    LeaveCriticalSection(&g_oCriticalSection);
#else
    g_oMutex.unlock();
    if (g_pCpu &&
        g_pCpu->isInIsr() == false)
    {
      g_pCpu->leaveCriticalSection();
    }
#endif
  }
}

void CcMemoryMonitor__insert(const void* pBuffer, const char* pFile, int iLine)
{
  CcMemoryMonitor::insert(pBuffer, pFile, static_cast<size_t>(iLine));
}

void CcMemoryMonitor::insert(const void* pBuffer, const char* pFile, size_t iLine)
{
  lock();
  CCCHECKNULL(pBuffer);
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    if (pBuffer == nullptr)
    {
      CcKernel::message(EMessage::Warning);
    }
    else
    {
      CItem pItem(pBuffer);
      pItem.pFile = pFile;
      pItem.iLine = iLine;
      g_pMemoryList->push_front(pItem);
    }
  }
  unlock();
}

bool CcMemoryMonitor::contains(const void* pBuffer)
{
  bool bContains = false;
  lock();
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    for(const CItem& rItem : *g_pMemoryList)
    {
      if (rItem.pBuffer == pBuffer)
      {
        bContains = true;
      }
    }
  }
  unlock();
  return bContains;
}

void CcMemoryMonitor__remove(const void* pBuffer)
{
  CcMemoryMonitor::remove(pBuffer);
}

void CcMemoryMonitor::remove(const void* pBuffer)
{
  lock();
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    if (pBuffer == nullptr)
    {
      CcKernel::message(EMessage::Warning);
    }
    else
    {
      std::list<CItem>::iterator oIter = g_pMemoryList->begin();
      while(oIter != g_pMemoryList->end())
      {
        if (oIter->pBuffer == pBuffer)
        {
          oIter = g_pMemoryList->erase(oIter);
        }
        else
        {
          oIter++;
        }
      }
    }
  }
  unlock();
}

void CcMemoryMonitor::printLeft(IIoDevice& oStream)
{
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr &&
      g_pMemoryList->size() > 0)
  {
    g_bMemoryEnabled = false;
    std::list<CItem>::iterator oIterator = g_pMemoryList->begin();
    do
    {
      CcString sLine;
      size_t uiPosLastPath =  CcStringUtil::findLastChar(oIterator->pFile, CcGlobalStrings::Seperators::Path[0]);
      if(uiPosLastPath == SIZE_MAX)
      {
        uiPosLastPath = 0;
      }
      sLine.append(oIterator->pFile + uiPosLastPath + 1);
      if(sLine == "CcString.cpp")
      {
        sLine << " " << ((char*)oIterator->pBuffer);
      }
      sLine << " " << CcString::fromNumber(oIterator->iLine) << CcGlobalStrings::EolShort;
      if(!oStream.writeString(sLine))
      {
        break;
      }
      oIterator++;
    } while (g_pMemoryList->end() != oIterator);
    g_bMemoryEnabled = true;
  }
}

size_t CcMemoryMonitor::getAllocationCount()
{
  lock();
  size_t uiAllocations = 0;
  if(g_pMemoryList != nullptr)
  {
    uiAllocations = g_pMemoryList->size();
  }
  unlock();
  return uiAllocations;
}

void CcMemoryMonitor::clear()
{
  lock();
  if(g_pMemoryList != nullptr)
  {
    g_pMemoryList->clear();
  }
  unlock();
}
