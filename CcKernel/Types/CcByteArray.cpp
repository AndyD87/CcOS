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
 * @brief     Implemtation of class CcByteArray
 */

#include "CcByteArray.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "Hash/CcCrc32.h"
#include "CcStringUtil.h"
#include "CcStatic.h"

CcByteArray::CcByteArray(const char* pToAppend)
{
  append((char*)pToAppend, CcStringUtil::strlen(pToAppend));
}

CcByteArray::CcByteArray(const char* pToAppend, size_t uiLen) :
  CcVector<char>(pToAppend, uiLen)
{
}

CcByteArray::CcByteArray(const unsigned char* pToAppend, size_t uiLen) :
  CcVector<char>(static_cast<const char*>(static_cast<const void*>(pToAppend)), uiLen)
{
}

CcByteArray::CcByteArray(const CcByteArray& oToAppend) :
  CcVector<char>(oToAppend)
{
}

CcByteArray::CcByteArray(CcByteArray&& oToMove) :
  CcVector<char>(CCMOVE(oToMove))
{
}


CcByteArray::CcByteArray(const CcString& sToAppend) :
  CcVector<char>(sToAppend.getCharString(), sToAppend.length())
{
}

CcByteArray::CcByteArray(size_t uiReserved):
  CcVector<char>(uiReserved)
{
  char* pData = (char*) getArray();
  while (uiReserved > 0)
  {
    *pData = 0;
    uiReserved--;
    pData++;
  }
}

size_t CcByteArray::find(const CcString& toFind, size_t uiBegin, size_t uiDistance) const
{
  CcByteArray oArray = toFind.getByteArray();
  return CcVector<char>::find(oArray, uiBegin, uiDistance);
}

size_t CcByteArray::getCharArray(char *buf, size_t bufSize, size_t from )
{
  size_t iRet=0;
  size_t iTo = bufSize + from;
  if (from <= size())
  {
    while (from < size() && from < iTo)
    {
      buf[iRet] = at(from);
      from++;
      iRet++;
    }
  }
  else
    iRet = SIZE_MAX;
  return iRet;
}

bool CcByteArray::setHexString(const CcString& sHexString)
{
  bool bRet = false;
  clear();
  size_t uiOffset=0;
  if (sHexString.startsWith("0x"))
    uiOffset = 2;
  for (size_t i = uiOffset; i + 1 < sHexString.length(); i++)
  {
    char newChar = 0;
    if (sHexString[i] >= 'a' && sHexString[i] <= 'z')
    {
      newChar = (sHexString[i] - ('a' - 10)) << 4;
    }
    else if (sHexString[i] >= 'A' && sHexString[i] <= 'Z')
    {
      newChar = (sHexString[i] - ('A' - 10)) << 4;
    }
    else if (sHexString[i] >= '0' && sHexString[i] <= '9')
    {
      newChar = ( sHexString[i] - '0'      ) << 4;
    }
    i++;
    if (sHexString[i] >= 'a' && sHexString[i] <= 'z')
    {
      newChar |= (sHexString[i] - ('a' - 10));
    }
    else if (sHexString[i] >= 'A' && sHexString[i] <= 'Z')
    {
      newChar |= (sHexString[i] - ('A' - 10));
    }
    else if (sHexString[i] >= '0' && sHexString[i] <= '9')
    {
      newChar |= (sHexString[i] - '0');
    }
    append(newChar);
  }
  return bRet;
}

void CcByteArray::appendString(const CcString& sString)
{
  append(sString.getCharString(), sString.length());
}

CcString CcByteArray::getHexString()const
{
  CcString sRet(size()*2, 0);
  size_t iString = 0;
  for (size_t i = 0; i < size(); i++, iString+=2)
  {
    uchar ucCurrent = (uchar) at(i);
    if ((ucCurrent >> 4) < 10)
    {
      sRet[iString] = '0' + (ucCurrent >> 4);
    }
    else
    {
      sRet[iString] = 'a' + ((ucCurrent >> 4) - 10);
    }
    if ((ucCurrent & 0x0f) < 10)
    {
      sRet[iString + 1] = '0' + (ucCurrent & 0x0f);
    }
    else
    {
      sRet[iString + 1] = 'a' + ((ucCurrent & 0x0f) - 10);
    }
  }
  return sRet;
}

void CcByteArray::swapEndian()
{
  size_t uiSizeHalf = size() / 2;
  char iTemp;
  for (size_t i = 0; i < uiSizeHalf; i++)
  {
    iTemp = at(i);
    at(i) = at(size() - 1 - i);
    at(size() - 1 - i) = iTemp;
  }
}

void CcByteArray::swapEndian32()
{
  char iTemp;
  for (size_t i = 0; i < size()-3; i+=4)
  {
    iTemp = at(i);
    at(i) = at(i + 3);
    at(i+3) = iTemp;
    iTemp = at(i+1);
    at(i+1) = at(i+2);
    at(i+2) = iTemp;
  }
}

CcString CcByteArray::getOctalString()
{
  CcString sRet;
  for (char& rChar : *this)
  {
    sRet << CcStringUtil::getOctalStringFromByte(rChar);
  }
  return sRet;
}

CcCrc32 CcByteArray::getCrc32()
{
  CcCrc32 oCrc;
  oCrc.append(*this);
  return oCrc;
}

bool CcByteArray::startsWith(const char* pData, size_t uiSize) const
{
  bool bSuccess = false;
  if(uiSize <= size())
  {
    bSuccess = true;
    for(size_t uiIndex = 0; uiIndex < uiSize; uiIndex++)
    {
      if(pData[uiIndex] != at(uiIndex))
      {
        bSuccess = false;
        break;
      }
    }
  }
  return bSuccess;
}

CcByteArray& CcByteArray::operator=(const CcByteArray& oToCopy)
{
  CcVector<char>::operator=(oToCopy);
  return *this;
}

CcByteArray& CcByteArray::operator=(CcByteArray&& oToMove)
{
  CcVector<char>::operator=(CCMOVE(oToMove));
  return *this;
}

CcByteArray& CcByteArray::operator=(CcString&& oToMove) CCNOEXCEPT
{
  clear();
  char* pBuffer;
  size_t uiSize;
  size_t uiReserved;
  oToMove.extract(pBuffer, uiSize, uiReserved);
  transfer(pBuffer, uiReserved);
  resize(uiSize);
  return *this;
}

size_t CcByteArray::read(void* pBuffer, size_t uSize)
{
  if (uSize < CcVector::size())
  {
    char* pcBuffer = static_cast<char*>(pBuffer);
    getCharArray(pcBuffer, uSize);
  }
  else
  {
    uSize = 0;
  }
  return uSize;
}

size_t CcByteArray::write(const void* pBuffer, size_t uSize)
{
  if (uSize < CcVector::size())
  {
    const char* pcBuffer = static_cast<const char*>(pBuffer);
    for (size_t i = 0; i < uSize; i++)
    {
      at(i) = pcBuffer[i];
    }
  }
  else
  {
    uSize = 0;
  }
  return uSize;
}
