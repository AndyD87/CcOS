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

class CcMemoryMonitorItem
{
public:
  size_t      iLine;
  const char* pFile = NULL;
};

static std::map<const void*, CcMemoryMonitorItem>* g_pMemoryList = nullptr;
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
  g_pMemoryList = new std::map<const void*, CcMemoryMonitorItem>;
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
      CcKernel::message(EMessage::Warning, "Do not add buffer at NULL to Memory Monitor");
    }
    else if (contains(pBuffer))
    {
      CcKernel::message(EMessage::Warning, "Buffer already exists in Memory Monitor");
    }
    else
    {
      CcMemoryMonitorItem pItem;
      pItem.pFile = pFile;
      pItem.iLine = iLine;
      g_pMemoryList->insert(std::pair<const void*, CcMemoryMonitorItem>(pBuffer, pItem));
    }
  }
}

bool CcMemoryMonitor::contains(const void* pBuffer)
{
  bool bContains = false;
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    std::map< const void*, CcMemoryMonitorItem>::iterator oIterator = g_pMemoryList->find(pBuffer);
    if (oIterator != g_pMemoryList->end())
    {
      bContains = true;
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
      CcKernel::message(EMessage::Warning, "Deleting on nullptr");
    }
    else if (!contains(pBuffer))
    {
      CcKernel::message(EMessage::Warning, "Queried buffer for delete is not existing");
    }
    else
    {
      g_pMemoryList->erase(pBuffer);
    }
  }
}

void CcMemoryMonitor::printLeft()
{
  if (g_bMemoryEnabled &&
      g_pMemoryList != nullptr)
  {
    for (const std::pair<const void*, CcMemoryMonitorItem>& oItem : *g_pMemoryList)
    {
      CcKernel::message(EMessage::Info, CcString::fromNumber(oItem.second.iLine) + oItem.second.pFile);
    }
  }
}

size_t CcMemoryMonitor::getAllocations()
{
  size_t uiAllocations = 0;
  if(g_pMemoryList != nullptr)
  {
    uiAllocations = g_pMemoryList->size();
  }
  return uiAllocations;
}
