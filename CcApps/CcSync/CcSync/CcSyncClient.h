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
 * @page      CcSyncClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncClient
 **/
#ifndef CcSyncClient_H_
#define CcSyncClient_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcString.h"
#include "CcList.h"
#include "CcSyncClientConfig.h"
#include "CcSyncDbClient.h"
#include "CcSyncDirectory.h"
#include "Network/CcSocket.h"
#include "CcSyncRequest.h"
#include "CcSyncResponse.h"

#ifdef WIN32
template class CcSyncSHARED CcList<CcSyncDirectory>;
#endif

// Forward Declarrations
class CcSslSocket;
class CcFile;

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncClient 
{
public:
  /**
   * @brief Constructor
   */
  CcSyncClient( const CcString& sAccountName);
  
  /**
   * @brief Constructor
   */
  CcSyncClient(const CcString& sConfigFilePath, const CcString& sAccountName);

  /**
   * @brief CopyConstructor
   */
  CcSyncClient( const CcSyncClient& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcSyncClient( CcSyncClient&& oToMove );

  /**
   * @brief Destructor
   */
  virtual ~CcSyncClient(void);

  CcSyncClient& operator=(const CcSyncClient& oToCopy);
  CcSyncClient& operator=(CcSyncClient&& oToMove);

  bool login();
  bool logout();
  CcString getAccountData();
  void checkForServerUpdates();
  bool setupLocations();
  void scanDirectory(const CcString& sDirectoryName, bool bDeepScan = false);
  void scanDirectories(bool bDeepScan = false);
  void doQueue();
  void resetQueues();
  void close();
  bool selectAccount(const CcString& sNewAccount);

  const CcString& getAccountName()
    { return m_pAccount->getName(); }

private: // Methods
  void init(const CcString& sConfigFile, const CcString& sAccountName);
  bool setupDatabase();
  bool checkSqlTables();
  bool setupSqlTables();
  bool initSocket();
  bool sendRequestGetResponse();
  void recursiveRemoveDirectory(CcSyncDirectory& oDirectory, const CcSyncFileInfo& oFileInfo);
  bool sendFile(const CcString& sPath);
  bool receiveFile(CcFile* pFile, CcSyncFileInfo& oFileInfo);
  bool doServerUpdate(CcSyncDirectory& oDirectory, uint64 uiDirId);
  bool serverDirectoryEqual(CcSyncDirectory& oDirectory, uint64 uiDirId);
  bool doAddDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  bool doRemoveDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  bool doUpdateDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  bool doDownloadDir(CcSyncDirectory& oDirectory, CcSyncFileInfo& oDirInfo, uint64 uiQueueIndex);
  bool doAddFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  bool doRemoveFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  bool doUpdateFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);
  bool doDownloadFile(CcSyncDirectory& oDirectory, CcSyncFileInfo& oFileInfo, uint64 uiQueueIndex);

private: // Member
  CcString        m_sDatabaseFile;
  CcSyncClientConfig m_oConfig;
  CcSyncAccountConfig* m_pAccount = nullptr;
  CcSyncDbClient       m_oDatabase;
  CcList<CcSyncDirectory> m_oBackupDirectories;
  CcSslSocket*    m_pSocket = nullptr;
  CcString        m_sSession;
  CcSyncResponse  m_oResponse;
  CcSyncRequest   m_oRequest;
};

#endif /* CcSyncClient_H_ */
