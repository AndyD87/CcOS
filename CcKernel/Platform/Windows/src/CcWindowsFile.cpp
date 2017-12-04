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
 * @brief     Implementation of Class CcWindowsFile
 */
#include "CcWindowsFile.h"
#include "CcKernel.h"
#include "CcStringUtil.h"
#include "CcWString.h"
#include "CcDirectory.h"
#include "Shellapi.h"

CcWindowsFile::CcWindowsFile(const CcString& path):
  m_hFile(INVALID_HANDLE_VALUE)
{
  if (path.at(0) == '/')
    m_sPath = path.substr(1).getOsPath().getWString();
  else
    m_sPath = path.getOsPath().getWString();
}

CcWindowsFile::~CcWindowsFile( void )
{
}

size_t CcWindowsFile::read(void* buffer, size_t size)
{
  DWORD dwByteRead;
  if (ReadFile(
    m_hFile,           // open file handle
    buffer,      // start of data to write
    (DWORD)size,  // number of bytes to write
    &dwByteRead, // number of bytes that were written
    nullptr))            // no overlapped structure
    return dwByteRead;
  else{
    return SIZE_MAX;
  }
}

size_t CcWindowsFile::write(const void* buffer, size_t size)
{
  DWORD dwBytesWritten;
  if(!WriteFile(
          m_hFile,           // open file handle
          buffer,      // start of data to write
          (DWORD)size,  // number of bytes to write
          &dwBytesWritten, // number of bytes that were written
          nullptr))            // no overlapped structure
  {
    CCDEBUG("WriteFile: " + CcString::fromNumber(GetLastError()));
  }
  return dwBytesWritten;
}

size_t CcWindowsFile::size(void)
{
  uint64 uiSize = size64();
  return static_cast<size_t>(uiSize);
}

uint64 CcWindowsFile::size64(void)
{
  uint64 uiSize = 0;
  WIN32_FILE_ATTRIBUTE_DATA fileAttr;

  if (GetFileAttributesExW(m_sPath.getWcharString(), GetFileExInfoStandard, (void*) &fileAttr))
  {
    uiSize = fileAttr.nFileSizeHigh;
    uiSize = uiSize << 32;
    uiSize += fileAttr.nFileSizeLow;
  }
  return uiSize;
}

CcStatus CcWindowsFile::open(EOpenFlags flags)
{
  bool bRet(true);
  DWORD AccessMode   = 0;
  DWORD ShareingMode = 0;
  DWORD CreateNew    = 0;
  DWORD Attributes   = 0;
  if (IS_FLAG_SET(flags, EOpenFlags::Read))
  {
    AccessMode |= GENERIC_READ;
    CreateNew  |= OPEN_EXISTING;
  }
  if (IS_FLAG_SET(flags, EOpenFlags::Write))
  {
    AccessMode |= GENERIC_WRITE;
    if      (IS_FLAG_SET(flags, EOpenFlags::Overwrite))
      CreateNew |= CREATE_ALWAYS;
    else if (IS_FLAG_SET(flags, EOpenFlags::Attributes))
      CreateNew |= OPEN_EXISTING;
    else
      CreateNew |= CREATE_NEW;
  }
  if (IS_FLAG_SET(flags, EOpenFlags::Append))
  {
    AccessMode |= FILE_APPEND_DATA;
    CreateNew |=  OPEN_ALWAYS;
  }
  if (IS_FLAG_SET(flags, EOpenFlags::ShareRead))
  {
    ShareingMode |= FILE_SHARE_READ;
  }
  if (IS_FLAG_SET(flags, EOpenFlags::ShareWrite))
  {
    ShareingMode |= FILE_SHARE_WRITE;
  }
  if (IS_FLAG_SET(flags, EOpenFlags::Attributes))
  {
    AccessMode |= GENERIC_READ;
    AccessMode |= GENERIC_WRITE;
    CreateNew |= OPEN_EXISTING;
    Attributes = FILE_FLAG_BACKUP_SEMANTICS;
  }
  else
  {
    Attributes = FILE_ATTRIBUTE_NORMAL;
  }
  if (bRet != false)
  {
    m_hFile = CreateFileW((wchar_t*)m_sPath.getWcharString(),                // name of the write
      AccessMode,         // open for writing
      ShareingMode,       // do not share
      nullptr,            // default security
      CreateNew,          // create new file only
      Attributes,         // normal file
      nullptr);                  // no attr. template
    if (m_hFile != INVALID_HANDLE_VALUE)
      bRet = true;
    else{
      bRet = false;
      CCDEBUG("CreateFile: " + CcString::fromNumber(GetLastError()));
    }
  }
  return bRet;
}

