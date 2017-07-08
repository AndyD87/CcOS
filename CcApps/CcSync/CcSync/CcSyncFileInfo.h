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
 * @page      CcSync
 * @subpage   CcSyncFileInfo
 *
 * @page      CcSyncFileInfo
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncFileInfo
 **/
#ifndef CcSyncFileInfo_H_
#define CcSyncFileInfo_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcString.h"
#include "CcByteArray.h"

class CcFileInfo;
class CcJsonObject;

#define CcSyncDirInfo CcSyncFileInfo

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncFileInfo
{
public:
  /**
   * @brief Constructor
   */
  CcSyncFileInfo()
  {}

  /**
   * @brief Destructor
   */
  ~CcSyncFileInfo( void )
  {}
  

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSyncFileInfo& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSyncFileInfo& oToCompare) const
    {return !operator==(oToCompare);}

  /**
   * @brief Compare content with System File Info Object
   * @param oToCompare: File Info to compare with
   * @return true if all required data matches, otherwise false
   */
  bool operator==(const CcFileInfo& oToCompare) const;

  /**
   * @brief Compare content with System File Info Object
   * @param oToCompare: File Info to compare with
   * @return true if all required data does not match, otherwise false
   */
  bool operator!=(const CcFileInfo& oToCompare) const
    {return !operator==(oToCompare);}

  bool fromSystemFile(const CcString& sFilePath);
  bool fromJsonObject(const CcJsonObject& sFilePath);
  CcJsonObject getJsonObject() const;
  
  inline size_t& id()
    { return m_uiId;}
  inline size_t& dirId()
    { return m_uiDirId;}
  inline CcString& name ()
    { return m_sFilename; }
  inline uint64& size()
    { return m_uiFileSize;}
  uint16& userId()
    { return m_uiUserId; }
  uint16& groupId()
    { return m_uiGroupId; }
  inline int64& modified()
    { return m_oLastModified; }
  inline CcByteArray& md5()
    { return m_oMd5; }
  inline uint32& crc()
    { return m_oCrc; }
  inline int64& changed()
    { return m_oChanged;}
  inline bool& isFile()
    { return m_bIsFile;}
  
  inline const size_t& getId() const
    { return m_uiId;}
  inline const size_t& getDirId() const
    { return m_uiDirId;}
  inline const CcString& getName () const
    { return m_sFilename; }
  inline const uint64& getSize() const
    { return m_uiFileSize;}
  uint16 getUserId() const
    { return m_uiUserId; }
  uint16 getGroupId() const
    { return m_uiGroupId; }
  inline const int64& getModified() const
    { return m_oLastModified; }
  inline const CcByteArray& getMd5() const
    { return m_oMd5; }
  inline const uint32& getCrc() const
    { return m_oCrc; }
  inline const int64& getChanged() const
    { return m_oChanged;}
  inline bool getIsFile() const
    { return m_bIsFile;}

private:
  size_t m_uiId         = 0;
  size_t m_uiDirId      = 0;
  uint64 m_uiFileSize   = 0;
  CcString m_sFilename  ;
  uint16    m_uiUserId  = 0;
  uint16    m_uiGroupId = 0;
  int64 m_oLastModified = 0;
  CcByteArray m_oMd5    ;
  uint32 m_oCrc         = 0;
  int64 m_oChanged      = 0;
  bool m_bIsFile        = true;
};

#endif /* CcSyncFileInfo_H_ */
