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

static std::list<CcMemoryMonitor::CItem>* g_pMemoryList = nullptr;
bool g_bMemoryEnabled = false;

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
  deinit();
  g_pMemoryList = new std::list<CcMemoryMonitor::CItem>;
}

void CcMemoryMonitor::deinit()
{
  if(g_pMemoryList == nullptr)
  {
    delete g_pMemoryList;
    g_pMemoryList = nullptr;
  }
}

void CcMemoryMonitor__insert(const void* pBuffer, const char* pFile, int iLine)
{
  CcMemoryMonitor::insert(pBuffer, pFile, static_cast<size_t>(iLine));
}

void CcMemoryMonitor::insert(const void* pBuffer, const char* pFile, size_t iLine)
{
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
      g_pMemoryList->push_back(pItem);
    }
  }
}

bool CcMemoryMonitor::contains(const void* pBuffer)
{
  bool bContains = false;
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
  return bContains;
}

void CcMemoryMonitor__remove(const void* pBuffer)
{
  CcMemoryMonitor::remove(pBuffer);
}

void CcMemoryMonitor::remove(const void* pBuffer)
{
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    if (pBuffer == nullptr)
    {
      CcKernel::message(EMessage::Warning);
    }
    else
    {
      size_t uiPos = 0;
      std::list<CItem>::iterator oIter = g_pMemoryList->begin();
      while(oIter != g_pMemoryList->end())
      {
        if (oIter->pBuffer == pBuffer)
        {
          g_pMemoryList->erase(oIter);
          break;
        }
        uiPos++;
        oIter++;
      }
    }
  }
}

void CcMemoryMonitor::printLeft()
{
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr &&
      g_pMemoryList->size() > 0)
  {
    std::list<CItem>::iterator oIterator = g_pMemoryList->end();
    do
    {
      CcString sItem = CcString::fromNumber(oIterator->iLine) + oIterator->pFile;
      CcKernel::message(EMessage::Info);
      oIterator--;
    } while (g_pMemoryList->begin() != oIterator);
  }
}

size_t CcMemoryMonitor::getAllocationCount()
{
  size_t uiAllocations = 0;
  if(g_pMemoryList != nullptr)
  {
    uiAllocations = g_pMemoryList->size();
  }
  return uiAllocations;
}

void CcMemoryMonitor::clear()
{
  if(g_pMemoryList != nullptr)
  {
    g_pMemoryList->clear();
  }
}

std::list<CcMemoryMonitor::CItem>& CcMemoryMonitor::getAllocationList()
{
  return *g_pMemoryList;
}
