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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcIOBuffer
 */

#include "CcIOBuffer.h"

CcIOBuffer::CcIOBuffer()
{

}

CcIOBuffer::~CcIOBuffer()
{

}

size_t CcIOBuffer::read(char* buffer, size_t size)
{
  size_t uiReadData = m_oBuffer.getCharArray(buffer, size, m_uiCurrentReadPos);
  if (uiReadData <= size)
    m_uiCurrentReadPos += uiReadData;
  else
    uiReadData = 0;
  return uiReadData;
}

size_t CcIOBuffer::write(const char* buffer, size_t size)
{
  m_oBuffer.append(buffer, size);
  return size;
}

bool CcIOBuffer::open(EOpenFlags)
{
  return true;
}

bool CcIOBuffer::close()
{
  return true;
}

bool CcIOBuffer::cancel()
{
  return true;
}