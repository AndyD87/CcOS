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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CCcWindowsSharedMemory
 */

#include "CcWindowsSharedMemory.h"
#include <windows.h>

class CcWindowsSharedMemory::CPrivate
{
public:
  HANDLE hMapFile = NULL; //<! Invalid Handle Value for SharedMemory in Windows is NULL
  CcWString sMapFilePath;
};


CcWindowsSharedMemory::CcWindowsSharedMemory(const CcString& sName, size_t uiMaxSize) : m_sName(sName), m_uiSize(uiMaxSize)
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->sMapFilePath.append(L"Local\\");
  m_pPrivate->sMapFilePath.append(CcWString(sName));
}

CcWindowsSharedMemory::~CcWindowsSharedMemory()
{
  close();
  CCDELETE(m_pPrivate);
}

CcStatus CcWindowsSharedMemory::open(EOpenFlags eOpenFlags)
{
  CcStatus oStatus(false);
  DWORD uiOpenFlags = 0;
  DWORD uiFileOpenFlags = 0;
  if (IS_FLAG_SET(eOpenFlags, EOpenFlags::ReadWrite))
  {
    uiOpenFlags = PAGE_READWRITE;
    uiFileOpenFlags = FILE_MAP_ALL_ACCESS;
  }
  else if (IS_FLAG_SET(eOpenFlags, EOpenFlags::Read))
  {
    uiOpenFlags = PAGE_READONLY;
    uiFileOpenFlags = FILE_MAP_READ;
  }
  else if (IS_FLAG_SET(eOpenFlags, EOpenFlags::Write))
  {
    uiOpenFlags = PAGE_WRITECOPY;
    uiFileOpenFlags = FILE_MAP_WRITE;
  }
  else
  {
    uiOpenFlags = PAGE_READONLY;
    uiFileOpenFlags = FILE_MAP_READ;
  }
  m_pPrivate->hMapFile = CreateFileMappingW(
    INVALID_HANDLE_VALUE,    // use paging file
    NULL,                    // default security
    uiOpenFlags,          // read/write access
    0,                       // maximum object size (high-order DWORD)
    static_cast<DWORD>(m_uiSize),                // maximum object size (low-order DWORD)
    m_pPrivate->sMapFilePath.getWcharString());                 // name of mapping object
  if (m_pPrivate->hMapFile != NULL)
  {
    m_pBuffer = (LPTSTR) MapViewOfFile(m_pPrivate->hMapFile,   // handle to map object
      uiFileOpenFlags, // read/write permission
      0,
      0,
      m_uiSize);
    if (m_pBuffer != nullptr)
    {
      oStatus = true;
    }
    else
    {
      CCDEBUG("MapViewOfFile failed: " + CcString::fromNumber(GetLastError()));
      close();
    }
  }
  else
  {
  oStatus.setSystemError(GetLastError());
  }
  return oStatus;
}

CcStatus CcWindowsSharedMemory::close()
{
  if (m_pBuffer != nullptr)
  {
    UnmapViewOfFile(m_pBuffer);
    m_pBuffer = nullptr;
  }
  if (m_pPrivate->hMapFile != NULL)
  {
    CloseHandle(m_pPrivate->hMapFile);
    m_pPrivate->hMapFile = NULL;
  }
  return CcStatus(true);
}

CcStatus CcWindowsSharedMemory::cancel()
{
    return CcStatus(false);
}

CcStatus CcWindowsSharedMemory::claim(EOpenFlags eOpenFlags)
{
  CcStatus oStatus(false);
  DWORD uiFileOpenFlags = 0;
  if (IS_FLAG_SET(eOpenFlags, EOpenFlags::ReadWrite))
  {
    uiFileOpenFlags = FILE_MAP_ALL_ACCESS;
  }
  else if (IS_FLAG_SET(eOpenFlags, EOpenFlags::Read))
  {
    uiFileOpenFlags = FILE_MAP_READ;
  }
  else if (IS_FLAG_SET(eOpenFlags, EOpenFlags::Write))
  {
    uiFileOpenFlags = FILE_MAP_WRITE;
  }
  else
  {
    uiFileOpenFlags = FILE_MAP_READ;
  }
  m_pPrivate->hMapFile = OpenFileMappingW(
    uiFileOpenFlags,    // use paging file
    FALSE,
    m_pPrivate->sMapFilePath.getWcharString());                 // name of mapping object
  if (m_pPrivate->hMapFile != NULL)
  {
    m_pBuffer = (LPTSTR) MapViewOfFile(m_pPrivate->hMapFile,   // handle to map object
      uiFileOpenFlags, // read/write permission
      0,
      0,
      m_uiSize);
    if (m_pBuffer != nullptr)
    {
      oStatus = true;
    }
    else
    {
      close();
    }
  }
  return oStatus;
}

bool CcWindowsSharedMemory::exists()
{
  bool bRet = false;
  HANDLE hMapFile = OpenFileMappingW(
    FILE_MAP_ALL_ACCESS,    // use paging file
    FALSE,
    m_pPrivate->sMapFilePath.getWcharString());                 // name of mapping object
  if (hMapFile != NULL)
  {
    CloseHandle(hMapFile);
    bRet = true;
  }
  return bRet;
}

size_t CcWindowsSharedMemory::read(void* buffer, size_t size)
{
  if (size <= m_uiSize)
  {
    CopyMemory(buffer, m_pBuffer, size);
    return size;
  }
  return SIZE_MAX;
}

size_t CcWindowsSharedMemory::write(const void* buffer, size_t size)
{
  if (size <= m_uiSize)
  {
    CopyMemory(m_pBuffer, buffer, size);
    return size;
  }
  return SIZE_MAX;
}
