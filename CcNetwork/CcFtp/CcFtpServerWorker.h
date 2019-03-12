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
 * @page      CcFtp
 * @subpage   CcFtpServerWorker
 *
 * @page      CcFtpServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFtpServerWorker
 */
#ifndef _CcFtpServerWorker_H_
#define _CcFtpServerWorker_H_

#include "CcBase.h"
#include "CcFtp.h"
#include "CcUser.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "IWorker.h"

// Forward declarations
class CcFtpServer;

/**
 * @brief Worker-Class for FTP. Each incomming Connection to Server
 *        will create this Object.
 *        It is getting automaticaly closed if connection is stopped or
 *        interrupted.
 */
class CcFtpSHARED CcFtpServerWorker : public IWorker
{
public:
  /**
   * @brief Constructor
   * @param socket: Socket opend from Server to work with.
   * @param incomeServer: Pointer to Server to access for example UserList
   */
  CcFtpServerWorker(CcSocket socket, CcFtpServer *incomeServer);

  /**
   * @brief Destructor
   */
  virtual ~CcFtpServerWorker();

  /**
   * @brief Main Loop for Worker, afert leaving this, the Worker is getting deleted
   */
  void run();


private: // Methods
  /**
   * @brief Parse a Command received from Client
   * @param CommandBuf: String with containing command and parameter
   */
  void parseCommand(const CcString& sCommandLine);

  /**
   * @brief Accept an additional Dataconnection to client
   * @return true if operation was successfully
   */
  bool acceptDataConnection();

  /**
   * @brief Username is received, check List for existence
   * @param Username: Username to check for.
   */
  void doUser(const CcString& Username);

  /**
   * @brief Compare an previously received Username and Password
   *        with User-List.
   * @param Password: Password to check for.
   */
  void doPass(const CcString& Password);

private:
  /**
   * @brief State of UserLogin as enum
   */
  typedef enum{
    userNone=0,   //!< no changes was done
    userNameSet,  //!< Name was set to check for
    userLogedIn   //!< User was successfully login to Server.
  } userState;
  CcString      m_WD;           //!< Current Working Directory for Client
  CcSocket      m_Socket; //!< Socket received from Server
  CcSocket      m_DataSocket; //!< Data connection socket to Client
  CcByteArray   m_InBuf;        //!< Temporary Input Buffer for operating.
  CcString      m_Temp;         //!< Temporary String Buffer for operating.
  CcFtpServer  *m_Server        =nullptr; //!< Pointer to Server which was creating this Worker
  CcUserHandle  m_User          =nullptr; //!< Pointer to current logged in User
  userState     m_UserState;    //!< State of User-Login
  uint16        m_DataPortInc;  //!< DataPort to Client
  CcSocketAddressInfo        m_PasvAddress;       //!< IpAddress to a Pasv-Connection
  bool          m_TransferType; //!< Check wich transfertype is Ascii-Type or Binary-Type
  bool          m_Active;       //!< Indication if data-connection has to be active or passiv
};

#endif /* _CcFtpServerWorker_H_ */
