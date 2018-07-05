/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxFile
 */

// set define to default use 64bit operations in Linux
#define _FILE_OFFSET_BITS 64
#include "CcLinuxFile.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utime.h>
#include <dirent.h>
#include <errno.h>
#include "CcKernel.h"
#include "CcStringUtil.h"
#include "CcFileInfo.h"
#include "CcFileInfoList.h"

CcLinuxFile::CcLinuxFile(const CcString &path)
{
  m_Path = path;
}

CcLinuxFile::~CcLinuxFile(void )
{
  close();
}

bool CcLinuxFile::createFile(void){
  return false;
}

CcFileInfo CcLinuxFile::getInfo() const
{
  CcFileInfo oFileInfo;
  struct stat sStat;
  int iError = stat(m_Path.getCharString(), &sStat);  
  if (iError == 0)
  {
    oFileInfo.setName(CcStringUtil::getFilenameFromPath(m_Path));

    CcDateTime oTimeTemp;
    oTimeTemp.setTimestampUs((static_cast<int64>(sStat.st_ctim.tv_sec) * 1000000) + (static_cast<int64>(sStat.st_ctim.tv_nsec) / 1000));
    oFileInfo.setCreated(oTimeTemp);
    oTimeTemp.setTimestampUs((static_cast<int64>(sStat.st_mtim.tv_sec) * 1000000) + (static_cast<int64>(sStat.st_mtim.tv_nsec) / 1000));
    oFileInfo.setModified(oTimeTemp);

    oFileInfo.setUserId(sStat.st_uid);
    oFileInfo.setGroupId(sStat.st_gid);

    oFileInfo.setFileSize(sStat.st_size);
    
    if (S_ISDIR(sStat.st_mode))
    {
      oFileInfo.addFlags(EFileAttributes::Directory);
    }
    if (sStat.st_mode & S_IRUSR)
      oFileInfo.addFlags(EFileAttributes::UserRead);
    if (sStat.st_mode & S_IWUSR)
      oFileInfo.addFlags(EFileAttributes::UserWrite);
    if (sStat.st_mode & S_IXUSR)
      oFileInfo.addFlags(EFileAttributes::UserExecute);
    if (sStat.st_mode & S_IRGRP)
      oFileInfo.addFlags(EFileAttributes::GroupRead);
    if (sStat.st_mode & S_IWGRP)
      oFileInfo.addFlags(EFileAttributes::GroupWrite);
    if (sStat.st_mode & S_IXGRP)
      oFileInfo.addFlags(EFileAttributes::GroupExecute);
    if (sStat.st_mode & S_IROTH)
      oFileInfo.addFlags(EFileAttributes::GlobalRead);
    if (sStat.st_mode & S_IWOTH)
      oFileInfo.addFlags(EFileAttributes::GlobalWrite);
    if (sStat.st_mode & S_IXOTH)
      oFileInfo.addFlags(EFileAttributes::GlobalExecute);
  }
  else
  {
    CCDEBUG("Unable to retrieve FileInfo from: " + m_Path);  
  }    
  return oFileInfo;
}

size_t CcLinuxFile::read(void* pBuffer, size_t uSize)
{
  ssize_t dwByteRead;
  if ((dwByteRead = ::fread(pBuffer, sizeof(char), uSize, m_hFile)) <0)
  {
    return SIZE_MAX;
  }
  else
  {
    return dwByteRead;
  }
}

size_t CcLinuxFile::size(void)
{
  struct stat st;
  if (stat(m_Path.getCharString(), &st) == 0)
  {
    return static_cast<size_t>(st.st_size);
  }
  return SIZE_MAX;
}

uint64 CcLinuxFile::size64(void)
{
  uint64 uiSize = 0;
  struct stat st;
  if (stat(m_Path.getCharString(), &st) == 0)
  {
    uiSize = st.st_size;
  }
  return uiSize;
}

size_t CcLinuxFile::write(const void* pBuffer, size_t uSize){
  ssize_t dwByteWritten;
  if ((dwByteWritten = ::fwrite(pBuffer, sizeof(char), uSize, m_hFile)) <0)
    return SIZE_MAX;
  else{
    return dwByteWritten;
  }
}

