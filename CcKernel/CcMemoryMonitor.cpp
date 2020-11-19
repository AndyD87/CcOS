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

#define CCMONITOR_IGNORE
#include "CcBase.h"
#include "CcMemoryMonitor.h"
#include "CcKernel.h"
#include "IThread.h"
#include <cstdlib>
#include <map>
#include "CcMutex.h"
#include "Devices/ICpu.h"
#include "CcDevice.h"
#include "IIo.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"

#ifdef MEMORYMONITOR_ENABLED
static CcMemoryMonitor::SInterface* g_pMemoryInterface = nullptr;
static ICpu*                        g_pCpu = nullptr;
static bool                         g_bMemoryEnabled = false;
static std::map<const void*, CcMemoryMonitor::CItem>* g_pMemoryList = nullptr;

#ifdef WINDOWS
#include <windows.h>
CRITICAL_SECTION g_oCriticalSection;
#endif

size_t CcMemoryMonitor::CItem::uiCurrentIndex = 0;
CcMemoryMonitor_Type CcMemoryMonitor::m_oContext;

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
#ifdef USE_STD_MUTEX
#elif defined(LINUX)
  m_oContext = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#elif defined(WINDOWS)
  InitializeCriticalSection(&m_oContext);
#else
  m_oContext = false;
#endif
  lock();
  g_bMemoryEnabled = false;
  CCNEW(g_pMemoryList, std::map<const void* CCCOMMA CcMemoryMonitor::CItem>);
  g_pCpu = CcKernel::getDevice(EDeviceType::Cpu, 0).cast<ICpu>().ptr();
  unlock();
}

void CcMemoryMonitor::deinit()
{
  lock();
  g_bMemoryEnabled = false;
  g_pMemoryInterface = nullptr;
  CCDELETE(g_pMemoryList);
  unlock();
#ifdef USE_STD_MUTEX
#elif defined(LINUX)
#elif defined(WINDOWS)
  DeleteCriticalSection(&m_oContext);
#else
#endif
}

void CcMemoryMonitor::lock()
{
  if (g_bMemoryEnabled)
  {
#ifdef USE_STD_MUTEX
    m_oContext.lock();
#elif defined(LINUX)
    pthread_mutex_lock(&m_oContext);
#elif defined(WINDOWS)
    EnterCriticalSection(&m_oContext);
#else
    while (isLocked() == true)
      CcKernel::delayMs(0);
    m_oContext = true;
#endif
  }
}

void CcMemoryMonitor::unlock()
{
  if (g_bMemoryEnabled)
  {
#ifdef USE_STD_MUTEX
    m_oContext.unlock();
#elif defined(LINUX)
    pthread_mutex_unlock(&m_oContext);
#elif defined(WINDOWS)
    LeaveCriticalSection(&m_oContext);
#else
    m_oContext = false;
#endif
  }
}

bool CcMemoryMonitor::isLocked()
{
  if (g_bMemoryEnabled)
  {
#ifdef USE_STD_MUTEX
    if(m_oContext.try_lock())
    {
      m_oContext.unlock();
      return false;
    }
    else
    {
      return true;
    }
#elif defined(LINUX)
    if(0 == pthread_mutex_trylock(&m_oContext))
    {
      pthread_mutex_unlock(&m_oContext);
      return false;
    }
    else
    {
      return true;
    }
#elif defined(WINDOWS)
    if(TryEnterCriticalSection(&m_oContext))
    {
      LeaveCriticalSection(&m_oContext);
      return false;
    }
    else
    {
      return true;
    }
#else
    return m_oContext;
#endif
  }
  else
    return false;
}

void CcMemoryMonitor__insert(const void* pBuffer, const char* pFile, int iLine)
{
  CcMemoryMonitor::insert(pBuffer, pFile, static_cast<size_t>(iLine));
}

void CcMemoryMonitor::insert(const void* pBuffer, const char* pFile, size_t iLine)
{
  if (g_pMemoryInterface)
  {
    g_pMemoryInterface->opNewMemory(pBuffer, pFile, iLine);
  }
  else
  {
    lock();
    if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
    {
      std::map<const void*, CItem>*pMemoryList = g_pMemoryList;
      if (pBuffer == nullptr)
      {
        unlock();
        CcKernel::message(EMessage::Warning, "Buffer is null!");
        lock();
      }
      else if (contains(pBuffer))
      {
        unlock();
        std::map<const void*, CcMemoryMonitor::CItem>::iterator uiPos = g_pMemoryList->find(pBuffer);
        CcMemoryMonitor::CItem oTest = uiPos->second;
        CcKernel::message(EMessage::Warning, CcString("Buffer already existing") + oTest.pFile);
        lock();
      }
      else
      {
        CItem pItem(pBuffer);
        pItem.uiIndex = CItem::uiCurrentIndex++;
        pItem.pFile = pFile;
        pItem.iLine = iLine;
        pMemoryList->insert(std::pair<const void*, CItem>(pBuffer, pItem));
      }
    }
    unlock();
  }
}

void CcMemoryMonitor__remove(const void* pBuffer)
{
  CcMemoryMonitor::remove(pBuffer);
}

void CcMemoryMonitor::remove(const void* pBuffer)
{
  if (g_pMemoryInterface)
  {
    g_pMemoryInterface->opDelMemory(pBuffer);
  }
  else
  {
    lock();
    if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
    {
      if (pBuffer == nullptr)
      {
        unlock();
        CcKernel::message(EMessage::Warning, "Buffer is null!");
        lock();
      }
      else
      {
        std::map<const void*, CItem> *pMemoryList = g_pMemoryList;
        size_t uiNumber = pMemoryList->erase(pBuffer);
        if (uiNumber != 1)
          CcKernel::message(EMessage::Warning, "Invalid buffer erased");
      }
    }
    unlock();
  }
}

void CcMemoryMonitor::printLeft(IIo* pStream)
{
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr &&
      g_pMemoryList->size() > 0)
  {
    g_bMemoryEnabled = false;
    for(std::pair<const void*, CItem> rListItem : *g_pMemoryList)
    {
      CcString sLine;
      sLine << CcGlobalStrings::Space << CcString::fromSize(rListItem.second.uiIndex) << ": Line " << CcString::fromSize(rListItem.second.iLine) << CcGlobalStrings::Space << rListItem.second.pFile;
      if(sLine.endsWith("CcString.cpp", ESensitivity::CaseInsensitiv))
      {
        const char* pString = static_cast<const char*>(rListItem.second.pBuffer);
        sLine << CcGlobalStrings::Space << pString;
      }
      sLine << "\r\n";
      if(pStream != nullptr)
      {
        pStream->writeString(sLine);
      }
    }
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

void CcMemoryMonitor::setInterface(SInterface* pInterface)
{
  if( pInterface &&
      pInterface->opDelMemory != CcMemoryMonitor::remove &&
      pInterface->opNewMemory != CcMemoryMonitor::insert
  )
    g_pMemoryInterface = pInterface;
  else
    g_pMemoryInterface = nullptr;
}

bool CcMemoryMonitor::contains(const void* pBuffer)
{
  bool bContains = false;
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    bContains = g_pMemoryList->end() != g_pMemoryList->find(pBuffer);
  }
  return bContains;
}
#endif
