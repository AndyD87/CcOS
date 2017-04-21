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
 * @subpage   CcSyncDirectory
 *
 * @page      CcSyncDirectory
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncDirectory
 **/
#ifndef CcSyncDirectory_H_
#define CcSyncDirectory_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcSyncDirectoryConfig.h"

// forward declarations
class CcDateTime;
class CcFileInfo;
class CcSqlite;
class CcSyncFileInfo;
class CcSyncFileInfoList;
class CcString;
class CcSyncDbClient;
enum class EBackupQueueType : uint16;

/**
 * @brief Class impelmentation
 */
class CcSyncDirectory {
public:

  CcSyncDirectory(void) :
    m_pDatabase(nullptr),
    m_pConfig(nullptr)
    {}

  /**
   * @brief Constructor
   */
  CcSyncDirectory( CcSyncDbClient* oDatabase, CcSyncDirectoryConfig* oConfig);
  
  /**
   * @brief Constructor
   */
  CcSyncDirectory( const CcSyncDirectory& oToCopy);

  /**
   * @brief Destructor
   */
  virtual ~CcSyncDirectory( void );

  CcSyncDirectory& operator=(const CcSyncDirectory& oToCopy);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcSyncDirectory oToCompare) const
    { return ((&m_pDatabase == &oToCompare.m_pDatabase && &m_pConfig == &oToCompare.m_pConfig));}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcSyncDirectory oToCompare) const
    { return !operator==(oToCompare); }

  void scan(bool bDeepSearch);
  bool validate();

  bool queueHasItems();
  EBackupQueueType queueGetNext(CcSyncFileInfo& oFileInfo, uint64& uiQueueIndex);
  void queueFinalizeDirectory(CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  void queueFinalizeFile(uint64 uiQueueIndex);
  void queueIncrementItem(uint64 uiQueueIndex);
  void queueReset();
  void queueResetAttempts();
  void queueDownloadDirectory(const CcSyncFileInfo& oFileInfo);
  void queueDownloadFile(const CcSyncFileInfo& oFileInfo);
  void queueUpdateFile(const CcSyncFileInfo& oFileInfo);

  inline const CcString& getName()
    { return m_pConfig->getName(); }
  CcString getInnerPathById(uint64 uiDirId);
  CcString getFullDirPathById(uint64 uiDirId);
  CcString getFullFilePathById(uint64 uiFileId);

  bool fileListInsert(const CcSyncFileInfo& oFileInfo);
  bool fileListUpdate(const CcSyncFileInfo& oFileInfo);
  bool fileListRemove(const CcSyncFileInfo& oFileInfo, bool bDoUpdateParents = true);
  bool fileListRemoveWithHistory(CcSyncFileInfo& oFileInfo);
  bool fileListCreate(CcSyncFileInfo& oFileInfo);
  bool fileListExists(uint64 uiFileId);

  bool directoryListRemove(const CcSyncFileInfo& oFileInfo, bool bDoUpdateParents = true);
  bool directoryCreate(CcSyncFileInfo& oFileInfo);
  bool directoryUpdate(const CcSyncFileInfo& oFileInfo);
  bool directoryListRemoveWithHistory(CcSyncFileInfo& oFileInfo);
  bool directoryExists(uint64 uiDirectoryId); 
  bool directorySubDirExists(uint64 uiParentDirId, const CcString& sName);
  bool directoryEmpty(uint64 uiDirectoryId);
  bool directoryInsert(const CcSyncFileInfo& oFileInfo);
  void directoryUpdateChanged(uint64 uiDirId);

  bool fileInDirExists(uint64 uiDirectoryId, const CcSyncFileInfo& oFileInfo);
  const CcString& getLocation() const
    { return m_pConfig->getLocation(); }
  const CcString& getName() const
    { return m_pConfig->getName(); }
  CcSyncFileInfoList getDirectoryInfoListById(size_t uiDirId);
  CcSyncFileInfoList getFileInfoListById(size_t uiDirId);
  CcSyncFileInfo getDirectoryInfoById(size_t uiDirId);
  CcSyncFileInfo getDirectoryInfoFromSubdir(size_t uiDirId, const CcString& sSubDirName);
  CcSyncFileInfo getFileInfoById(size_t uiFileId);
  CcSyncFileInfo getFileInfoByFilename(size_t uiDirId, const CcString& sFileName);
  CcString getTemporaryDir();
  CcString getHistoryDir();

private: // methods
  void scanSubDir(uint64 uiDbIndex, const CcString& sPath, bool bDeepSearch);
  void queueAddDir(uint64 uiDependent, uint64 uiQueueId, const CcString& sParentPath, const CcFileInfo& oFileInfo);
  void queueUpdateDir(const CcSyncFileInfo& oFileInfo);
  uint64 queueRemoveDir(uint64 uiDependent, const CcSyncFileInfo& oFileInfo);
  uint64 queueRemoveFile(uint64 uiDependent, const CcSyncFileInfo& oFileInfo);
  void queueAddFile(uint64 uiDependent, uint64 uiDirectoryId, const CcFileInfo& oDirectoryInfo);
  bool historyInsert(EBackupQueueType eQueueType, const CcSyncFileInfo& oFileInfo);

private:
  CcSyncDbClient*         m_pDatabase = nullptr;
  CcSyncDirectoryConfig*  m_pConfig   = nullptr;
  uint64 m_uiRootId = 1;
};

#endif /* CcSyncDirectory_H_ */
