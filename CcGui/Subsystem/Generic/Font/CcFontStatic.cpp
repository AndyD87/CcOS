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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFontStatic
 */
#include "CcFontStatic.h"

CcFontStatic::CcFontStatic()
{

}

int16 CcFontStatic::swapInt16(int16 iInput)
{
  swap(&iInput, sizeof(iInput))  ;
  return iInput;
}

int32 CcFontStatic::swapInt32(int32 iInput)
{
  swap(&iInput, sizeof(iInput))  ;
  return iInput;
}

int64 CcFontStatic::swapInt64(int64 iInput)
{
  swap(&iInput, sizeof(iInput))  ;
  return iInput;
}

uint16 CcFontStatic::swapUint16(uint16 uiInput)
{
  swap(&uiInput, sizeof(uiInput))  ;
  return uiInput;
}

uint32 CcFontStatic::swapUint32(uint32 uiInput)
{
  swap(&uiInput, sizeof(uiInput))  ;
  return uiInput;
}

uint64 CcFontStatic::swapUint64(uint64 uiInput)
{
  swap(&uiInput, sizeof(uiInput))  ;
  return uiInput;
}

float CcFontStatic::uint16ToFloat(uint16 uiInput)
{
  uint16 uiCalc = uiInput & 0x7fff;
  float fOutput = static_cast<float>(uiCalc) / 0x8000;
  if(uiInput & 0x8000) fOutput -= fOutput;
  return fOutput;
}

void CcFontStatic::swap(void *pBuffer, size_t uiSize)
{
  uchar* puBuffer = static_cast<uchar*>(pBuffer);
  for(size_t i = 0; i < (uiSize/2); i++)
  {
    uchar uTemp = puBuffer[i] ;
    puBuffer[i] = puBuffer[uiSize - (1+i)];
    puBuffer[uiSize - (1+i)] = uTemp;
  }
}
