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
 * @par       Web: http://adirmeier.de/CcOS
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

bool s_bIsInit = false;

class CcMemoryMonitorItem
{
public:
  //CcMemoryMonitorItem* prv;
  void* pBuffer = NULL;
  int   iLine;
  const char* pFile = NULL;
};

static std::list<CcMemoryMonitorItem>* m_oMemoryList;
static std::list<void*>* m_oMemoryRemoveList;

class CcMemoryMonitorWorker : public CcThreadObject
{
  void run() override
  {
    while (getThreadState() == EThreadState::Running)
    {
      while (m_oMemoryRemoveList->empty() == false)
      {
        void* pCurrentBuffer = *(m_oMemoryRemoveList->begin());
        m_oMemoryRemoveList->pop_front();
        std::list<CcMemoryMonitorItem>::iterator oIterator = m_oMemoryList->begin();
        std::list<CcMemoryMonitorItem>::iterator oIteratorEnd = m_oMemoryList->end();
        while (oIterator != oIteratorEnd)
        {
          if (oIterator->pBuffer == pCurrentBuffer)
          {
            m_oMemoryList->erase(oIterator);
            break;
          }
          else
            oIterator++;
        }
      }
      CcKernel::delayMs(1);
    }
  }
};

CcMemoryMonitorWorker m_oMonitorThread;

void CcMemoryMonitor::initLists()
{
  m_oMemoryList = new std::list<CcMemoryMonitorItem>;
  m_oMemoryRemoveList = new std::list<void*>;
  s_bIsInit = true;
}

void CcMemoryMonitor::initThread()
{
  m_oMonitorThread.start();
}

void CcMemoryMonitor::deinit()
{
  m_oMonitorThread.stop();
  delete m_oMemoryList;
  delete m_oMemoryRemoveList;
}

void CcMemoryMonitor::insert(void* pBuffer, const char* pFile, int iLine)
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
      pItem.pBuffer = pBuffer;
      pItem.pFile = pFile;
      pItem.iLine = iLine;
      m_oMemoryList->push_back(pItem);
    }
  }
}

void CcMemoryMonitor::remove(void* pBuffer)
{
  if (s_bIsInit)
  {
    if (pBuffer == nullptr)
    {
      //CCDEBUG("Do not remove buffer at NULL from Memory Monitor");
    }
    else
    {
      m_oMemoryRemoveList->push_back(pBuffer);
    }
  }
}
