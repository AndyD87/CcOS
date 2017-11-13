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
* @version   0.01
* @date      2016-04
* @par       Language   C++ ANSI V3
* @brief     Implementation of Class CCcLinuxSharedMemory
*/
#include "CcLinuxSharedMemory.h"

#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

enum class ELinuxSharedMemoryOpenType
{
  Closed,
  Created,
  Claimed,
};

class CcLinuxSharedMemoryPrivate
{
public:
  int iId = -1;
  ELinuxSharedMemoryOpenType eOpenType = ELinuxSharedMemoryOpenType::Closed;
  void* pBuffer = nullptr;
};

CcLinuxSharedMemory::CcLinuxSharedMemory(const CcString& sName, uint32_t uiSize) :
  m_sName(sName),
  m_uiSize(uiSize)
{
  m_pPrivate = new CcLinuxSharedMemoryPrivate;
}

CcLinuxSharedMemory::~CcLinuxSharedMemory()
{
  close();
  delete m_pPrivate;
}

CcStatus CcLinuxSharedMemory::open(EOpenFlags eOpenFlags)
{
  CcStatus oStatus(false);
  // @todo key generation by sName
  int iFlags = IPC_CREAT | 0666;
  m_pPrivate->iId = shmget(2404, m_uiSize, iFlags);
  if(m_pPrivate->iId < 0)
  {
    oStatus.setSystemError(errno);
  }
  else
  {
    m_pPrivate->eOpenType = ELinuxSharedMemoryOpenType::Created;
    iFlags = 0;
    if(IS_FLAG_NOT_SET(eOpenFlags,EOpenFlags::Write))
      iFlags = SHM_RDONLY;
    m_pPrivate->pBuffer = shmat(m_pPrivate->iId, nullptr,iFlags);
    if(m_pPrivate->pBuffer == reinterpret_cast<void*>(-1))
    {
      oStatus = false;
      m_pPrivate->pBuffer = nullptr;
      close();
    }
    else
    {
      oStatus = true;
    }
  }
  return oStatus;
}

CcStatus CcLinuxSharedMemory::close()
{
  if(m_pPrivate->pBuffer != nullptr)
  {
    shmdt(m_pPrivate->pBuffer);
    m_pPrivate->pBuffer = nullptr;
  }
  if(m_pPrivate->iId >= 0)
  {
    switch(m_pPrivate->eOpenType)
    {
      case ELinuxSharedMemoryOpenType::Created:
        shmctl(m_pPrivate->iId, IPC_RMID, 0);
        m_pPrivate->iId = -1;
        break;
      case ELinuxSharedMemoryOpenType::Claimed:
        break;
    }
  }
  return CcStatus(true);
}

CcStatus CcLinuxSharedMemory::cancel()
{
    return CcStatus(false);
}

CcStatus CcLinuxSharedMemory::claim(EOpenFlags eOpenFlags)
{
  CcStatus oStatus(false);
  // @todo key generation by sName
  int iFlags = IPC_CREAT;
  m_pPrivate->iId = shmget(2404, m_uiSize, iFlags);
  if(m_pPrivate->iId < 0)
  {
    oStatus.setSystemError(errno);
  }
  else
  {
    m_pPrivate->eOpenType = ELinuxSharedMemoryOpenType::Claimed;
    iFlags = 0;
    if(IS_FLAG_NOT_SET(eOpenFlags,EOpenFlags::Write))
      iFlags = SHM_RDONLY;
    m_pPrivate->pBuffer = shmat(m_pPrivate->iId, nullptr,iFlags);
    oStatus = true;
  }
  return oStatus;
}

bool CcLinuxSharedMemory::exists()
{
  bool bRet = true;
  int iFlags = IPC_CREAT | IPC_EXCL;
  // @todo key generation by sName
  int iId = shmget(2404, 1, iFlags);
  if(iId >= 0)
  {
    bRet = false;
    shmctl(iId, IPC_RMID, 0);
  }
  return bRet;
}

size_t CcLinuxSharedMemory::read(void* buffer, size_t size)
{
  if(m_pPrivate->pBuffer != nullptr &&
     size <= m_uiSize)
  {
    memcpy(buffer, m_pPrivate->pBuffer, size);
    return size;
  }
  else
  {
    return SIZE_MAX;
  }
}

size_t CcLinuxSharedMemory::write(const void *buffer, size_t size)
{
  if(m_pPrivate->pBuffer != nullptr &&
     size <= m_uiSize)
  {
    memcpy(m_pPrivate->pBuffer, buffer, size);
    return size;
  }
  else
  {
    return SIZE_MAX;
  }
}
