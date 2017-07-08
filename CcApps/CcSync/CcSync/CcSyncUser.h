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
 * @subpage   CcSyncUser
 *
 * @page      CcSyncUser
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncUser
 **/
#ifndef CcSyncUser_H_
#define CcSyncUser_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcSyncGlobals.h"

class CcXmlNode;
class CcSyncUser;
class CcSyncDbClient;
class CcSyncClientConfig;
class CcSyncAccountConfig;

/**
* @brief Class impelmentation
*/
class CcSyncUser
{
public:
  CcSyncUser()
    {}

  CcSyncUser(const CcSyncUser& oToCopy)
    { operator=(oToCopy); }
  
  CcSyncUser(CcSyncUser&& oToMove)
    { operator=(std::move(oToMove)); }

  /**
   * @brief Constructor
   */
  CcSyncUser(CcString sToken, CcSyncClientConfig* pClientConfig, CcSyncAccountConfig* pAccountConfig, CcSyncDbClient* pDatabase, ESyncRights eRights) :
    m_sToken(sToken),
    m_pClientConfig(pClientConfig),
    m_pAccountConfig(pAccountConfig),
    m_pDatabase(pDatabase),
    m_eRights(eRights),
    m_bIsValid(true)
  {}

  ~CcSyncUser(void);

  bool isValid() const
    { return m_bIsValid; }

  const CcString& getToken() const
    { return m_sToken; }
  CcSyncAccountConfig* getAccountConfig() const
    { return m_pAccountConfig; }
  CcSyncDbClient* getDatabase() const
    { return m_pDatabase; }
  ESyncRights getRights() const
    { return m_eRights; }

  CcSyncUser& operator=(const CcSyncUser& oToCopy);
  CcSyncUser& operator=(CcSyncUser&& oToMove);
private:
  CcString                m_sToken;
  CcSyncClientConfig*     m_pClientConfig = nullptr;
  CcSyncAccountConfig*    m_pAccountConfig = nullptr;
  CcSyncDbClient*         m_pDatabase = nullptr;
  ESyncRights             m_eRights = ESyncRights::None;
  bool                    m_bIsValid = false;
};

#endif /* CcSyncUser_H_ */
