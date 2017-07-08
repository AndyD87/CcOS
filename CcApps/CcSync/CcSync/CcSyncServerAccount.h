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
 * @subpage   CcSyncServerAccount
 *
 * @page      CcSyncServerAccount
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncServerAccount
 **/
#ifndef CcSyncServerAccount_H_
#define CcSyncServerAccount_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcList.h"
#include "CcString.h"
#include "CcPassword.h"

class CcXmlNode;
class CcSyncServerAccount;
class CcSyncDbClient;
class CcSyncClientConfig;

typedef class CcSyncSHARED CcList<CcSyncServerAccount> CcSyncServerAccountList;

/**
* @brief Class impelmentation
*/
class CcSyncSHARED CcSyncServerAccount
{
public:
  /**
  * @brief Constructor
  */
  CcSyncServerAccount()
    {}
  CcSyncServerAccount(CcXmlNode& pAccountNode);
  CcSyncServerAccount(const CcSyncServerAccount& oToCopy);
  CcSyncServerAccount(CcSyncServerAccount&& oToMove);
  ~CcSyncServerAccount(void);

  CcSyncServerAccount& operator=(const CcSyncServerAccount& oToCopy);
  CcSyncServerAccount& operator=(CcSyncServerAccount&& oToMove);

  inline bool isValid() const
    { return m_bIsValid; }
  inline bool isAdmin()const
    { return m_bIsAdmin; }

  CcSyncClientConfig* clientConfig();
  CcSyncDbClient* database();

  inline const CcString& getName() const
    { return m_sName; }
  inline const CcPassword& getPassword() const
    { return m_oPassword; }
  inline const CcString& getBasePath() const
    { return m_sBasePath; }
  inline void setBasePath(const CcString& sBasePath)
    { m_sBasePath = sBasePath; }

private:
  bool                  m_bIsAdmin = false;
  CcString              m_sName;
  CcPassword            m_oPassword;
  CcString              m_sBasePath;
  CcSyncClientConfig*   m_pClientConfig = nullptr;
  CcSyncDbClient*       m_pDatabase = nullptr;
  bool                  m_bIsValid = false;
};

#endif /* CcSyncServerAccount_H_ */
