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
 * @subpage   CcSyncServerWorker
 *
 * @page      CcSyncServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncServerWorker
 **/
#ifndef CcSyncServerWorker_H_
#define CcSyncServerWorker_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcWorker.h"
#include "CcSyncRequest.h"
#include "CcSyncResponse.h"
#include "CcSyncUser.h"

class CcSyncDirectory;
class CcSyncDirectoryConfig;
class CcSyncClientConfig;
class CcSyncServer;
class CcSyncAccount;
class CcSocket;
class CcSqlite;
class CcFile;

/**
 * @brief Class impelmentation
 */
class CcSyncServerWorker : public CcWorker
{
public:
  /**
   * @brief Constructor
   */
  CcSyncServerWorker( CcSyncServer* m_oServer, CcSocket* m_oSocket );

  /**
   * @brief Destructor
   */
  virtual ~CcSyncServerWorker( void );

private:
  void run() override;
  bool getRequest();
  bool sendResponse();
  bool loadConfigsBySessionRequest();
  bool loadConfigsBySession(const CcString& sSession);
  bool loadDirectory();
  bool sendFile(const CcString& sPath);
  bool receiveFile(CcFile* pFile, CcSyncFileInfo& oFileInfo);
  void doServerGetInfo(); 
  void doServerAccountCreate();
  void doAccountCreate();
  void doAccountLogin();
  void doAccountGetData();
  void doAccountSetData();
  void doAccountGetDirectoryList();
  void doUserGetCommandList();
  void doDirectoryGetFileList();
  void doDirectoryGetFileInfo();
  void doDirectoryGetDirectoryInfo();
  void doDirectoryCreateDirectory();
  void doDirectoryRemoveDirectory();
  void doDirectoryUploadFile();
  void doDirectoryRemoveFile();
  void doDirectoryDownloadFile();
private:
  CcSyncServer*         m_oServer   = nullptr;
  CcSocket*             m_oSocket   = nullptr;
  CcSyncUser            m_oUser;
  CcSyncDirectory*         m_pDirectory   = nullptr;
  CcSyncRequest         m_oRequest;
  CcSyncResponse        m_oResponse;
};

#endif /* CcSyncServerWorker_H_ */
