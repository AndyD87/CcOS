/*
 * @copyright  Andreas Dirmeier (C) 2022
 *
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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGenericThreadHelper
 */
#include "CcGenericThreadHelper.h"
#include "CcGenericThreadHelper_prvt.h"
#include "CcKernel.h"


//! see definitions from https://github.com/Akagi201/uCOS-III/blob/master/Source/os.h
class SGenericThreadHelperListItem
{
public:
  SGenericThreadHelperListItem(void* pContext, void* pStackPointer, const char* pName) :
    pContext(pContext),
    StkPtr(pStackPointer),
    NamePtr(pName)
  {}
  void* pContext;
  void* StkPtr;
  const char* NamePtr;
  unsigned char TaskState = 1;
  unsigned char Prio      = 0;
  SGenericThreadHelperListItem* DbgPrevPtr = nullptr;
  SGenericThreadHelperListItem* DbgNextPtr = nullptr;
};

void CcGenericThreadHelper::insert(void* pContext, void* pStackPointer, const char* pName)
{
  CCNEWTYPE(pNewItem, SGenericThreadHelperListItem, pContext, pStackPointer, pName);
  SGenericThreadHelperListItem* pCurrentItem = static_cast<SGenericThreadHelperListItem*>(pxReadyTasksLists);
  pNewItem->DbgNextPtr = pCurrentItem;
  if(pCurrentItem != nullptr)
  {
    pCurrentItem->DbgPrevPtr = pNewItem;
  }
  pxReadyTasksLists = pNewItem;
  uxCurrentNumberOfTasks++;
}

void CcGenericThreadHelper::current(void* pContext)
{
  SGenericThreadHelperListItem* pCurrentItem = static_cast<SGenericThreadHelperListItem*>(pxReadyTasksLists);
  while(pCurrentItem != nullptr)
  {
    if(pCurrentItem->pContext == pContext)
    {
      pxReadyTasksLists = pCurrentItem;
      break;
    }
    pCurrentItem = pCurrentItem->DbgNextPtr;
  }
}

void CcGenericThreadHelper::remove(void* pContext)
{
  SGenericThreadHelperListItem* pCurrentItem = static_cast<SGenericThreadHelperListItem*>(pxReadyTasksLists);
  while(pCurrentItem != nullptr)
  {
    if(pCurrentItem->pContext == pContext)
    {
      if(pCurrentItem->DbgNextPtr != nullptr)
        pCurrentItem->DbgNextPtr->DbgPrevPtr = pCurrentItem->DbgPrevPtr;
      if(pCurrentItem->DbgPrevPtr != nullptr)
        pCurrentItem->DbgPrevPtr->DbgNextPtr = pCurrentItem->DbgNextPtr;
      else
        pxReadyTasksLists = pCurrentItem->DbgNextPtr;
      uxCurrentNumberOfTasks--;
      CCDELETE(pCurrentItem);
      break;
    }
  }
}
