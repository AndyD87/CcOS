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
 * @brief     Class CcIOBuffer
 */

#include "CcIOBuffer.h"

size_t CcIOBuffer::read(void* pBuffer, size_t uSize)
{
  size_t uiReadData = m_oBuffer.getCharArray(static_cast<char*>(pBuffer), uSize, m_uiCurrentReadPos);
  if (uiReadData <= uSize)
    m_uiCurrentReadPos += uiReadData;
  else
    uiReadData = 0;
  return uiReadData;
}

size_t CcIOBuffer::write(const void* pBuffer, size_t uSize)
{
  m_oBuffer.append(static_cast<const char*>(pBuffer), uSize);
  return uSize;
}

CcStatus CcIOBuffer::open(EOpenFlags)
{
  return true;
}

CcStatus CcIOBuffer::close()
{
  return true;
}

CcStatus CcIOBuffer::cancel()
{
  return true;
}
