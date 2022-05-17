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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFileInfo
 **/
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "CcString.h"
#include "CcDateTime.h"
#include "IFile.h"

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcFileInfo
{
public:
  CcFileInfo() = default;
  ~CcFileInfo() = default;
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcFileInfo)

  /**
   * @brief Move file informations from another info to this.
   *        This will not change informations on disk.
   * @param oToMove: Information values to move from
   * @return Handle to this
   */
  CcFileInfo& operator=(CcFileInfo&& oToMove);

  /**
   * @brief Copy file informations from another info to this.
   *        This will not change informations on disk.
   * @param oToCopy: Information values to copy from
   * @return Handle to this
   */
  CcFileInfo& operator=(const CcFileInfo& oToCopy);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcFileInfo& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcFileInfo& oToCompare) const;


  //! @return True if this object is a directory
  bool isDir() const;
  //! @return True if this object is a file
  bool isFile() const;
  //! @return True if this object is executable
  inline bool isExecutable() const
  { return IS_FLAG_SET(m_eAccess, EFileAccess::X); }
  //! @return True if this object is readable
  inline bool isReadable() const
  { return IS_FLAG_SET(m_eAccess, EFileAccess::R); }
  //! @return True if this object is writable
  inline bool isWritable() const
  { return IS_FLAG_SET(m_eAccess, EFileAccess::W); }

  //! @return Get file of size as uint64 value
  uint64 getFileSize() const
  { return m_uiFileSize; }
  //! @return Get name of file
  const CcString& getName() const
  { return m_sName; }
  //! @return Get user id of file
  uint32 getUserId() const
  { return m_uiUserId; }
  //! @return Get group id of file
  uint32 getGroupId() const
  { return m_uiGroupId; }
  //! @return Get modified timestamp of file
  CcDateTime getModified() const
  { return m_oLastModified; }
  //! @return Get created timestamp of file
  CcDateTime getCreated() const
   { return m_oCreated; }
  //! @return Get current file attributes
  EFileAttributes getAttributes() const
    { return m_uiFlags; }
  //! @return Get attributes as string depending on current OS
  CcString getAttributesString() const;
  //! @return Get attributes as string from specified enum
  //! @param uiAttributes: Enum to generate string from
  static CcString getAttributesString(EFileAttributes uiAttributes);
  //! @return Get editable file name
  CcString& name()
  { return m_sName; }

  //! @param bIsFile: Set value of is file to true
  void setIsFile(bool bIsFile);
  //! @param uiFlags: Set new set of EFileAttributes flags
  void setFlags(EFileAttributes uiFlags);
  //! @param uiFlagsToAdd: Add additional set of EFileAttributes flags
  void addFlags(EFileAttributes uiFlagsToAdd);
  //! @param uiFlagsToRome: Remove specific EFileAttributes flags
  void removeFlags(EFileAttributes uiFlagsToRome);

  //! @param sFileName: Update file name to new one
  void setName(const CcString& sFileName);
  //! @param uiUserId: Set new user id
  void setUserId(uint32 uiUserId);
  //! @param uiGroupId: Set new group id
  void setGroupId(uint32 uiGroupId);
  //! @param oTime: Set new modified time
  void setModified(CcDateTime oTime);
  //! @param oTime: Set new created time
  void setCreated(CcDateTime oTime);
  //! @param uiFileSize: Set new file size
  void setFileSize(uint64 uiFileSize);
  //! @param eFileAccess: Set new FileAccess attributes
  inline void setFileAccess(EFileAccess eFileAccess)
  { m_eAccess = eFileAccess; }

public:

private:
  CcString    m_sName;
  uint32      m_uiUserId = 0;
  uint32      m_uiGroupId = 0;
  CcDateTime  m_oCreated;
  CcDateTime  m_oLastModified;
  EFileAttributes m_uiFlags = EFileAttributes::EFlags::None;
  EFileAccess m_eAccess     = EFileAccess::None;
  uint64      m_uiFileSize = 0;
};
