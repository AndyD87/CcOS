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

CcLinuxSharedMemory::CcLinuxSharedMemory(const CcString& sName, size_t uiSize) :
  m_sName(sName),
  m_uiSize(uiSize)
{
}

CcLinuxSharedMemory::~CcLinuxSharedMemory()
{
  close();
}

CcStatus CcLinuxSharedMemory::open(EOpenFlags eOpenFlags)
{
  CcStatus oStatus(false);
  // @todo key generation by sName
  int iFlags = IPC_CREAT | 0666;
  m_iId = shmget(2404, m_uiSize, iFlags);
  if(m_iId < 0)
  {
    oStatus.setSystemError(errno);
  }
  else
  {
    m_eOpenType = EOpenType::Created;
    iFlags = 0;
    if(IS_FLAG_NOT_SET(eOpenFlags,EOpenFlags::Write))
      iFlags = SHM_RDONLY;
    m_pBuffer = shmat(m_iId, nullptr,iFlags);
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
  if(m_iId >= 0)
  {
    switch(m_eOpenType)
    {
      case EOpenType::Created:
        shmctl(m_iId, IPC_RMID, 0);
        m_iId = -1;
        break;
      case EOpenType::Claimed:
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
  m_iId = shmget(2404, m_uiSize, iFlags);
  if(m_iId < 0)
  {
    oStatus.setSystemError(errno);
  }
  else
  {
    m_eOpenType = EOpenType::Claimed;
    iFlags = 0;
    if(IS_FLAG_NOT_SET(eOpenFlags,EOpenFlags::Write))
      iFlags = SHM_RDONLY;
    m_pBuffer = shmat(m_iId, nullptr,iFlags);
    oStatus = true;
  }
  return oStatus;
}

bool CcLinuxSharedMemory::exists()
{
  bool bRet = true;
  int iFlags = IPC_CREAT | IPC_EXCL;
  // @todo key generation by sName
  int m_iId = shmget(2404, 1, iFlags);
  if(m_iId >= 0)
  {
    bRet = false;
    shmctl(m_iId, IPC_RMID, 0);
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
