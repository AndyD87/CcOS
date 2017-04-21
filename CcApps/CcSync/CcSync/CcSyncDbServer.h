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
 * @subpage   CcSyncDbServer
 *
 * @page      CcSyncDbServer
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncDbServer
 **/
#ifndef CcSyncDbServer_H_
#define CcSyncDbServer_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcSharedPointer.h"
#include "CcSqlite.h"

class CcString;

#ifdef WIN32
template class CcSyncSHARED CcSharedPointer<CcSqlite>;
#endif

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncDbServer 
{
public:
  /**
   * @brief Constructor
   */
  CcSyncDbServer( void )
    { }

  /**
   * @brief Constructor
   */
  CcSyncDbServer( const CcString& sPath );

  /**
   * @brief CopyConstructor
   */
  CcSyncDbServer( const CcSyncDbServer& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcSyncDbServer( CcSyncDbServer&& oToMove );

  /**
   * @brief Destructor
   */
  virtual ~CcSyncDbServer( void );

  CcSyncDbServer& operator=(const CcSyncDbServer& oToCopy);
  CcSyncDbServer& operator=(CcSyncDbServer&& oToMove);
  bool operator==(const CcSyncDbServer& oToCompare) const;
  bool operator!=(const CcSyncDbServer& oToCompare) const;

  bool openDatabase(const CcString& sPath);
  bool userExistsInDatabase(const CcString& sAccountName, const CcString& sUsername);
  bool updateUser(const CcString& sAccountName, const CcString& sUsername, const CcString& sToken);
  bool insertUser(const CcString& sAccountName, const CcString& sUsername, const CcString& sToken);
  bool getUserByToken(const CcString& sToken, CcString& sAccountName, CcString& sUserName);
private: //Methods
  CcString getDbCreateUser();
  CcString getDbInsertUser(const CcString& sAccountName, const CcString& sUsername, const CcString& sToken);
  CcString getDbUpdateUser(const CcString& sAccountName, const CcString& sUsername, const CcString& sToken);
private:
  CcSharedPointer<CcSqlite> m_pDatabase;
};

#endif /* CcSyncDbServer_H_ */