CcStatus CcLinuxFile::open(EOpenFlags flags)
{
  CcStatus bRet(true);
  char flag[3];
  if (IS_FLAG_SET(flags, EOpenFlags::Read) && IS_FLAG_SET(flags, EOpenFlags::Write))
  {
    flag[0] = 'r';
    flag[1] = '+';
  }
  else if (IS_FLAG_SET(flags, EOpenFlags::Read))
  {
    flag[0] = 'r';
    flag[1] = '\0';
  }
  else if (IS_FLAG_SET(flags, EOpenFlags::Attributes))
  {
    flag[0] = 'a';
    flag[1] = '\0';
    if(isDir())
      return true;
  }
  else if (IS_FLAG_SET(flags, EOpenFlags::Write))
  {
    flag[0] = 'w';
    flag[1] = '\0';
  }
  else if (IS_FLAG_SET(flags, EOpenFlags::Append))
  {
    flag[0] = 'a';
    flag[1] = '\0';
  }
  const char* pPath = m_Path.getCharString();
  m_hFile = fopen(pPath, flag);
  if (m_hFile != 0)
  {
    bRet = true;
  }
  else
  {
    CCDEBUG("fopen failed: " + CcString::fromNumber(errno));
    CCDEBUG("        File: " + m_Path);
    bRet = false;
  }
  return bRet;
}

CcStatus CcLinuxFile::close(void)
{
  bool bRet = true;
  if(m_hFile != nullptr)
  {
    if(fclose(m_hFile) == 0)
    {
      m_hFile = nullptr;
      bRet = true;
    }
    else
    {
      bRet = false;
    }
  }
  return bRet;
}

bool CcLinuxFile::isFile(void) const 
{
  struct stat sStat;
  if(0 == stat(m_Path.getCharString(), &sStat))
    if(S_ISREG(sStat.st_mode ))
       return true;
  return false;
}

CcStatus CcLinuxFile::setFilePointer(uint64 pos)
{
  bool bRet(false);
  if(0==fseek(m_hFile, pos,SEEK_SET))
  {
    bRet = true;
  }
  return bRet;
}

bool CcLinuxFile::isDir(void) const
{
  struct stat sStat;
  const char *pPath = m_Path.getCharString();
  if( 0 == stat(pPath, &sStat))
  {
    if(S_ISDIR(sStat.st_mode ))
      return true;
  }
  return false;
}

CcFileInfoList CcLinuxFile::getFileList() const
{
  CcFileInfoList slRet;
  if (isDir())
  {
    DIR           *d;
    struct dirent *dir;
    d = opendir(m_Path.getCharString());
    if (d)
    {
      while ((dir = readdir(d)) != NULL)
      {
        CcString sFilename(dir->d_name);
        if (sFilename != "." &&
            sFilename != "..")
        {
          CcString sFilePath(m_Path);
          sFilePath.appendPath(sFilename);
          CcLinuxFile oFile (sFilePath);
          CcFileInfo oFileInfo = oFile.getInfo();                    
          slRet.append(oFileInfo);
        }
      }
      closedir(d);
    }
  }
  return slRet;
}

CcStatus CcLinuxFile::move(const CcString& Path)
{
  if (rename( m_Path.getCharString(),
              Path.getCharString() ) == 0)
  {
    m_Path = Path;
    return true;
  }
  else
  {
    CCDEBUG("File move failed: " + CcString::fromNumber(errno));
    return false;
  }
}

CcStatus CcLinuxFile::copy(const CcString& Path)
{
  bool bSuccess = false;
  if(open(EOpenFlags::Read))
  {
    CcFile oTarget(Path);
    if(oTarget.exists())
    {
      bSuccess = oTarget.open(EOpenFlags::Overwrite);
    }
    else
    {
      bSuccess = oTarget.open(EOpenFlags::Write);
    }
    if(bSuccess)
    {
      CcByteArray oBuffer(1024);
      size_t uiLastRead;
      do
      {
        uiLastRead = readArray(oBuffer, false);
        oTarget.write(oBuffer.getArray(), uiLastRead);
      }while (uiLastRead > 0);
      oTarget.close();
    }
    close();
  }
  return bSuccess;
}

