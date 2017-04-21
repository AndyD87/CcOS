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
 * @subpage   CcFileInfo
 *
 * @page      CcFileInfo
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcFileInfo
 **/
#ifndef CcFileInfo_H_
#define CcFileInfo_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcDateTime.h"

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcFileInfo {
public:
  /**
   * @brief Constructor
   */
  CcFileInfo( void );

  /**
   * @brief CopyConstructor
   */
  CcFileInfo( const CcFileInfo& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcFileInfo( CcFileInfo&& oToMove );

  /**
   * @brief Destructor
   */
  virtual ~CcFileInfo( void );

  bool isDir() const;
  bool isFile() const;
  bool isExecutable() const;
  bool isReadable() const;
  bool isWritable() const;

  uint64 getFileSize() const
    { return m_uiFileSize; }
  const CcString& getName() const
    { return m_sName; }
  uint16 getUserId() const
    { return m_uiUserId; }
  uint16 getGroupId() const
    { return m_uiGroupId; }
  CcDateTime getModified() const
    { return m_oLastModified; }
  CcDateTime getCreated() const
    { return m_oCreated; }

  void setFlags(uint16 uiFlags);
  void addFlags(uint16 uiFlagsToAdd);
  void removeFlags(uint16 uiFlagsToRome);

  CcString& name()
    {return m_sName;}

  void setName(const CcString& sFileName);
  void setUserId(uint16 uiUserId);
  void setGroupId(uint16 uiGroupId);
  void setModified(CcDateTime oTime);
  void setCreated(CcDateTime oTime);
  void setFileSize(uint64 uiFileSize);

  CcString getFlagsString() const;

  CcFileInfo& operator=(const CcFileInfo& oToCopy);
  CcFileInfo& operator=(CcFileInfo&& oToMove);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcFileInfo& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcFileInfo& oToCompare) const;

public:
  static const uint16 GlobalExecute = 1;
  static const uint16 GlobalWrite   = 2;
  static const uint16 GlobalRead    = 4;
  static const uint16 GroupExecute  = 8;
  static const uint16 GroupWrite    = 16;
  static const uint16 GroupRead     = 32;
  static const uint16 UserExecute   = 64;
  static const uint16 UserWrite     = 128;
  static const uint16 UserRead      = 256;
  static const uint16 Directory     = 512;

private:
  CcString  m_sName;
  uint16    m_uiUserId;
  uint16    m_uiGroupId;
  CcDateTime  m_oCreated;
  CcDateTime  m_oLastModified;
  uint16    m_uiFlags  = 0;
  uint64    m_uiFileSize = 0;
};

#endif /* CcFileInfo_H_ */
