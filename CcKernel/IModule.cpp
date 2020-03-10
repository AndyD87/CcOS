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
 * @brief     Class IModule
 */
#include "IModule.h"
#include "IKernel.h"
#include <cstdlib>
#include <new>

const CcString IModule::sCreateName(IModule_CreateFunctionName);
const CcString IModule::sRemoveName(IModule_RemoveFunctionName);
IModule* IModule::s_pInstance = nullptr;

#ifndef CcKernel_Build

void* operator new(size_t uiSize)
{
  if (IModule::getInstance())
    return IModule::getInstance()->getKernel().opNew(uiSize);
  else
    return malloc(uiSize);
}

void operator delete(void* pBuffer)
{
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    free(pBuffer);
}

void operator delete(void* pBuffer, size_t uiSize)
{
  CCUNUSED(uiSize);
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    free(pBuffer);
}

void* operator new[](size_t uiSize)
{
  if (IModule::getInstance())
    return IModule::getInstance()->getKernel().opNew(uiSize);
  else
    return malloc(uiSize);
}

void operator delete[](void* pBuffer)
{
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    free(pBuffer);
}

void operator delete[](void* pBuffer, size_t uiSize)
{
  CCUNUSED(uiSize);
  if (IModule::getInstance())
    IModule::getInstance()->getKernel().opDel(pBuffer);
  else
    free(pBuffer);
}
#endif
