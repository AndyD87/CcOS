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

#ifdef LINUX
  #include <sys/mman.h>
#include <cstring>
#elif defined WINDOWS && !defined(_KERNEL_MODE)
#include <windows.h>
#include <cstring>
#elif defined GENERIC
#include <cstring>
#endif

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

int CcStatic::memcmp(const void* pBuffer1, const void* pBuffer2, size_t uiLen)
{
  int iRet = 0;
  size_t i = 0;
  const unsigned char* puBuffer1 =  static_cast<const unsigned char*>(pBuffer1);
  const unsigned char* puBuffer2 =  static_cast<const unsigned char*>(pBuffer2);
  while( iRet == 0 && i < uiLen )
  {
    if(puBuffer1[i] < puBuffer2[i])
    {
      iRet = -static_cast<int>(i);
      iRet--;
      break;
    }
    else if(puBuffer1[i] > puBuffer2[i])
    {
      iRet = static_cast<int>(i);
      iRet++;
      break;
    }
    i++;
  }
  return iRet;
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
#ifdef LINUX
  return ::memcpy(pDestination, pSource, uiSize);
#elif defined WINDOWS && !defined(_KERNEL_MODE)
  return ::memcpy(pDestination, pSource, uiSize);
#elif defined GENERIC
  return ::memcpy(pDestination, pSource, uiSize);
#else
  // Generic does not support this feature
  char* pcDest = static_cast<char*>(pDestination);
  const char* pcSource = static_cast<const char*>(pSource);
  for (size_t uiPos = 0; uiPos < uiSize; uiPos++)
  {
    pcDest[uiPos] = pcSource[uiPos];
  }
  return pcDest;
#endif
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
#elif defined WINDOWS && !defined(_KERNEL_MODE)
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
#elif defined WINDOWS && !defined(_KERNEL_MODE)
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
