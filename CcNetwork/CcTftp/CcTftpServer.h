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
 * @subpage   CcTftpServer
 *
 * @page      CcTftpServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTftpServer
 */
#ifndef CcTftpServer_H_
#define CcTftpServer_H_

#include "CcBase.h"
#include "CcTftp.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"
#include "CcApp.h"
#include "CcTftpServerWorker.h"
#include "CcUserList.h"

/**
 * @brief Button for GUI Applications
 */
class CcTftpSHARED CcTftpServer : public CcApp
{
public:
  /**
   * @brief Constructor
   * @param Port: Port on wich the Server should listen.
   *              Default Port is set to 27521
   */
  CcTftpServer( uint16 Port = 69 );

  /**
   * @brief Create FTP Server with sever arguments passed through
   * @param Arg: Argument List
   */
  CcTftpServer(CcStringList *Arg);

  /**
   * @brief Destructor
   */
  virtual ~CcTftpServer( void );

  /**
   * @brief Application callup routine for FtpServer
   * @param Arg: Argument List to pass to FtpServer
   * @return Pointer to running App. or NULL if failed
   */
  static CcApp* main(CcStringList *Arg);

  /**
   * @brief Main Loop of Application-Thread
   */
  void run(void) override;

  const CcString& getRootDir() { return m_sRootDir; }
  void setRootDir(const CcString& sRootDir) {m_sRootDir = sRootDir; }

private:
  CcSocket   *m_Socket; //!< Socket where Server is listen on
  uint16      m_Port;   //!< Port where Socket is listen on.
  CcString    m_sRootDir; //!< Root-Dir of Server
};

#endif /* CcTftpServer_H_ */