CcStatus CcWindowsFile::close(void)
{
  if(CloseHandle(m_hFile))
    return true;
  return false;
}

bool CcWindowsFile::isFile(void) const
{
  DWORD dwAttrib = GetFileAttributesW((wchar_t*)m_sPath.getWcharString());
  if (dwAttrib != INVALID_FILE_ATTRIBUTES &&
    !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
    return true;
  return false;
}

bool CcWindowsFile::isDir(void) const
{
  bool bRet(false);
  DWORD ubRet = GetFileAttributesW((wchar_t*)m_sPath.getWcharString());
  if (ubRet & FILE_ATTRIBUTE_DIRECTORY && ubRet != INVALID_FILE_ATTRIBUTES)
  {
    bRet = true;
  }
  return bRet;
}

CcStatus CcWindowsFile::setFilePointer(uint64 pos)
{
  bool bRet = false;
  LARGE_INTEGER uiMoveTo;
  uiMoveTo.QuadPart = pos;
  if(SetFilePointerEx(m_hFile, uiMoveTo, nullptr, FILE_BEGIN))
  {
    bRet = true;
  }
  return bRet;
}

// @TODO generate FileInfoList in FileSystem not here
CcFileInfoList CcWindowsFile::getFileList() const
{
  CcFileInfoList oRet;
  if (isDir())
  {
    WIN32_FIND_DATAW FileData;
    CcWString searchPath(m_sPath);
    searchPath.append(L"\\*", 2);
    HANDLE hDir = FindFirstFileW((wchar_t*)searchPath.getWcharString(), &FileData);
    if (hDir != INVALID_HANDLE_VALUE)
    {
      do
      {
        CcString sFilename(FileData.cFileName);
        if (sFilename != "." &&
            sFilename != "..")
        {
          CcFileInfo oFileInfo;
          oFileInfo.setFlags(EFileAttributes::GlobalRead | EFileAttributes::GlobalWrite | EFileAttributes::UserRead | EFileAttributes::UserWrite | EFileAttributes::GroupRead | EFileAttributes::GroupWrite);
          if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            oFileInfo.addFlags(EFileAttributes::Directory);
          size_t uiNameLength = wcslen(FileData.cFileName);
          oFileInfo.name().fromUnicode(FileData.cFileName, uiNameLength);
          oFileInfo.setUserId(1000);
          oFileInfo.setGroupId(1000);
          CcDateTime oDateTime;
          oDateTime.setFiletime(((int64) FileData.ftCreationTime.dwHighDateTime << 32) + FileData.ftCreationTime.dwLowDateTime);
          oFileInfo.setCreated(oDateTime);
          oDateTime.setFiletime(((int64) FileData.ftLastWriteTime.dwHighDateTime << 32) + FileData.ftLastWriteTime.dwLowDateTime);
          oFileInfo.setModified(oDateTime);
          uint64 size = FileData.nFileSizeHigh;
          size = size << 32;
          size += FileData.nFileSizeLow;
          oFileInfo.setFileSize(size);
          oRet.append(oFileInfo);
        }
      } while (FindNextFileW(hDir, &FileData));
      FindClose(hDir);
    }
  }
  return oRet;
}

CcStatus CcWindowsFile::move(const CcString& Path)
{
  if (MoveFileW(
            (wchar_t*) m_sPath.getWcharString(),
            (wchar_t*) Path.getOsPath().getWString().getWcharString()
                ))
  {
    m_sPath = Path.getOsPath();
    return true;
  }
  else
  {
    CCDEBUG("File move failed: " + CcString::fromNumber(GetLastError()));
    return false;
  }
}

CcStatus CcWindowsFile::copy(const CcString& Path)
{
  if (isFile())
  {
    if (CopyFileW(
      (wchar_t*) m_sPath.getWcharString(),
      (wchar_t*) Path.getOsPath().getWString().getWcharString(),
      FALSE))
    {
      return true;
    }
    else
    {
      CCDEBUG("File copy failed: " + CcString::fromNumber(GetLastError()));
      return false;
    }
  }
  else
  {
    return CcDirectory::copy(m_sPath.getString(), Path);
  }
}

CcFileInfo CcWindowsFile::getInfo(void) const
{
  CcFileInfo oFileInfo; 
  WIN32_FILE_ATTRIBUTE_DATA fileAttr;
  if (GetFileAttributesExW((wchar_t*)m_sPath.getWcharString(), GetFileExInfoStandard, &fileAttr))
  {
    CcDateTime oConvert;
    oConvert.setFiletime(((uint64)fileAttr.ftCreationTime.dwHighDateTime << 32) + fileAttr.ftCreationTime.dwLowDateTime);
    oFileInfo.setCreated(oConvert);
    oConvert.setFiletime(((uint64)fileAttr.ftLastWriteTime.dwHighDateTime << 32) + fileAttr.ftLastWriteTime.dwLowDateTime);
    oFileInfo.setModified(oConvert);
    oFileInfo.setUserId(1000);
    oFileInfo.setGroupId(1000);
    // @todo: implement split for Unicode String
    CcString sForName;
    sForName.fromUnicode(m_sPath);
    CcStringList slSplitPath = sForName.split(L"/");
    if(slSplitPath.size() > 0)
    { 
      oFileInfo.setName(slSplitPath.last());
    }
    oFileInfo.setFileSize(((uint64)fileAttr.nFileSizeHigh << 32) + fileAttr.nFileSizeLow);
    if (fileAttr.dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      !(fileAttr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      oFileInfo.setFlags(EFileAttributes::UserRead | EFileAttributes::UserWrite | EFileAttributes::GroupRead | EFileAttributes::GroupWrite | EFileAttributes::GlobalRead | EFileAttributes::GlobalWrite);
      oFileInfo.setIsFile(true);
    }
    else
    {
      oFileInfo.setFlags( EFileAttributes::UserExecute | EFileAttributes::UserRead | EFileAttributes::UserWrite | 
                          EFileAttributes::GroupExecute | EFileAttributes::GroupRead | EFileAttributes::GroupWrite | 
                          EFileAttributes::GlobalExecute | EFileAttributes::GlobalRead | EFileAttributes::GlobalWrite |
                          EFileAttributes::Directory);
      oFileInfo.setIsFile(false);
    }
  }
  else
  {
    CCERROR("getInfo failed with: " + CcString::fromNumber(GetLastError()));
  }
  return oFileInfo;
}

CcDateTime CcWindowsFile::getModified(void) const
{
  FILETIME winTime;
  CcDateTime tRet;
  memset(&tRet, 0, sizeof(tm));
  if (GetFileTime(m_hFile, nullptr, nullptr, &winTime))
  {
    tRet.setFiletime(((uint64) winTime.dwHighDateTime << 32) + winTime.dwLowDateTime);
  }
  return tRet;
}

CcDateTime CcWindowsFile::getCreated(void) const
{
  FILETIME winTime;
  CcDateTime tRet;
  memset(&tRet, 0, sizeof(tm));
  if (GetFileTime(m_hFile, &winTime, nullptr, nullptr))
  {
    tRet.setFiletime(((uint64) winTime.dwHighDateTime << 32) + winTime.dwLowDateTime);
  }
  return tRet;
}

CcStatus CcWindowsFile::setCreated(const CcDateTime& oDateTime)
{
  bool bRet = false;
  FILETIME winTime;
  uint64 uiFileTime = oDateTime.getFiletime();
  winTime.dwHighDateTime = (uint32) (uiFileTime >> 32);
  winTime.dwLowDateTime  = (uint32) (uiFileTime & 0xffffffff);
  if (SetFileTime(m_hFile, &winTime, nullptr, nullptr))
  {
    bRet = true;
  }
  else
  {
    CCDEBUG("File set created failed: " + m_sPath.getString());
  }
  return bRet;
}

CcStatus CcWindowsFile::setModified(const CcDateTime& oDateTime)
{
  bool bRet = false;
  FILETIME winTime;
  uint64 uiFileTime = oDateTime.getFiletime();
  winTime.dwHighDateTime = (uint32) (uiFileTime >> 32);
  winTime.dwLowDateTime = (uint32) (uiFileTime & 0xffffffff);
  if (SetFileTime(m_hFile, nullptr, nullptr, &winTime))
  {
    bRet = true;
  }
  else
  {
    CCDEBUG("File set modified failed: " + m_sPath.getString());
  }
  return bRet;
}

CcStatus CcWindowsFile::setUserId(uint32 uiUserId)
{
  CCUNUSED(uiUserId);
  return false;
}

CcStatus CcWindowsFile::setGroupId(uint32 uiUserId)
{
  CCUNUSED(uiUserId);
  return false;
}

CcStatus CcWindowsFile::setAttributes(EFileAttributes uiAttributes)
{
  CCUNUSED(uiAttributes);
  return false;
}

