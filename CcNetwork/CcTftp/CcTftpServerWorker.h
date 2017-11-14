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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTftpServerWorker
 */
#ifndef CcTftpServerWorker_H_
#define CcTftpServerWorker_H_

#include "CcBase.h"
#include "CcTftp.h"
#include "CcUser.h"
#include "Network/CcSocket.h"
#include "ETftpServer.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcWorker.h"

// Forward declarations
class CcTftpServer;

/**
 * @brief Worker-Class for FTP. Each incomming Connection to Server
 *        will create this Object.
 *        It is getting automaticaly closed if connection is stopped or
 *        interrupted.
 */
class CcTftpSHARED CcTftpServerWorker : public CcWorker
{
public:
  /**
   * @brief Constructor
   * @param socket: Socket opend from Server to work with.
   * @param incomeServer: Pointer to Server to access for example UserList
   */
  CcTftpServerWorker(CcByteArray* inData, CcSocket *oSocket, CcTftpServer *oIncomeServer);

  /**
   * @brief Destructor
   */
  virtual ~CcTftpServerWorker();

  /**
   * @brief Main Loop for Worker, afert leaving this, the Worker is getting deleted
   */
  void run(void);

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
  bool sendBlock(const CcByteArray& oData);
private:
  static uint16 s_uiTransferId;
  uint16        m_uiTransferId;
  uint16        m_uiBlockNr = 0;
  CcSocket     *m_Socket = nullptr; //!< Socket received from Server
  CcByteArray  *m_InData = nullptr; //!< Temporary Input Buffer for operating..
  CcTftpServer *m_Server = nullptr; //!< Pointer to Server which was creating

  // Incoming Data
  size_t        m_uiBlockSize = 512;
  size_t        m_uiTimeout;
  size_t        m_uiTSize;
  CcString      m_sFileName;
  ETftpServerTransferType m_eTransferType;
};

#endif /* CcTftpServerWorker_H_ */
