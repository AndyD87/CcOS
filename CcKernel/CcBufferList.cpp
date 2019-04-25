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
 * @brief     Class CcBufferList
 */

#include "CcBufferList.h"
#include "CcStatic.h"
#include "CcString.h"
#include "Hash/CcCrc32.h"

CcBufferList& CcBufferList::append(const CcByteArray& oByteArray)
{
  clear();
  m_uiSize = oByteArray.size();
  m_uiPosition = m_uiSize;
  CcList<CcByteArray>::append(oByteArray);
  return *this;
}

CcBufferList& CcBufferList::append(CcByteArray&& oByteArray)
{
  clear();
  m_uiSize = oByteArray.size();
  m_uiPosition = m_uiSize;
  CcList<CcByteArray>::append(std::move(oByteArray));
  return *this;
}

CcBufferList& CcBufferList::append(const CcString& oByteArray)
{
  m_uiSize += oByteArray.length();
  m_uiPosition = m_uiSize;
  CcList<CcByteArray>::append(oByteArray);
  return *this;
}

CcBufferList& CcBufferList::append(CcString&& oByteArray)
{
  m_uiSize += oByteArray.length();
  m_uiPosition = m_uiSize;
  CcList<CcByteArray>::append(std::move(oByteArray));
  return *this;
}

size_t CcBufferList::read(void* pBuffer, size_t uSize)
{
  size_t uiReadData = 0;
  size_t uiCurrentOffset = 0;
  size_t uiEndOfRead = m_uiPosition + uSize;
  char* pBufferPointer = static_cast<char*>(pBuffer);
  for (CcByteArray& oBuffer : *this)
  {
    if (uiCurrentOffset + oBuffer.size() < m_uiPosition)
    {
    }
    else if (uiCurrentOffset <= uiEndOfRead)
    {
      size_t uiCurrentBufferSize = oBuffer.size();
      char* pBufferData = oBuffer.getArray(0);
      if (uiCurrentOffset != m_uiPosition)
      {
        uiCurrentBufferSize -= m_uiPosition - uiCurrentOffset;
        pBufferData += m_uiPosition - uiCurrentOffset;
      }
      if (uSize >= uiCurrentBufferSize)
      {
        CcStatic::memcpy(pBufferPointer, pBufferData, oBuffer.size());
        uSize -= uiCurrentBufferSize;
        pBufferPointer += uiCurrentBufferSize;
        uiReadData += uiCurrentBufferSize;
        m_uiPosition += uiCurrentBufferSize;
      }
      else
      {
        CcStatic::memcpy(pBufferPointer, pBufferData, uSize);
        pBufferPointer += uSize;
        uiReadData += uSize;
        m_uiPosition += uSize;
      }
    }
    else
    {
      break;
    }
    uiCurrentOffset += oBuffer.size();
  }
  return uiReadData;
}

size_t CcBufferList::write(const void* pBuffer, size_t uSize)
{
  size_t uiWrittenData = SIZE_MAX;
  if (m_uiPosition == m_uiSize)
  {
    append(pBuffer, uSize);
  }
  else
  {
    size_t uiCurrentOffset = 0;
    size_t uiEndOfWrite = m_uiPosition + uSize;
    const char* pBufferPointer = static_cast<const char*>(pBuffer);
    uiWrittenData = uSize;
    for (CcByteArray& oBuffer : *this)
    {
      if (uiCurrentOffset + oBuffer.size() < m_uiPosition)
      {
        uiCurrentOffset += oBuffer.size();
      }
      else if (uiCurrentOffset <= uiEndOfWrite)
      {
        size_t uiCurrentBufferSize = oBuffer.size();
        char* pBufferData = oBuffer.getArray(0);
        if(uiCurrentOffset != m_uiPosition)
        {
          uiCurrentBufferSize -= m_uiPosition - uiCurrentOffset;
          pBufferData += m_uiPosition - uiCurrentOffset;
        }
        if (uSize >= uiCurrentBufferSize)
        {
          CcStatic::memcpy(pBufferData, pBufferPointer, uiCurrentBufferSize);
          uSize -= uiCurrentBufferSize;
          pBufferPointer += uiCurrentBufferSize;
          m_uiPosition += uiCurrentBufferSize;
        }
        else
        {
          CcStatic::memcpy(pBufferData, pBufferPointer, uSize);
          pBufferPointer += uSize;
          m_uiPosition += uSize;
        }
      }
      else
      {
        break;
      }
      uiCurrentOffset += oBuffer.size();
    }
    if (uSize)
    {
      append(pBufferPointer, uSize);
    }
  }
  return uiWrittenData;
}

size_t CcBufferList::readAll(void* pBuffer, size_t uSize) const
{
  size_t uiRead = 0;
  char* pTempBuffer = static_cast<char*>(pBuffer);
  for (CcByteArray& oBuffer : *this)
  {
    if(uSize > 0)
    {
      size_t uNewSize = CCMIN(uSize, oBuffer.size());
      CcStatic::memcpy(pTempBuffer, oBuffer.getArray(), uNewSize);
      uiRead += uNewSize;
      uSize -= uNewSize;
      pTempBuffer +=uNewSize;
    }
  }
  return uSize;
}

size_t CcBufferList::writeAll(const void* pBuffer, size_t uSize)
{
  clear();
  append(pBuffer, uSize);
  return uSize;
}

CcBufferList& CcBufferList::append(const void* pBuffer, size_t uSize)
{
  CcByteArray oData;
  oData.append(static_cast<const char*>(pBuffer), uSize);
  CcList<CcByteArray>::append(std::move(oData));
  m_uiSize += uSize;
  m_uiPosition = m_uiSize;
  return *this;
}

void CcBufferList::transferBegin(void* pBuffer, size_t uSize)
{
  CcByteArray oData;
  oData.transfer(static_cast<char*>(pBuffer), uSize);
  CcList<CcByteArray>::prepend(std::move(oData));
  m_uiSize += uSize;
  m_uiPosition = m_uiSize;
}

void CcBufferList::transfer(void* pBuffer, size_t uSize)
{
  CcByteArray oData;
  oData.transfer(static_cast<char*>(pBuffer), uSize);
  CcList<CcByteArray>::append(std::move(oData));
  m_uiSize += uSize;
  m_uiPosition = m_uiSize;
}

void CcBufferList::clear()
{
  CcList<CcByteArray>::clear();
  m_uiSize = 0;
  m_uiPosition = 0;
}

void CcBufferList::collapse()
{
  if (getChunkCount() == 0)
  {
    CcByteArray oData;
    CcList<CcByteArray>::append(oData);
  }
  else if (getChunkCount() > 1)
  {
    size_t uiSize = m_uiSize;
    char* pNewData = new char[m_uiSize];
    CCMONITORNEW(pNewData);
    size_t uiOffset = 0;
    for (CcByteArray& rData : *this)
    {
      CcStatic::memcpy(pNewData + uiOffset, rData.getArray(), rData.size());
      uiOffset += rData.size();
    }
    clear();
    transfer(pNewData, uiSize);
  }
  m_uiPosition = 0;
}

void* CcBufferList::getBuffer()
{
  collapse();
  if(size()>0)
    return (at(0).getArray());
  return nullptr;
}

void* CcBufferList::getCurrentBuffer() const
{
  if(size()>0)
    return (at(0).getArray() + m_uiPosition);
  return nullptr;
}

uint32 CcBufferList::getCrc32()
{
  CcCrc32 oCrc;
  for (CcByteArray& oBuffer : *this)
  {
    oCrc.append(oBuffer);
  }
  return oCrc.getValueUint32();
}
