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
 * @subpage   CcSyncClientConfig
 *
 * @page      CcSyncClientConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSyncClientConfig
 **/
#ifndef CcSyncClientConfig_H_
#define CcSyncClientConfig_H_

#include "CcBase.h"
#include "CcSync.h"
#include "CcSyncAccountConfig.h"
#include "CcXml/CcXmlFile.h"

/**
 * @brief Class impelmentation
 */
class CcSyncSHARED CcSyncClientConfig
{
public:
  /**
   * @brief Constructor
   */
  CcSyncClientConfig();

  bool readConfig(const CcString& sConfigFile);
  bool readConfigFromServer(const CcString& sConfigFile, const CcString &sBasePath);
  CcSyncAccountConfig* getAccountConfig(const CcString& sAccountname);
  CcSyncAccountConfig* getFirstAccountConfig();
  
private:
  CcXmlFile               m_oXmlFile;
  CcSyncAccountConfigList m_oAccounts;
};

#endif /* CcSyncClientConfig_H_ */
