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
 * @subpage   CcSyncServerConfig
 *
 * @page      CcSyncServerConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncServerConfig
 **/
#ifndef CcSyncServerConfig_H_
#define CcSyncServerConfig_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcString.h"
#include "CcSyncServerAccount.h"
#include "CcSyncLocationConfig.h"
#include "CcSyncUser.h"
#include "CcXml/CcXmlFile.h"

class CcXmlNode;

/**
* @brief Class impelmentation
*/
class CcSyncSHARED CcSyncServerConfig
{
public:
  /**
  * @brief Constructor
  */
  CcSyncServerConfig(void);
  
  uint16 getPort() const
    { return m_uiPort; }
  const CcString& getSslCertFile() const
    { return m_sSslCertFile; }
  const CcString& getSslKeyFile() const
    { return m_sSslKeyFile; }
  const CcSyncServerAccountList& getAccountList() const
    {return m_oAccountList; }
  const CcSyncLocationConfigList& getLocationList() const
    {return m_oLocationList; }

  bool readConfig(const CcString& sConfigFile);
  CcSyncServerAccount* findAccount(const CcString& sAccountName) const;

  bool isValid()
    { return m_bValid; }

  CcSyncServerConfig& operator=(const CcSyncServerConfig& oToCopy);
  CcSyncServerConfig& operator=(CcSyncServerConfig&& oToMove);

private:
  bool findServerConfig(CcXmlNode& pNode);
  bool findLocationsConfig(CcXmlNode& pNode);
  bool findAccountConfigs(CcXmlNode& pNode);

private:
  bool     m_bValid = false;
  uint16   m_uiPort;
  bool     m_bSsl = true;
  bool     m_bSslRequired = true;
  CcString m_sSslCertFile;
  CcString m_sSslKeyFile;
  CcPassword m_oRootPassword;
  CcSyncLocationConfigList m_oLocationList;
  CcSyncServerAccountList  m_oAccountList;
  CcXmlFile m_oXmlFile;
};

#endif /* CcSyncServerConfig_H_ */
