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
 * @brief     Class CcStringStream
 */
#include "CcStringStream.h"
#include "CcStatic.h"
#include "CcStringUtil.h"

size_t CcStringStream::write(const void* pData, size_t uiSize)
{
  rString.append(static_cast<const char*>(pData), uiSize);
  return uiSize;
}

size_t CcStringStream::read(void* pData, size_t uiSize)
{
  //! @todo, it's just a fake implementation
  size_t uiMinSize = CCMIN(uiSize, rString.size());
  CcStatic::memcpy(pData, rString.getCharString(), uiMinSize);
  return uiMinSize;
}

CcStatus CcStringStream::open(EOpenFlags)
{
  return false;
}

CcStatus CcStringStream::close()
{
  return false;
}

CcStatus CcStringStream::cancel()
{
  return false;
}

