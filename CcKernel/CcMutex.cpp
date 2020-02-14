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
 * @brief     Class CcMutex
 */

#include "CcMutex.h"
#include "CcKernel.h"

CcMutex::CcMutex()
{
#ifdef _MSC_VER
  m_oMutex = new std::mutex;
#else
  while (isLocked() == true)
    CcKernel::delayMs(0);
  m_bLocked = true;
#endif
}

CcMutex::~CcMutex()
{
  lock();
  unlock();
#ifdef _MSC_VER
  delete m_oMutex;
#endif
}

void CcMutex::lock()
{
#ifdef _MSC_VER
  m_oMutex->lock();
#else
  while (isLocked() == true) 
    CcKernel::delayMs(0);
  m_bLocked = true;
#endif
}

void CcMutex::unlock()
{
#ifdef _MSC_VER
  m_oMutex->unlock();
#else
  m_bLocked = false;
#endif
}

bool CcMutex::isLocked()
{
#ifdef _MSC_VER
  return !m_oMutex->try_lock();
#else
  return m_bLocked;
#endif
}