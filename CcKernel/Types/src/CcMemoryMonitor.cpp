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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcMemoryMonitor
 */
#include "CcMemoryMonitor.h"
#include "CcKernel.h"
#include "CcThreadObject.h"
#include <cstdlib>
#include <list>
#include <map>

bool s_bIsInit = false;

class CcMemoryMonitorItem
{
public:
  int   iLine;
  const char* pFile = NULL;
};

static std::map<const void*, CcMemoryMonitorItem>* m_oMemoryList;
bool m_bMemoryEnabled = false;

void CcMemoryMonitor::enable()
{
  m_bMemoryEnabled = true;
  initLists();
  //initThread();
}

void CcMemoryMonitor::initLists()
{
  if (m_bMemoryEnabled)
  {
    m_oMemoryList = new std::map<const void*, CcMemoryMonitorItem>;
    s_bIsInit = true;
  }
}

void CcMemoryMonitor::initThread()
{
  if (m_bMemoryEnabled)
  {
    //m_oMonitorThread.start();
  }
}

void CcMemoryMonitor::deinit()
{
  if (m_bMemoryEnabled)
  {
    //m_oMonitorThread.stop();
    delete m_oMemoryList;
  }
}

void CcMemoryMonitor::insert(const void* pBuffer, const char* pFile, int iLine)
{
  if (m_bMemoryEnabled)
  {
    if (s_bIsInit)
    {
      if (pBuffer == nullptr)
      {
        //CCDEBUG("Do not add buffer at NULL to Memory Monitor");
      }
      else
      {
        CcMemoryMonitorItem pItem;
        pItem.pFile = pFile;
        pItem.iLine = iLine;
        m_oMemoryList->insert(std::pair<const void*, CcMemoryMonitorItem>(pBuffer, pItem));
      }
    }
  }
}

void CcMemoryMonitor::remove(const void* pBuffer)
{
  if (m_bMemoryEnabled)
  {
    m_oMemoryList->erase(pBuffer);
  }
}

void CcMemoryMonitor::printLeft()
{
  if (m_bMemoryEnabled)
  {
    for (std::pair<const void*, CcMemoryMonitorItem> oItem : *m_oMemoryList)
    {
      printf("Not resolved: %p %s\n", oItem.first, oItem.second.pFile);
    }
  }
}