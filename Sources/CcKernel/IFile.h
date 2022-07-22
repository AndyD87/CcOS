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
 * @brief     Class IFile
 */
#pragma once

#include "CcBase.h"
#include "IIo.h"
#include "CcSharedPointer.h"

/**
 * @brief Enum for file access rights from 0=none to 7=full
 */
enum class EFileAccess : uint8
{
  None=0,
  X=1,
  W=2,
  XW=3,
  R=4,
  XR=5,
  WR=6,
  RWX=7,
};

/**
 * @brief OR EFileAccess rights
 * @param leftOp  : First elment for OR
 * @param rightOp : Second element for OR
 * @return ORed result.
 */
inline EFileAccess operator|(EFileAccess leftOp, EFileAccess rightOp)
{
  return static_cast<EFileAccess> (static_cast<uint8>(leftOp) | static_cast<uint8>(rightOp));
}

/**
 * @brief OR EFileAccess rights and safe in first operator
 * @param[in,out] leftOp  : First elment and output for OR
 * @param rightOp         : Second element for OR
 */
inline void operator|=(EFileAccess& leftOp, EFileAccess rightOp)
{
  leftOp =  static_cast<EFileAccess> (static_cast<uint8>(leftOp) | static_cast<uint8>(rightOp));
}

/**
 * @brief AND EFileAccess rights
 * @param leftOp  : First elment for AND
 * @param rightOp : Second element for AND
 * @return ANDed result.
 */
inline EFileAccess operator&(EFileAccess leftOp, EFileAccess rightOp)
{
  return static_cast<EFileAccess> (static_cast<uint8>(leftOp) & static_cast<uint8>(rightOp));
}

/**
 * @brief AND EFileAccess rights and safe in first operator
 * @param[in,out] leftOp  : First elment and output for AND
 * @param rightOp         : Second element for AND
 */
inline void operator&=(EFileAccess& leftOp, EFileAccess rightOp)
{
  leftOp =  static_cast<EFileAccess> (static_cast<uint8>(leftOp) & static_cast<uint8>(rightOp));
}

/**
 * @brief Attributes to define access rights and type of file.
 */
class CcKernelSHARED EFileAttributes
{
public:
  /**
   * @brief File access flags in global context.
   *        The rights are generated from basic EFileAccess.
   *        Additionaly information is available with directory flag as type.
   */
  enum class EFlags : uint16
  {
    None          = static_cast<uint16>(EFileAccess::None),
    GlobalExecute = static_cast<uint16>(EFileAccess::X),
    GlobalWrite   = static_cast<uint16>(EFileAccess::W),
    GlobalRead    = static_cast<uint16>(EFileAccess::R),
    GroupExecute  = static_cast<uint16>(EFileAccess::X) << 3,
    GroupWrite    = static_cast<uint16>(EFileAccess::W) << 3,
    GroupRead     = static_cast<uint16>(EFileAccess::R) << 3,
    UserExecute   = static_cast<uint16>(EFileAccess::X) << 6,
    UserWrite     = static_cast<uint16>(EFileAccess::W) << 6,
    UserRead      = static_cast<uint16>(EFileAccess::R) << 6,
    Directory     = 512,
  };

  /**
   * @brief Create attributes with flags which can be ORed.
   * @param eFlags: Flags to set.
   */
  EFileAttributes(EFlags eFlags = EFlags::None)
  {
    m_uiData = static_cast<uint16>(eFlags);
  }

  /**
   * @brief Create attributes from integer value as they could come from OS
   * @param uiFlags: Flag to directly set.
   */
  EFileAttributes(uint16 uiFlags) : m_uiData(uiFlags)
  {}

  /**
   * @brief Compare this attributes with other attributes
   * @param oToCompare: Object to compare to
   * @return True if attributes are same
   */
  inline bool operator==(const EFileAttributes& oToCompare) const
  {
    return m_uiData == oToCompare.m_uiData;
  }

