/**
 * @copyright  Andreas Dirmeier (C) 2017
 *
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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Implementation of Class CCcSharedMemory
 */
#include "CcSharedMemory.h"
#include "CcKernel.h"

CcSharedMemory::CcSharedMemory(const CcString& sName, size_t uiMaxSize)
{
  m_pSystem = CcKernel::getSharedMemory(sName, uiMaxSize);
}

CcSharedMemory::~CcSharedMemory()
{
  if (m_pSystem != nullptr)
  {
    delete m_pSystem;
  }
}

CcSharedMemory& CcSharedMemory::operator = (CcSharedMemory&& oToMove)
{
  if (this != &oToMove)
  {
    m_pSystem = oToMove.m_pSystem;
    oToMove.m_pSystem = nullptr;
  }
  return *this;
}

CcStatus CcSharedMemory::open(EOpenFlags eOpenFlags)
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->open(eOpenFlags);
  }
  else
  {
    return CcStatus(false);
  }
}

CcStatus CcSharedMemory::close()
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->close();
  }
  else
  {
    return CcStatus(false);
  }
}

CcStatus CcSharedMemory::cancel()
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->cancel();
  }
  else
  {
    return CcStatus(false);
  }
}

CcStatus CcSharedMemory::claim(EOpenFlags eOpenFlags)
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->claim(eOpenFlags);
  }
  else
  {
    return false;
  }
}

bool CcSharedMemory::exists()
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->exists();
  }
  else
  {
    return false;
  }
}

size_t CcSharedMemory::read(void* pBuffer, size_t uSize)
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->read(pBuffer, uSize);
  }
  else
  {
    return SIZE_MAX;
  }
}

size_t CcSharedMemory::write(const void *pBuffer, size_t uSize)
{
  if (m_pSystem != nullptr)
  {
    return m_pSystem->write(pBuffer, uSize);
  }
  else
  {
    return SIZE_MAX;
  }
}
