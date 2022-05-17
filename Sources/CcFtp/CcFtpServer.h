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
 * @file
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcFtpServer
 */
#pragma once

#include "CcBase.h"
#include "CcFtp.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"
#include "CcApp.h"
#include "CcFtpServerWorker.h"
#include "CcUserList.h"
#include "Network/CcCommonPorts.h"

/**
 * @brief Button for GUI Applications
 */
class CcFtpSHARED CcFtpServer : public CcApp
{
public:
  /**
   * @brief Constructor
   * @param Port: Port on wich the Server should listen.
   *              Default Port is set to 21
   */
  CcFtpServer(uint16 Port = CcCommonPorts::FTP);

  /**
   * @brief Create FTP Server with sever arguments passed through
   * @param Arg: Argument List
   */
  CcFtpServer(CcStringList *Arg);

  /**
   * @brief Destructor
   */
  virtual ~CcFtpServer();

  /**
   * @brief Application callup routine for FtpServer
   * @param Arg: Argument List to pass to FtpServer
   * @return Pointer to running App. or NULL if failed
   */
  static CcApp* main(CcStringList *Arg);

  /**
   * @brief Main Loop of Application-Thread
   */
  void run() override;

  /**
   * @brief Set an externel Userlist for Login managment
   * @param UserList: List of Users wich are able to login
   */
  void setUserList(CcUserList UserList);

  /**
   * @brief Enable or Disable an Anonymous User for Ftp
   * @param bEnable: true = enable, false = disable
   */
  void setAnonymous(bool bEnable);

  /**
   * @brief Get Current UserList registered to FtpServer
   * @return reference to internal UserList
   */
  CcUserList &getUserList();

private:
  CcSocket  m_Socket; //!< Socket where Server is listen on
  uint16    m_Port;   //!< Port where Socket is listen on.
  CcUserList m_UserList; //!< List with registered User for login.
  CcUser*   m_pAnonymousUser = nullptr;
};