  /**
   * @brief Execute compare and negotiate the result
   * @param oToCompare: Object to compare to
   * @return True if attributes are not even.
   */
  inline bool operator!=(const EFileAttributes& oToCompare) const
  {
    return !operator ==(oToCompare);
  }

  /**
   * @brief AND operation with another object.
   * @param oToDo: Object to and with this
   * @return Result of AND
   */
  inline EFileAttributes operator&(const EFileAttributes& oToDo) const
  {
    return m_uiData & oToDo.m_uiData;
  }

  /**
   * @brief AND operation with another object.
   *        Save result in this object.
   * @param oToDo: Object to AND with this
   * @return Handle to this object
   */
  inline EFileAttributes& operator&=(const EFileAttributes& oToDo)
  {
    m_uiData &= oToDo.m_uiData;
    return *this;
  }

  /**
   * @brief AND operation with another object.
   * @param oToDo: Object to and with this
   * @return Result of OR
   */
  inline EFileAttributes operator|(const EFileAttributes& oToDo) const
  {
    return m_uiData | oToDo.m_uiData;
  }

  /**
   * @brief OR operation with another object.
   *        Save result in this object.
   * @param oToDo: Object to OR with this
   * @return Handle to this object
   */
  inline EFileAttributes& operator|=(const EFileAttributes& oToDo)
  {
    m_uiData |= oToDo.m_uiData;
    return *this;
  }

  /**
   * @brief Invert all attributes
   * @return Inverted result.
   */
  inline EFileAttributes operator~() const
  {
    return ~m_uiData;
  }

private:
  uint16 m_uiData; //! Storage for all EFlags
};

/**
 * @brief Invert operatored for EFileAttributes::EFlags
 * @param Operator: Enum to invert
 * @return Result of invert operation.
 */
inline EFileAttributes::EFlags operator~(EFileAttributes::EFlags Operator)
{
  return static_cast<EFileAttributes::EFlags>(~static_cast<uint16>(Operator));
}

/**
 * @brief OR two EFlags
 * @param leftOp:   First EFlag to OR
 * @param rightOp:  Second EFlag to OR
 * @return Result of OR operation
 */
inline EFileAttributes::EFlags operator|(EFileAttributes::EFlags leftOp, EFileAttributes::EFlags rightOp)
{
  return static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) | static_cast<uint16>(rightOp));
}

/**
 * @brief OR two EFlags and save result in first
 * @param[in,out] leftOp:   First EFlag to OR, and output for result
 * @param rightOp:          Second EFlag to OR
 */
inline void operator|=(EFileAttributes::EFlags& leftOp, EFileAttributes::EFlags rightOp)
{
  leftOp =  static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) | static_cast<uint16>(rightOp));
}

/**
 * @brief AND two EFlags
 * @param leftOp:   First EFlag to AND
 * @param rightOp:  Second EFlag to AND
 * @return Result of AND operation
 */
inline EFileAttributes::EFlags operator&(EFileAttributes::EFlags leftOp, EFileAttributes::EFlags rightOp)
{
  return static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) & static_cast<uint16>(rightOp));
}

/**
 * @brief AND two EFlags and save result in first
 * @param[in,out] leftOp:   First EFlag to AND, and output for result
 * @param rightOp:          Second EFlag to AND
 */
inline void operator&=(EFileAttributes::EFlags& leftOp, EFileAttributes::EFlags rightOp)
{
  leftOp = static_cast<EFileAttributes::EFlags> (static_cast<uint16>(leftOp) & static_cast<uint16>(rightOp));
}

/**
 * @brief AND two EFlags and save result in first
 * @param[in,out] leftOp:   First EFlag to AND, and output for result
 * @param rightOp:          Second EFlag to AND
 */
static inline EFileAttributes::EFlags operator+(EFileAttributes::EFlags leftOp, EFileAttributes::EFlags rightOp)
{
  return static_cast<EFileAttributes::EFlags>(static_cast<uint16>(leftOp) + static_cast<uint16>(rightOp));
}

