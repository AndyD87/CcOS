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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxFile
 */

//! set define to default use 64bit operations in Linux
#define _FILE_OFFSET_BITS 64
#include "CcBaseSettings.h"
#include "CcGlobalStrings.h"
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

CcLinuxFile::CcLinuxFile(const CcString& sPath)
{
  m_sPath = sPath;
}

CcLinuxFile::~CcLinuxFile()
{
  close();
}

bool CcLinuxFile::createFile(){
  return false;
}

CcFileInfo CcLinuxFile::getInfo() const
{
  CcFileInfo oFileInfo;
  struct stat sStat;
  int iError = stat(m_sPath.getCharString(), &sStat);
  if (iError == 0)
  {
    oFileInfo.setName(CcStringUtil::getFilenameFromPath(m_sPath));

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
      oFileInfo.addFlags(EFileAttributes::EFlags::Directory);
    }
    if (sStat.st_mode & S_IRUSR)
      oFileInfo.addFlags(EFileAttributes::EFlags::UserRead);
    if (sStat.st_mode & S_IWUSR)
      oFileInfo.addFlags(EFileAttributes::EFlags::UserWrite);
    if (sStat.st_mode & S_IXUSR)
      oFileInfo.addFlags(EFileAttributes::EFlags::UserExecute);
    if (sStat.st_mode & S_IRGRP)
      oFileInfo.addFlags(EFileAttributes::EFlags::GroupRead);
    if (sStat.st_mode & S_IWGRP)
      oFileInfo.addFlags(EFileAttributes::EFlags::GroupWrite);
    if (sStat.st_mode & S_IXGRP)
      oFileInfo.addFlags(EFileAttributes::EFlags::GroupExecute);
    if (sStat.st_mode & S_IROTH)
      oFileInfo.addFlags(EFileAttributes::EFlags::GlobalRead);
    if (sStat.st_mode & S_IWOTH)
      oFileInfo.addFlags(EFileAttributes::EFlags::GlobalWrite);
    if (sStat.st_mode & S_IXOTH)
      oFileInfo.addFlags(EFileAttributes::EFlags::GlobalExecute);
    EFileAccess eAccess = (access(m_sPath.getCharString(), R_OK))?EFileAccess::R : EFileAccess::None;
    eAccess |= (access(m_sPath.getCharString(), W_OK))?EFileAccess::W : EFileAccess::None;
    eAccess |= (access(m_sPath.getCharString(), X_OK))?EFileAccess::X : EFileAccess::None;
    oFileInfo.setFileAccess(eAccess);
  }
  else
  {
    CCDEBUG("Unable to retrieve FileInfo from: " + m_sPath);
  }
  return oFileInfo;
}

size_t CcLinuxFile::read(void* pBuffer, size_t uSize)
{
  size_t dwByteRead;
  if ((dwByteRead = ::fread(pBuffer, sizeof(char), uSize, m_hFile)) > uSize)
  {
    return SIZE_MAX;
  }
  else
  {
    return dwByteRead;
  }
}

size_t CcLinuxFile::size()
{
  struct stat st;
  if (stat(m_sPath.getCharString(), &st) == 0)
  {
    return static_cast<size_t>(st.st_size);
  }
  return SIZE_MAX;
}

uint64 CcLinuxFile::size64()
{
  uint64 uiSize = 0;
  struct stat st;
  if (stat(m_sPath.getCharString(), &st) == 0)
  {
    uiSize = st.st_size;
  }
  return uiSize;
}

size_t CcLinuxFile::write(const void* pBuffer, size_t uSize)
{
  size_t dwByteWritten;
  if ((dwByteWritten = ::fwrite(pBuffer, sizeof(char), uSize, m_hFile)) > uSize)
  {
    dwByteWritten = SIZE_MAX;
  }
  return dwByteWritten;
}

CcStatus CcLinuxFile::flush()
{
  CcStatus oStatus(false);
  int iResult = fflush(m_hFile);
  if(iResult == 0)
    oStatus = true;
  return oStatus;
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
    flag[1] = '+';
  }
  const char* psPath = m_sPath.getCharString();
  m_hFile = fopen(psPath, flag);
  if (m_hFile != nullptr)
  {
    bRet = true;
  }
  else
  {
    CCDEBUG("fopen failed: " + CcString::fromNumber(errno));
    CCDEBUG("        File: " + m_sPath);
    bRet = false;
  }
  return bRet;
}

CcStatus CcLinuxFile::close()
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

bool CcLinuxFile::isFile() const
{
  struct stat sStat;
  if(0 == stat(m_sPath.getCharString(), &sStat))
    if(S_ISREG(sStat.st_mode ) || // Regular file
       S_ISBLK(sStat.st_mode ) || // Block device like partition
       S_ISCHR(sStat.st_mode ))   // Char device like video stream
       return true;
  return false;
}

