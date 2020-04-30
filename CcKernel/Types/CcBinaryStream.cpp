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
  m_rData.append(static_cast<const char*>(pData), uiSize);
  return uiSize;
}

size_t CcBinaryStream::read(void* pData, size_t uiSize)
{
  //! @todo, it's just a fake implementation
  size_t uiMinSize = CCMIN(uiSize, m_rData.size());
  CcStatic::memcpy(pData, m_rData.getArray(), uiMinSize);
  return uiMinSize;
}

CcStatus CcBinaryStream::open(EOpenFlags)
{
  return false;
}

CcStatus CcBinaryStream::close()
{
  return false;
}

CcStatus CcBinaryStream::cancel()
{
  return false;
}

