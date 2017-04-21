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
 * @file      LinuxFile
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class LinuxFile
 */
#include "LinuxFile.h"
#include "unistd.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utime.h>
#include "dirent.h"
#include "CcKernel.h"
#include "CcStringUtil.h"
#include "CcFileInfo.h"
#include "CcFileInfoList.h"

LinuxFile::LinuxFile(const CcString &path)
{
  m_Path = path;
}

LinuxFile::~LinuxFile( void )
{
  close();
}

bool LinuxFile::createFile(void){
  return false;
}

CcFileInfo LinuxFile::getInfo() const
{
  CcFileInfo oFileInfo;
  struct stat sStat;
  int iError = stat(m_Path.getCharString(), &sStat);  
  if (iError == 0)
  {
    oFileInfo.setName(CcStringUtil::getFilenameFromPath(m_Path));    
    oFileInfo.setFlags(CcFileInfo::GlobalRead | CcFileInfo::GlobalWrite | CcFileInfo::UserRead | CcFileInfo::UserWrite | CcFileInfo::GroupRead | CcFileInfo::GroupWrite);

    CcDateTime oTimeTemp;
    oTimeTemp.setTimestampMs((sStat.st_ctim.tv_sec * 1000) + (sStat.st_ctim.tv_nsec / 1000));
    oFileInfo.setCreated(oTimeTemp);
    oTimeTemp.setTimestampMs((sStat.st_mtim.tv_sec * 1000) + (sStat.st_mtim.tv_nsec / 1000));
    oFileInfo.setModified(oTimeTemp);

    oFileInfo.setUserId(sStat.st_uid);
    oFileInfo.setGroupId(sStat.st_gid);

    oFileInfo.setFileSize(sStat.st_size);
    
    if (S_ISDIR(sStat.st_mode))
    {
      oFileInfo.addFlags(CcFileInfo::Directory);
    }
  }
  else
  {
    CCDEBUG("Unable to retrieve FileInfo from: " + m_Path);  
  }    
  return oFileInfo;
}

size_t LinuxFile::read(char* buffer, size_t size){
  ssize_t dwByteRead;
  if ((dwByteRead = ::fread(buffer, sizeof(char), size, m_hFile)) <0)
    return SIZE_MAX;
  else{
    return dwByteRead;
  }
}

size_t LinuxFile::size(void){
  struct stat st;
  if (stat(m_Path.getCharString(), &st) == 0)
      return st.st_size;
  return -1;
}

size_t LinuxFile::write(const char* buffer, size_t size){
  ssize_t dwByteWritten;
  if ((dwByteWritten = ::fwrite(buffer, sizeof(char), size, m_hFile)) <0)
    return SIZE_MAX;
  else{
    return dwByteWritten;
  }
}

bool LinuxFile::open(EOpenFlags flags)
{
  bool bRet(true);
  char flag[3];
  if (IS_FLAG_SET(flags, EOpenFlags::Read))
  {
    flag[0] = 'r';
    flag[1] = '\0';
  }
  if (IS_FLAG_SET(flags, EOpenFlags::Write))
  {
    flag[0] = 'w';
    flag[1] = '\0';
  }
  if (IS_FLAG_SET(flags, EOpenFlags::Attributes))
  {
    flag[0] = 'a';
    flag[1] = '\0';
  }
  const char* pPath = m_Path.getCharString();
  m_hFile = fopen(pPath, flag);
  if (m_hFile != 0)
    bRet = true;
  else{
    int errornr = errno;
    CCDEBUG("fopen failed: " + CcString::fromNumber(errornr));
    CCDEBUG("        File: " + m_Path);
    bRet = false;
  }
  return bRet;
}

bool LinuxFile::close(void)
{
  bool bRet = false;
  if(m_hFile != nullptr)
  {
    if(fclose(m_hFile) == 0)
    {
      m_hFile = nullptr;
      bRet = true;
    }
  }
  return bRet;
}

bool LinuxFile::isFile(void) const 
{
  struct stat sStat;
  if(0 == stat(m_Path.getCharString(), &sStat))
    if(S_ISREG(sStat.st_mode ))
       return true;
  return false;
}

bool LinuxFile::setFilePointer(size_t pos){
  bool bRet(false);
  //TODO: setting filepointer
  return bRet;
}

bool LinuxFile::isDir(void) const
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

CcFileInfoList LinuxFile::getFileList() const
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
          LinuxFile oFile (sFilePath);
          CcFileInfo oFileInfo = oFile.getInfo();                    
          slRet.append(oFileInfo);
        }
      }
      closedir(d);
    }
  }
  return slRet;
}

bool LinuxFile::move(const CcString& Path)
{
  if (rename(
                m_Path.getCharString(),
                Path.getCharString()
                ))
  {
    m_Path = Path;
    return true;
  }
  return false;
}

CcDateTime LinuxFile::getModified(void) const
{
  struct stat sStat;
  CcDateTime tRet;
  if (stat(m_Path.getCharString(), &sStat))
  {
    int64 iTempTime = (sStat.st_mtim.tv_sec * 1000) + (sStat.st_mtim.tv_nsec / 1000);
    tRet.setTimestampMs(iTempTime);
  }
  return tRet;
}

CcDateTime LinuxFile::getCreated() const
{
  struct stat sStat;
  CcDateTime tRet;
  if (stat(m_Path.getCharString(), &sStat))
  {
    int64 iTempTime = (sStat.st_ctim.tv_sec * 1000) + (sStat.st_ctim.tv_nsec / 1000);
    tRet.setTimestampMs(iTempTime);
  }
  return tRet;
}

bool LinuxFile::setCreated(const CcDateTime& oDateTime)
{
  CCDEBUG("File set modified on linux not available: " + m_Path);
  return false;
}

bool LinuxFile::setModified(const CcDateTime& oDateTime)
{
  timespec uiTimes[2];
  uiTimes[0].tv_sec = 0;
  uiTimes[0].tv_nsec = UTIME_NOW;
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

bool LinuxFile::setUserId(uint16 uiUserId)
{
  chown(m_Path.getCharString(), uiUserId, -1);
}

bool LinuxFile::setGroupId(uint16 uiGroupId)
{
  chown(m_Path.getCharString(), -1, uiGroupId);
}

bool LinuxFile::ioControl(uint32 cmd, const void *argument)
{
  bool bSuccess = false;
  int iRet = ioctl(fileno(m_hFile), cmd, argument);
  if(iRet == 0)
  {
    bSuccess = false;
  }
  else{
    CCERROR("File returned error on ioCtrontrol " + CcString::fromNumber(iRet));
  }
  return false;
}
