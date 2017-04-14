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
 * @page      CcSyncServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncServer
 **/
#ifndef CcSyncServer_H_
#define CcSyncServer_H_

#include "CcBase.h"
#include "CcUserList.h"
#include "CcSyncServerConfig.h"
#include "CcApp.h"
#include "CcSyncDbServer.h"
#include "CcSyncServerAccount.h"

class CcSslSocket;

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncServer : public CcApp{
public:
  /**
   * @brief Constructor
   */
  CcSyncServer( void );

  /**
   * @brief CopyConstructor
   */
  CcSyncServer( const CcSyncServer& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcSyncServer( CcSyncServer&& oToMove );

  /**
   * @brief Destructor
   */
  virtual ~CcSyncServer( void );

  void run() override;

  CcSyncServerConfig& config()
    { return m_oConfig; }

  CcSyncDbServer& database()
    { return m_oDatabase; }

  CcSyncServer& operator=(const CcSyncServer& oToCopy);
  CcSyncServer& operator=(CcSyncServer&& oToMove);
  CcSyncUser loginUser(const CcString& sAccount, const CcString& sUserName, const CcString& sPassword);
  CcSyncUser getUserByToken(const CcString& sToken);

private:
  bool setupDatabase();

private:
  CcString              m_sDatabaseFile;
  CcSyncServerConfig    m_oConfig;
  CcSyncDbServer        m_oDatabase;
  CcSslSocket*          m_pSocket = nullptr;
};

#endif /* CcSyncServer_H_ */