/**
 * @brief AND two EFlags and save result in first
 * @param[in,out] leftOp:   First EFlag to AND, and output for result
 * @param rightOp:          Second EFlag to AND
 */
static inline EFileAttributes::EFlags operator+(EFileAttributes::EFlags leftOp, uint16 rightOp)
{
  return static_cast<EFileAttributes::EFlags>(static_cast<uint16>(leftOp) + rightOp);
}


class IFile;
class CcFileInfo;
class CcFileInfoList;
class CcString;
class CcDateTime;

#ifdef _MSC_VER
template class CcKernelSHARED CcSharedPointer<IFile>;
#endif

//! Define a file handle with shared pointers
typedef CcSharedPointer<IFile> CcFilePointer;

/**
 * @brief File Interface with all definition for implemnting file access.
 *        Some basic methods for all files are implemented too.
 */
class CcKernelSHARED IFile : public IIo
{
public:
  /**
   * @brief Get Size of File
   * @return Size of File or SIZE_MAX if any Error occured
   */
  virtual size_t size() = 0;

  /**
   * @brief Get Size of File as 64 bit unsigned integer
   * @return Size of File or UINT64_MAX if any Error occured
   */
  virtual uint64 size64() = 0;

  /**
   * @brief Read an amount of bytes from File
   * @param pBuffer: buffer where read data gets stored
   * @param uSize:   size of buffer, to maximum read
   * @return Number of bytes read from file or SIZE_MAX if any Error occured
   */
  virtual size_t read(void* pBuffer, size_t uSize) = 0;

  /**
   * @brief Write an amount of bytes to file
   * @param pBuffer: buffer with bytes to write to file
   * @param uSize:   number of bytes to write to file
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
  virtual bool isFile() const = 0;

  /**
   * @brief Check if stored Path is a Directory, and not for example file
   * @return true if stored Path is Directory
   */
  virtual bool isDir() const = 0;

  /**
   * @brief Move Current File to new Location and remove source
   * @param sPath: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus move(const CcString& sPath) = 0;

  /**
   * @brief Move Current File to new Location
   * @param sPath: Target Location
   * @return true if File was successfully moved.
   */
  virtual CcStatus copy(const CcString& sPath) = 0;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getModified() const = 0;

  /**
   * @brief Get Date of Last Modification of File
   * @return time struct with data.
   */
  virtual CcDateTime getCreated() const = 0;

  /**
   * @brief Set created timstamp to file
   * @param oDateTime: Target time to set
   * @return Success if operation succeeded.
   */
  virtual CcStatus setCreated(const CcDateTime& oDateTime) = 0;

  /**
   * @brief Set modified timstamp to file
   * @param oDateTime: Target time to set
   * @return Success if operation succeeded.
   */
  virtual CcStatus setModified(const CcDateTime& oDateTime) = 0;

  /**
   * @brief Set user id of file.
   * @param uiUserId: Traget user id to set
   * @return Success if operation succeeded.
   */
  virtual CcStatus setUserId(uint32 uiUserId) = 0;

  /**
   * @brief Set group id of file.
   * @param uiGroupId: Traget user id to set
   * @return Success if operation succeeded.
   */
  virtual CcStatus setGroupId(uint32 uiGroupId) = 0;

  /**
   * @brief Set file attributes.
   * @param uiAttributes: New attributes to assign.
   * @return Success if operation succeeded.
   */
  virtual CcStatus setAttributes(EFileAttributes uiAttributes) = 0;

  /**
   * @brief Get current location of filepointer within file.
   * @return Index of current offset.
   */
  virtual uint64 getFilePointer() const = 0;

  /**
   * @brief Get all informations from file.
   * @return Object with file info
   */
  virtual CcFileInfo getInfo() const = 0;

  /**
   * @brief Get Filelist from current Filepath
   *          If current File is not a Directory, List will be empty
   * @return List of Files in Directory
   * @todo replace showFlags with enum
   */
  virtual CcFileInfoList getFileList() const = 0;
};
