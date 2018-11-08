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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFileInfo
 **/
#ifndef _CcFileInfo_H_
#define _CcFileInfo_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcDateTime.h"
#include "CcFileAbstract.h"

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcFileInfo {
public:
  /**
   * @brief Constructor
   */
  CcFileInfo(void );

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
  ~CcFileInfo(void );

  CcFileInfo& operator=(CcFileInfo&& oToMove);
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


  bool isDir() const;
  bool isFile() const;
  inline bool isExecutable() const
  {
    return IS_FLAG_SET(m_eAccess, EFileAccess::X);
  }

  inline bool isReadable() const
  {
    return IS_FLAG_SET(m_eAccess, EFileAccess::R);
  }

  inline bool isWritable() const
  {
    return IS_FLAG_SET(m_eAccess, EFileAccess::W);
  }


  uint64 getFileSize() const
    { return m_uiFileSize; }
  const CcString& getName() const
    { return m_sName; }
  uint32 getUserId() const
    { return m_uiUserId; }
  uint32 getGroupId() const
    { return m_uiGroupId; }
  CcDateTime getModified() const
    { return m_oLastModified; }
  CcDateTime getCreated() const
    { return m_oCreated; }

  void setIsFile(bool bIsFile);
  void setFlags(EFileAttributes uiFlags);
  void addFlags(EFileAttributes uiFlagsToAdd);
  void removeFlags(EFileAttributes uiFlagsToRome);

  CcString& name()
    {return m_sName;}

  void setName(const CcString& sFileName);
  void setUserId(uint32 uiUserId);
  void setGroupId(uint32 uiGroupId);
  void setModified(CcDateTime oTime);
  void setCreated(CcDateTime oTime);
  void setFileSize(uint64 uiFileSize);
  inline void setFileAccess(EFileAccess eFileAccess)
    { m_eAccess = eFileAccess; }

  EFileAttributes getAttributes() const
    { return m_uiFlags; }
  CcString getAttributesString() const;
  static CcString getAttributesString(EFileAttributes uiAttributes);

public:

private:
  CcString    m_sName;
  uint32      m_uiUserId;
  uint32      m_uiGroupId;
  CcDateTime  m_oCreated;
  CcDateTime  m_oLastModified;
  EFileAttributes m_uiFlags = EFileAttributes::None;
  EFileAccess m_eAccess = EFileAccess::None;
  uint64      m_uiFileSize = 0;
};

#endif /* _CcFileInfo_H_ */
