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
 * @page      CcSyncRequest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncRequest
 **/
#ifndef CcSyncRequest_H_
#define CcSyncRequest_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcError.h"
#include "ESyncCommandType.h"
#include "CcJson/CcJsonObject.h"

class CcCrc32;
class CcString;
class CcUser;
class CcByteArray;
class CcSyncFileInfo;

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncRequest {
public:
  /**
   * @brief Constructor
   */
  CcSyncRequest( void );

  CcSyncRequest(ESyncCommandType eCommandType);

  /**
   * @brief CopyConstructor
   */
  CcSyncRequest( const CcSyncRequest& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcSyncRequest( CcSyncRequest&& oToMove );

  /**
   * @brief Destructor
   */
  virtual ~CcSyncRequest( void );

  CcSyncRequest& operator=(const CcSyncRequest& oToCopy);
  CcSyncRequest& operator=(CcSyncRequest&& oToMove);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSyncRequest& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSyncRequest& oToCompare) const;
  
  inline ESyncCommandType getCommandType() const
    { return m_eType; }

  bool parseData(const CcString& oData);

  CcByteArray getBinary();
  inline const CcJsonObject& getData() const
    { return m_oData; }
  CcCrc32 getCrc();
  
  inline CcJsonObject& data()
    { return m_oData; }

  inline bool hasAdditionalData() const
    { return m_bHasAdditionalData; }

  bool hasFileInfo();
  void addFileInfo(const CcSyncFileInfo& oDirectoryInfo);
  CcSyncFileInfo getFileInfo();

  void init(ESyncCommandType eCommandType);
  void setCrc(const CcCrc32& oCrc);
  void setSession(const CcString& sSession);
  void setAccountLogin(const CcString& sAccount, const CcString& sUsername, const CcString& sPassword);
  void setDirectoryCreateDirectory(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo);
  void setDirectoryRemoveDirectory(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo);
  void setDirectoryUploadFile(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo);
  void setDirectoryRemoveFile(const CcString& sDirectoryName, const CcSyncFileInfo& oFileInfo);
  void setDirectoryDownloadFile(const CcString& sDirectoryName, uint64 uiFileId);
  void setDirectoryGetDirectoryInfo(const CcString& sDirectoryName, uint64 uiDirId);
  void setDirectoryGetFileInfo(const CcString& sDirectoryName, uint64 uiFileId);
  void setDirectoryGetFileList(const CcString& sDirectoryName, uint64 uiDirId);
private:
  bool getTypeFromData();
private:
  ESyncCommandType m_eType;
  CcJsonObject m_oData;
  bool m_bHasAdditionalData = false;
};

#endif /* CcSyncRequest_H_ */
