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
 * @page      CcHttp
 * @subpage   CcHttpServerConfig
 *
 * @page      CcHttpServerConfig
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpServerConfig
 */
#ifndef _CcHttpServerConfig_H_
#define _CcHttpServerConfig_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/CcCommonPorts.h"

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpServerConfig
{
public:
  CcHttpServerConfig(uint16 uiPort = CcCommonPorts::HTTP);

  void setWorkingDir(const CcString& sWorkingDir)
    { m_sWorkingDir = sWorkingDir; }
  const CcString& getWorkingDir() const
    { return m_sWorkingDir; }
  CcString& getWorkingDir()
    { return m_sWorkingDir; }

  void setAddressInfo(const CcSocketAddressInfo& sAddressInfo)
    { m_oAddressInfo = sAddressInfo; }
  const CcSocketAddressInfo& getAddressInfo() const
    { return m_oAddressInfo; }
  CcSocketAddressInfo& getAddressInfo()
    { return m_oAddressInfo; }
  
  void setSslEnabled(bool bOnOff)
    { m_bSslEnabled = bOnOff; }
  bool isSslEnabled() const
    { return m_bSslEnabled; }

  void setSslKey(const CcString& sKey)
    { m_sSslKey = sKey; }
  void setSslCertificate(const CcString& sCertificate)
    { m_sSslCertificate = sCertificate; }
  const CcString& getSslKey()
    { return m_sSslKey; }
  const CcString& getSslCertificate()
    { return m_sSslCertificate; }

private:
  CcSocketAddressInfo   m_oAddressInfo;
  CcString              m_sWorkingDir;
  CcString              m_sSslKey = "Key.crt";
  CcString              m_sSslCertificate = "Certificate.crt";
  bool                  m_bSslEnabled = false;
};

#endif /* _CcHttpServerConfig_H_ */
