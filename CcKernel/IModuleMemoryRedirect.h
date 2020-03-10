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
 * @page      CcKernel
 * @subpage   IModuleMemoryRedirect
 *
 * @page      IModuleMemoryRedirect
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Redirect new and delete to main application.
 *            This file must be included once on every Module
 */
#ifndef H_IModuleMemoryRedirect_H_
#define H_IModuleMemoryRedirect_H_

#include "IKernel.h"
#include <cstdlib>
#include <new>

IModule* IModule::s_pInstance = nullptr;

#ifndef _GLIBCXX_THROW
  #define _GLIBCXX_THROW(BLAH)
#endif
#ifndef _GLIBCXX_USE_NOEXCEPT
  #define _GLIBCXX_USE_NOEXCEPT
#endif
void* operator new(std::size_t uiSize) _GLIBCXX_THROW (std::bad_alloc)
{
  if (IModule::getInstance())
    return IModule::getInstance()->getKernel().opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

void* operator new[](std::size_t uiSize) _GLIBCXX_THROW (std::bad_alloc)
{
  if (IModule::getInstance())
    return IModule::getInstance()->getKernel().opNew(uiSize);
  else
    // redirect to malloc if instance not yet set or already removed
    return malloc(uiSize);
}

void operator delete(void* pBuffer) _GLIBCXX_USE_NOEXCEPT
{
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete(void* pBuffer, size_t uiSize) _GLIBCXX_USE_NOEXCEPT
{
  CCUNUSED(uiSize);
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete[](void* pBuffer) _GLIBCXX_USE_NOEXCEPT
{
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

void operator delete[](void* pBuffer, size_t uiSize) _GLIBCXX_USE_NOEXCEPT
{
  CCUNUSED(uiSize);
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    // redirect to free if instance not yet set or already removed
    free(pBuffer);
}

#endif // H_IModuleMemoryRedirect_H_
