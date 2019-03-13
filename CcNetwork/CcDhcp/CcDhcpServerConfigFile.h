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
 * @page      CcDhcp
 * @subpage   CcDhcpServerConfigFile
 *
 * @page      CcDhcpServerConfigFile
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpServerConfigFile
 **/
#ifndef _CcDhcpServerConfigFile_H_
#define _CcDhcpServerConfigFile_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcDhcpServerConfig.h"

class CcString;
class CcDhcpServerConfigFilePrivate;

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpServerConfigFile
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpServerConfigFile();

  /**
   * @brief Destructor
   */
  ~CcDhcpServerConfigFile();

  bool loadConfigFile(const CcString& sPath);
  inline const CcDhcpServerConfig& getConfig()
    { return m_oConfig; }

private:
  bool parseConfigData();

private:
  CcDhcpServerConfigFilePrivate* m_pPrivate = nullptr;
  CcDhcpServerConfig m_oConfig;
};
#endif /* _CcDhcpServerConfigFile_H_ */
