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
 * @brief     Class CcBinaryStream
 */
#include "Types/CcBinaryStream.h"
#include "CcStatic.h"
#include "CcStringUtil.h"

size_t CcBinaryStream::write(const void* pData, size_t uiSize)
{
  size_t uiMinSize = SIZE_MAX;
  if(m_rData.size() < m_uiPosition + uiSize &&
     m_bFixedSize)
  {
  }
  else if(m_rData.size() < m_uiPosition + uiSize &&
          m_uiPosition < m_rData.size())
  {
    size_t uiWrite = m_rData.size()-m_uiPosition;
    size_t uiAppend = uiSize - uiWrite;
    m_rData.write(pData, uiWrite, m_uiPosition);
    m_rData.append(static_cast<const char*>(pData) + uiWrite, uiAppend);
    uiMinSize = uiSize;
    m_uiPosition += uiSize;
  }
  else if(m_uiPosition == m_rData.size())
  {
    m_rData.append(static_cast<const char*>(pData), uiSize);
    uiMinSize = uiSize;
    m_uiPosition += uiSize;
  }
  else
  {
    m_rData.write(pData, uiSize, m_uiPosition);
    uiMinSize = uiSize;
    m_uiPosition += uiSize;
  }
  return uiMinSize;
}

size_t CcBinaryStream::read(void* pData, size_t uiSize)
{
  size_t uiMinSize = SIZE_MAX;
  if(m_rData.size() >= m_uiPosition + uiSize)
  {
    uiMinSize = m_rData.read(pData, uiSize, m_uiPosition);
    m_uiPosition += uiMinSize;
  }
  else if(m_rData.size() > m_uiPosition)
  {
    uiMinSize = m_rData.read(pData, m_rData.size() - m_uiPosition, m_uiPosition);
    m_uiPosition += uiMinSize;
  }
  return uiMinSize;
}

CcStatus CcBinaryStream::open(EOpenFlags)
{
  return true;
}

CcStatus CcBinaryStream::close()
{
  return false;
}

CcStatus CcBinaryStream::cancel()
{
  return false;
}

CcStatus CcBinaryStream::setPosition(size_t uiPosition)
{
  CcStatus oStatus(false);
  if(m_uiPosition <= m_rData.size())
  {
    m_uiPosition = uiPosition;
    oStatus = true;
  }
  return oStatus;
}
