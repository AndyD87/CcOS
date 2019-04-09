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
#elif defined WINDOWS
#include <Windows.h>
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

void* CcStatic::memcpySwapped(void* pDestination, const void* pSource, size_t uiSize)
{
  for(size_t uiPos = 0; uiPos < uiSize; uiPos++)
  {
    static_cast<uint8*>(pDestination)[uiPos] = static_cast<const uint8*>(pSource)[uiSize-1-uiPos];
  }
  return pDestination;
}

CcStatus CcStatic::mlock(void *pMemory, size_t uiSize)
{
  CcStatus oStatus(EStatus::NotSupported);
#ifdef LINUX
  oStatus.setSystemError(::mlock(pMemory, uiSize));
#elif defined WINDOWS
  if (!VirtualLock(pMemory, uiSize))
  {
    oStatus.setSystemError(static_cast<uint32>(GetLastError()));
  }
#else
  // Generic does not support this feature
  CCUNUSED(pMemory);
  CCUNUSED(uiSize);
#endif
  return oStatus;
}

CcStatus CcStatic::munlock(void *pMemory, size_t uiSize)
{
  CcStatus oStatus(EStatus::NotSupported);
#ifdef LINUX
  oStatus.setSystemError(::munlock(pMemory, uiSize));
#elif defined WINDOWS
  if (!VirtualUnlock(pMemory, uiSize))
  {
    oStatus.setSystemError(static_cast<uint32>(GetLastError()));
  }
#else
  // Generic does not support this feature
  CCUNUSED(pMemory);
  CCUNUSED(uiSize);
#endif
  return oStatus;
}

void CcStatic::swap(void *pBuffer, size_t uiSize)
{
  uchar* puBuffer = static_cast<uchar*>(pBuffer);
  for(size_t i = 0; i < (uiSize/2); i++)
  {
    uchar uTemp = puBuffer[i] ;
    puBuffer[i] = puBuffer[uiSize - (1+i)];
    puBuffer[uiSize - (1+i)] = uTemp;
  }
}

uint64 CcStatic::swapUint64(uint64 uiToSwap)
{
  uint32 uiRet = 0;
  uiRet |= (uiToSwap & 0x00000000000000ff) << (64 - (1 * 8));
  uiRet |= (uiToSwap & 0x000000000000ff00) << (64 - (3 * 8));
  uiRet |= (uiToSwap & 0x0000000000ff0000) << (64 - (5 * 8));
  uiRet |= (uiToSwap & 0x00000000ff000000) << (64 - (7 * 8));
  uiRet |= (uiToSwap & 0x000000ff00000000) >> (64 - (7 * 8));
  uiRet |= (uiToSwap & 0x0000ff0000000000) >> (64 - (5 * 8));
  uiRet |= (uiToSwap & 0x00ff000000000000) >> (64 - (3 * 8));
  uiRet |= (uiToSwap & 0xff00000000000000) >> (64 - (1 * 8));
  return uiRet;
}

uint32 CcStatic::swapUint32(uint32 uiToSwap)
{
  uint32 uiRet = 0;
  uiRet |= (uiToSwap & 0x000000ff) << 24;
  uiRet |= (uiToSwap & 0x0000ff00) << 8;
  uiRet |= (uiToSwap & 0x00ff0000) >> 8;
  uiRet |= (uiToSwap & 0xff000000) >> 24;
  return uiRet;
}

uint16 CcStatic::swapUint16(uint16 uiToSwap)
{
  uint16 uiRet = 0;
  uiRet |= (uiToSwap & 0x00ff) << 8;
  uiRet |= (uiToSwap & 0xff00) >> 8;
  return uiRet;
}
