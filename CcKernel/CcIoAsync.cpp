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
 * @brief     Class CcIoAsync
 */

#include "IIo.h"
#include "CcIoAsync.h"


CcIoAsync::CcIoAsync(IIo *device) :
  m_Device(device)
{
}

CcIoAsync::~CcIoAsync()
{
}

bool CcIoAsync::read(char* pBuffer, size_t uSize)
{
  size_t szRet = m_Device->read(pBuffer, uSize);
  onReadDone(szRet);
  return true;
}

bool CcIoAsync::write(const char* pBuffer, size_t uSize)
{
  size_t szRet = m_Device->write(pBuffer, uSize);
  onWriteDone(szRet);
  return true;
}

bool CcIoAsync::onReadDone(size_t size)
{
  if (size > 0)
    return true;
  else
    return false;
}

bool CcIoAsync::onWriteDone(size_t size)
{
  if (size > 0)
    return true;
  else 
    return false;
}