CcDateTime CcLinuxFile::getModified(void) const
{
  struct stat sStat;
  CcDateTime tRet;
  if (stat(m_Path.getCharString(), &sStat) == 0)
  {
    int64 iTempTime = (static_cast<int64>(sStat.st_mtim.tv_sec) * 1000000) + (static_cast<int64>(sStat.st_mtim.tv_nsec) / 1000);
    tRet.setTimestampUs(iTempTime);
  }
  else
  {
    CCDEBUG("Failed to set modified: " + CcString::fromNumber(errno));
  }
  return tRet;
}

CcDateTime CcLinuxFile::getCreated() const
{
  struct stat sStat;
  CcDateTime tRet;
  if (stat(m_Path.getCharString(), &sStat) == 0)
  {
    int64 iTempTime = (static_cast<int64>(sStat.st_ctim.tv_sec) * 1000000) + (static_cast<int64>(sStat.st_ctim.tv_nsec) / 1000);
    tRet.setTimestampUs(iTempTime);
  }
  else
  {
    CCDEBUG("Failed to set modified: " + CcString::fromNumber(errno));
  }
  return tRet;
}

CcStatus CcLinuxFile::setCreated(const CcDateTime& oDateTime)
{
  CCUNUSED(oDateTime);
  CCDEBUG("File set modified on linux not available: " + m_Path);
  return false;
}

CcStatus CcLinuxFile::setModified(const CcDateTime& oDateTime)
{
  timespec uiTimes[2];
  uiTimes[0].tv_sec = 0;
  uiTimes[0].tv_nsec = UTIME_OMIT;
  uiTimes[1].tv_sec = oDateTime.getTimestampS();
  uiTimes[1].tv_nsec = (oDateTime.getTimestampUs() % 1000000) * 1000;
  if(futimens(fileno(m_hFile), uiTimes) == 0)
  {
    return true;
  }
  else
  {
    CCDEBUG("File set modified failed: " + m_Path);
  } 
  return false;
}

CcStatus CcLinuxFile::setUserId(uint32 uiUserId)
{
  if(0==chown(m_Path.getCharString(), uiUserId, -1))
    return true;
  return false;
}

CcStatus CcLinuxFile::setGroupId(uint32 uiGroupId)
{
  int iRet = chown(m_Path.getCharString(), -1, uiGroupId);
  if(0==iRet)
    return true;
  else
  {
    CCDEBUG("CcLinuxFile::setGroupId failed: " + CcString::fromNumber(errno));
    return false;
  }
}

CcStatus CcLinuxFile::ioControl(uint32 cmd, const void *argument)
{
  bool bSuccess = false;
  int iRet = ioctl(fileno(m_hFile), cmd, argument);
  if(iRet == 0)
  {
    bSuccess = true;
  }
  else
  {
    CCERROR("File returned error on ioCtrontrol " + CcString::fromNumber(iRet));
  }
  return bSuccess;
}

CcStatus CcLinuxFile::setAttributes(EFileAttributes uiAttributes)
{
  mode_t uiMode = 0;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::UserRead))
    uiMode = S_IRUSR;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::UserWrite))
    uiMode = S_IWUSR;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::UserExecute))
    uiMode = S_IXUSR;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::GroupRead))
    uiMode = S_IRGRP;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::GroupWrite))
    uiMode = S_IWGRP;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::GroupExecute))
    uiMode = S_IXGRP;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::GlobalRead))
    uiMode = S_IROTH;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::GlobalWrite))
    uiMode = S_IWOTH;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::GlobalExecute))
    uiMode = S_IXOTH;
  int iResult = chmod(m_Path.getCharString(), uiMode);
  if (iResult == 0)
  {
    return true;
  }
  else
  {
    CCDEBUG("File set atributes failed: " + CcString::fromNumber(iResult));
    return false;
  }
}

