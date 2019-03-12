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
 * @page      CcTftp
 * @subpage   CcTftpServerWorker
 *
 * @page      CcTftpServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTftpServerWorker
 */
#ifndef _CcTftpServerWorker_H_
#define _CcTftpServerWorker_H_

#include "CcBase.h"
#include "CcTftp.h"
#include "CcUser.h"
#include "Network/CcSocket.h"
#include "ETftpServer.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "IWorker.h"
#include "CcTftpServerConfig.h"

// Forward declarations
class CcTftpServerWorkerPrivate;

/**
 * @brief Worker-Class for FTP. Each incomming Connection to Server
 *        will create this Object.
 *        It is getting automaticaly closed if connection is stopped or
 *        interrupted.
 */
class CcTftpSHARED CcTftpServerWorker : public IWorker
{
public:
  /**
   * @brief Constructor
   * @param socket: Socket opend from Server to work with.
   * @param incomeServer: Pointer to Server to access for example UserList
   */
  CcTftpServerWorker(CcByteArray* inData, CcSocket oSocket, CcTftpServerConfigHandle hServerConfig);

  /**
   * @brief Destructor
   */
  virtual ~CcTftpServerWorker();

  /**
   * @brief Main Loop for Worker, afert leaving this, the Worker is getting deleted
   */
  void run();

private: // Methods
  /**
   * @brief Parse a Command received from Client
   * @param sRequest: String with containing command and parameter
   */
  bool parseRequest(const CcString& sRequest);
  static uint16 getNewTransferId();
  void runFileUpload();
  void runFileDownload();
  void sendError(ETftpServerErrors eErrorCode);
  bool sendNextWindow();
  bool sendBlock(const CcByteArray& oData);

private:
  CcSocket     m_pSocket; //!< Socket received from Server
  CcByteArray  *m_InData = nullptr; //!< Temporary Input Buffer for operating..
  CcTftpServerConfigHandle m_hServerConfig; //!< Pointer to Server which was creating
  CcTftpServerWorkerPrivate* m_pPrivate = nullptr;
};

#endif /* _CcTftpServerWorker_H_ */
