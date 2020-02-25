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
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtations for malloc and new
 */

#include "Platform/Windows/Features/CcOS_malloc.h"
#include "CcKernel.h"
#include <ntddk.h>

#define CcOS_TAG 'CcOS'

void* operator new(size_t uiSize)
{
  return ExAllocatePoolWithTag(NonPagedPool, uiSize, CcOS_TAG);
}

void operator delete(void* pBuffer)
{
  ExFreePoolWithTag(pBuffer, CcOS_TAG);
}

void operator delete(void* pBuffer, size_t uiSize)
{
  CCUNUSED(uiSize);
  ExFreePoolWithTag(pBuffer, CcOS_TAG);
}

void* operator new[](size_t uiSize)
{
  return ExAllocatePoolWithTag(NonPagedPool, uiSize, CcOS_TAG);
}

void operator delete[](void* pBuffer)
{
  ExFreePoolWithTag(pBuffer, CcOS_TAG);
}

void operator delete[](void* pBuffer, size_t uiSize)
{
  CCUNUSED(uiSize);
  ExFreePoolWithTag(pBuffer, CcOS_TAG);
}

int atexit(void(*func)(void))
{
  CCUNUSED(func);
  return 0;
}

float _fltused = 0.0;