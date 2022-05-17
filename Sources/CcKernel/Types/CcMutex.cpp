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
 * @brief     Class CcMutex
 */

#include "CcMutex.h"
#if !defined(WINDOWSKERNEL)
  #include "CcKernel.h"
#endif // CCKERNEL_MODE

CcMutex::CcMutex()
{
#ifdef USE_STD_MUTEX
#elif defined(LINUX)
  m_oContext = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#elif defined(WINDOWSKERNEL)
  KeInitializeMutex(&m_oContext, 0);
#elif defined(WINDOWS)
  m_oContext = CreateMutex(NULL, FALSE, NULL);
  CCMONITORNEW(m_oContext);
#else
  m_oContext = false;
#endif
}

CcMutex::~CcMutex()
{
#ifdef USE_STD_MUTEX
#elif defined(LINUX)
#elif defined(WINDOWS)
  CCMONITORDELETE(m_oContext);
  CloseHandle(m_oContext);
#else
  lock();
  unlock();
#endif
}

void CcMutex::lock()
{
#ifdef USE_STD_MUTEX
  m_oContext.lock();
#elif defined(LINUX)
  pthread_mutex_lock(&m_oContext);
#elif defined(WINDOWSKERNEL)
  KeWaitForSingleObject(
    &m_oContext,
    Executive,
    KernelMode,
    FALSE,
    NULL
  );
#elif defined(WINDOWS)
  WaitForSingleObject(m_oContext, INFINITE);
#else
  while (isLocked() == true)
    CcKernel::delayMs(0);
  m_oContext = true;
#endif
}

bool CcMutex::tryLock()
{
#ifdef USE_STD_MUTEX
  return m_oContext.try_lock();
#elif defined(LINUX)
  return 0 == pthread_mutex_trylock(&m_oContext);
#elif defined(WINDOWSKERNEL)
  return STATUS_SUCCESS == KeWaitForSingleObject(
    &m_oContext,
    Executive,
    KernelMode,
    FALSE,
    NULL
  );
#elif defined(WINDOWS)
  return 0 == WaitForSingleObject(m_oContext, 0);
#else
  if(m_oContext == true)
    return false;
  else
    return m_oContext = true;
#endif
}

void CcMutex::unlock()
{
#ifdef USE_STD_MUTEX
  m_oContext.unlock();
#elif defined(LINUX)
  pthread_mutex_unlock(&m_oContext);
#elif defined(WINDOWSKERNEL)
  KeReleaseMutex(&m_oContext, FALSE);
#elif defined(WINDOWS)
  ReleaseMutex(m_oContext);
#else
  m_oContext = false;
#endif
}

bool CcMutex::isLocked()
{
#ifdef USE_STD_MUTEX
  if(m_oContext.try_lock())
  {
    m_oContext.unlock(&m_oContext);
    return false;
  }
  else
  {
    return true;
  }
#elif defined(LINUX)
  if(0 == pthread_mutex_trylock(&m_oContext))
  {
    pthread_mutex_unlock(&m_oContext);
    return false;
  }
  else
  {
    return true;
  }
#elif defined(WINDOWSKERNEL)
  return false;
#elif defined(WINDOWS)
  if(tryLock())
  {
    ReleaseMutex(m_oContext);
    return false;
  }
  else
  {
    return true;
  }
#else
  return false;
#endif
}
