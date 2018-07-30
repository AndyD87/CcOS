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
 * @brief     Implementation of Class CcStatic
 */
#include "CcStatic.h"
#include <cstring>

#ifdef LINUX
  #include <sys/mman.h>
#endif

void* CcStatic::g_pNull = nullptr;

void* CcStatic::memset(void* pBuffer, int iValue, size_t uiSize)
{
  unsigned char* pcBuffer = static_cast<unsigned char*>(pBuffer);
  while(uiSize > 0)
  {
    *pcBuffer = static_cast<unsigned char>(iValue);
    pcBuffer++;
    uiSize--;
  }
  return pBuffer;
}

void* CcStatic::zerofill(void* pBuffer, size_t uiSize)
{
  size_t uiTempSize = uiSize / sizeof(int);
  for(size_t i = 0; i < uiTempSize; i++ )
  {
    static_cast<int*>(pBuffer)[i] = 0;
  }
  size_t uiRest = uiSize - (uiTempSize * sizeof(int));
  if(uiRest != 0)
  {
    size_t uiOffset = uiTempSize * sizeof(int);
    for(size_t i = uiOffset; i < uiSize; i++ )
    {
      static_cast<char*>(pBuffer)[i] = 0;
    }
  }
  return pBuffer;
}

void* CcStatic::memcpy(void* pDestination, const void* pSource, size_t uiSize)
{
  return ::memcpy(pDestination, pSource, uiSize);
}

CcStatus CcStatic::munlock(const void *pMemory, size_t uiSize)
{
  CcStatus oStatus(EStatus::NotSupported);
#ifdef LINUX
  oStatus.setSystemError(::munlock(pMemory, uiSize));
#elif WINDOWS

#endif
  return oStatus;
}

CcStatus CcStatic::mlock(const void *pMemory, size_t uiSize)
{
  CcStatus oStatus(EStatus::NotSupported);
#ifdef LINUX
  oStatus.setSystemError(::mlock(pMemory, uiSize));
#elif WINDOWS

#endif
  return oStatus;
}
