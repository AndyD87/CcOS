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
};

CcLinuxSharedMemory::CcLinuxSharedMemory(const CcString& sName, size_t uiSize) :
  m_sName(sName),
  m_uiSize(uiSize)
{
  CCNEW(m_pPrivate, CcLinuxSharedMemoryPrivate);
}

CcLinuxSharedMemory::~CcLinuxSharedMemory()
{
  close();
  CCDELETE(m_pPrivate);
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
    m_pBuffer = shmat(m_pPrivate->iId, nullptr,iFlags);
    if(m_pBuffer == reinterpret_cast<void*>(-1))
    {
      oStatus = false;
      m_pBuffer = nullptr;
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
  CcStatus oStatus(true);
  if(m_pBuffer != nullptr)
  {
    shmdt(m_pBuffer);
    m_pBuffer = nullptr;
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
       default:
        oStatus = false;
    }
  }
  return oStatus;
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
    m_pBuffer = shmat(m_pPrivate->iId, nullptr,iFlags);
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

size_t CcLinuxSharedMemory::read(void* pBuffer, size_t uiSize)
{
  if(m_pBuffer != nullptr &&
     uiSize <= m_uiSize)
  {
    memcpy(pBuffer, m_pBuffer, uiSize);
    return uiSize;
  }
  else
  {
    return SIZE_MAX;
  }
}

size_t CcLinuxSharedMemory::write(const void *pBuffer, size_t uSize)
{
  if(m_pBuffer != nullptr &&
     uSize <= m_uiSize)
  {
    memcpy(m_pBuffer, pBuffer, uSize);
    return uSize;
  }
  else
  {
    return SIZE_MAX;
  }
}