CcStatus CcLinuxFile::setFilePointer(uint64 pos)
{
  bool bRet(false);
  if(0==fseek(m_hFile, static_cast<long long>(pos), SEEK_SET))
  {
    bRet = true;
  }
  return bRet;
}

bool CcLinuxFile::isDir() const
{
  struct stat sStat;
  const char *psPath = m_sPath.getCharString();
  if( 0 == stat(psPath, &sStat))
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
    DIR           *pDir;
    struct dirent *pDirEnt;
    pDir = opendir(m_sPath.getCharString());
    if (pDir)
    {
      int iDotsEscaped = 2;
      while ((pDirEnt = readdir(pDir)) != nullptr)
      {
        if (iDotsEscaped == 0 || (
            CcStringUtil::strcmp(pDirEnt->d_name, CcGlobalStrings::Seperators::Dot.getCharString()) != 0 &&
            CcStringUtil::strcmp(pDirEnt->d_name, CcGlobalStrings::Seperators::DoubleDot.getCharString()) != 0 ))
        {
          CcString sFilePath(m_sPath);
          sFilePath.appendPath(pDirEnt->d_name);
          CcFile oFile (sFilePath);
          slRet.append(oFile.getInfo());
        }
        else
        {
          iDotsEscaped--;
        }
      }
      closedir(pDir);
    }
  }
  return slRet;
}

CcStatus CcLinuxFile::move(const CcString& sPath)
{
  if (rename( m_sPath.getCharString(),
              sPath.getCharString() ) == 0)
  {
    m_sPath = sPath;
    return true;
  }
  else
  {
    CCDEBUG("File move failed: " + CcString::fromNumber(errno));
    return false;
  }
}

CcStatus CcLinuxFile::copy(const CcString& sPath)
{
  bool bSuccess = false;
  if(open(EOpenFlags::Read))
  {
    CcFile oTarget(sPath);
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
      CcByteArray oBuffer(CCOS_DEFAULT_FILE_RW_BUFFER_SIZE);
      size_t uiLastRead;
      do
      {
        uiLastRead = readArray(oBuffer, false);
        if(uiLastRead != 0 && uiLastRead <= oBuffer.size())
        {
          oTarget.write(oBuffer.getArray(), uiLastRead);
        }
      }while (uiLastRead != 0  && uiLastRead <= oBuffer.size());
      oTarget.close();
    }
    close();
  }
  return bSuccess;
}

CcDateTime CcLinuxFile::getModified() const
{
  struct stat sStat;
  CcDateTime tRet;
  if (stat(m_sPath.getCharString(), &sStat) == 0)
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
  if (stat(m_sPath.getCharString(), &sStat) == 0)
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
  CCDEBUG("File set modified on linux not available: " + m_sPath);
  return false;
}

uint64 CcLinuxFile::getFilePointer() const
{
  long int iPos = ftell(m_hFile);
  return static_cast<uint64>(iPos);
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
    CCDEBUG("File set modified failed: " + m_sPath);
  }
  return false;
}

CcStatus CcLinuxFile::setUserId(uint32 uiUserId)
{
  if(0==chown(m_sPath.getCharString(), uiUserId, TYPE_MAX(gid_t)))
    return true;
  return false;
}

CcStatus CcLinuxFile::setGroupId(uint32 uiGroupId)
{
  int iRet = chown(m_sPath.getCharString(), TYPE_MAX(uid_t), uiGroupId);
  if(0==iRet)
    return true;
  else
  {
    CCDEBUG("CcLinuxFile::setGroupId failed: " + CcString::fromNumber(errno));
    return false;
  }
}

CcStatus CcLinuxFile::ioControl(uint32 cmd, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  bool bSuccess = false;
  CCUNUSED(uiInSize);
  CCUNUSED(pOutArg);
  CCUNUSED(uiOutSize);
  CCUNUSED(puiWritten);
  int iRet = ioctl(fileno(m_hFile), cmd, pInArg);
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
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::UserRead))
    uiMode = S_IRUSR;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::UserWrite))
    uiMode = S_IWUSR;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::UserExecute))
    uiMode = S_IXUSR;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::GroupRead))
    uiMode = S_IRGRP;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::GroupWrite))
    uiMode = S_IWGRP;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::GroupExecute))
    uiMode = S_IXGRP;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::GlobalRead))
    uiMode = S_IROTH;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::GlobalWrite))
    uiMode = S_IWOTH;
  if (IS_FLAG_SET(uiAttributes, EFileAttributes::EFlags::GlobalExecute))
    uiMode = S_IXOTH;
  int iResult = chmod(m_sPath.getCharString(), uiMode);
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
