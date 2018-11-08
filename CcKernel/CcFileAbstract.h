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
 * @page      CcKernel
 * @subpage   CcFileAbstract
 *
 * @page      CcFileAbstract
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFileAbstract
 */
#ifndef _CcFileAbstract_H_
#define _CcFileAbstract_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "CcSharedPointer.h"

enum class EFileAccess : uint8
{
  None=0,
  R=1,
  W=2,
  RW=3,
  X=4,
  RX=5,
  WX=6,
  RWX=7,
};

inline EFileAccess operator|(EFileAccess leftOp, EFileAccess rightOp)
{
  return static_cast<EFileAccess> (static_cast<uint8>(leftOp) | static_cast<uint8>(rightOp));
}

inline void operator|=(EFileAccess& leftOp, EFileAccess rightOp)
{
  leftOp =  static_cast<EFileAccess> (static_cast<uint8>(leftOp) | static_cast<uint8>(rightOp));
}

inline EFileAccess operator&(EFileAccess leftOp, EFileAccess rightOp)
{
  return static_cast<EFileAccess> (static_cast<uint8>(leftOp) & static_cast<uint8>(rightOp));
}

inline void operator&=(EFileAccess& leftOp, EFileAccess rightOp)
{
  leftOp =  static_cast<EFileAccess> (static_cast<uint8>(leftOp) & static_cast<uint8>(rightOp));
}

class CcKernelSHARED EFileAttributes
{
public:
  typedef enum
  {
    None = 0,
    GlobalExecute = 1,
    GlobalWrite   = 2,
    GlobalRead    = 4,
    GroupExecute  = 8,
    GroupWrite    = 16,
    GroupRead     = 32,
    UserExecute   = 64,
    UserWrite     = 128,
    UserRead      = 256,
    Directory     = 512,
  } EFlags;

  EFileAttributes(EFlags eFlags = None)
  {
    m_uiData = static_cast<uint16>(eFlags);
  }

  EFileAttributes(uint16 uiFlags) : m_uiData(uiFlags)
  {}

  inline bool operator==(const EFileAttributes& oToCompare) const
  {
    return m_uiData == oToCompare.m_uiData;
  }

  inline bool operator!=(const EFileAttributes& oToCompare) const
  {
    return !operator ==(oToCompare);
  }

  inline EFileAttributes operator&(const EFileAttributes& oToDo) const
  {
    return m_uiData & oToDo.m_uiData;
  }

  inline EFileAttributes operator&=(const EFileAttributes& oToDo)
  {
    m_uiData &= oToDo.m_uiData;
    return *this;
  }

  inline EFileAttributes operator|(const EFileAttributes& oToDo) const
  {
    return m_uiData | oToDo.m_uiData;
  }

  inline EFileAttributes operator|=(const EFileAttributes& oToDo)
  {
    m_uiData |= oToDo.m_uiData;
    return *this;
  }

  inline EFileAttributes operator~() const
  {
    return ~m_uiData;
  }

private:
  uint16 m_uiData;
};

inline EFileAttributes::EFlags operator~(EFileAttributes::EFlags Operator)
{
  return static_cast<EFileAttributes::EFlags>(~static_cast<uint16>(Operator));
}

inline EFileAttributes::EFlags operator|(EFileAttributes::EFlags leftOp, EFileAttributes::EFlags rightOp)
{
  return static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) | static_cast<uint16>(rightOp));
}

inline void operator|=(EFileAttributes::EFlags& leftOp, EFileAttributes::EFlags rightOp)
{
  leftOp =  static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) | static_cast<uint16>(rightOp));
}

inline EFileAttributes::EFlags operator&(EFileAttributes::EFlags leftOp, EFileAttributes::EFlags rightOp)
{
  return static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) & static_cast<uint16>(rightOp));
}

inline void operator&=(EFileAttributes::EFlags& leftOp, EFileAttributes::EFlags rightOp)
{
  leftOp = static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) & static_cast<uint16>(rightOp));
}


class CcFileAbstract;
class CcFileInfo;
class CcFileInfoList;
class CcString;
class CcDateTime;

#ifdef WIN32
template class CcKernelSHARED CcSharedPointer<CcFileAbstract>;
#endif
typedef CcSharedPointer<CcFileAbstract> CcFilePointer;

/**
 * @brief File Handler.
 */
class CcKernelSHARED CcFileAbstract : public CcIODevice {
public:
  /**
   * @brief Get Size of File
   * @return Size of File or SIZE_MAX if any Error occured
   */
  virtual size_t size(void) = 0;

  /**
   * @brief Get Size of File as 64 bit unsigned integer
   * @return Size of File or UINT64_MAX if any Error occured
   */
  virtual uint64 size64(void) = 0;

  /**
   * @brief Read an amount of bytes from File
   * @param buffer: buffer where read data gets stored
   * @param size:   size of buffer, to maximum read
   * @return Number of bytes read from file or SIZE_MAX if any Error occured
   */
  virtual size_t read(void* pBuffer, size_t uSize) = 0;

  /**
   * @brief Write an amount of bytes to file
   * @param buffer: buffer with bytes to write to file
   * @param size:   number of bytes to write to file
   * @return Number of bytes written to file or SIZE_MAX if any Error occured
   */
  virtual size_t write(const void* pBuffer, size_t uSize) = 0;

  /**
   * @brief Open File with flags for type of access to be created
   * @param flags:
   * @return true if file was successfully opend
   * @todo make flags as enum for type savety
   */
  virtual CcStatus open(EOpenFlags flags) = 0;

  /**
   * @brief Close File
   * @return true if no error occured
   */
  virtual CcStatus close() = 0;

  /**
   * @brief Set File Pointer to a specific Position in File
   * @param pos: Position to set to
   * @return true if File Pointer is set successfully
   */
  virtual CcStatus setFilePointer(uint64 pos) = 0;

  /**
   * @brief Check if stored Path is a File, and not for example a directory
   * @return true if stored Path is File
   */
  virtual bool isFile(void) const = 0;

  /**
   * @brief Check if stored Path is a Directory, and not for example file
   * @return true if stored Path is Directory
   */
  virtual bool isDir(void) const = 0;

  /**
   * @brief Move Current File to new Location and remove source
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus move(const CcString& sPath) = 0;

  /**
   * @brief Move Current File to new Location
   * @param Path: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus copy(const CcString& sPath) = 0;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getModified(void) const = 0;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getCreated(void) const = 0;

  virtual CcStatus setCreated(const CcDateTime& oDateTime) = 0;
  virtual CcStatus setModified(const CcDateTime& oDateTime) = 0;
  virtual CcStatus setUserId(uint32 uiUserId) = 0;
  virtual CcStatus setGroupId(uint32 uiGroupId) = 0;
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) = 0;

  virtual CcFileInfo getInfo() const = 0;

  /**
   * @brief Get Filelist from current Filepath
   *          If current File is not a Directory, List will be empty
   * @param showFlags
   * @return List of Files in Directory
   * @todo replace showFlags with enum
   */
  virtual CcFileInfoList getFileList() const = 0;
};

#endif /* _CcFileAbstract_H_ */
