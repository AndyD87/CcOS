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
#ifndef H_CcHttpServerConfig_H_
#define H_CcHttpServerConfig_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcDateTime.h"
#include "CcHttpTransferEncoding.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/CcCommonPorts.h"
#include "CcConfig/CcConfigBinary.h"

class CcJsonNode;

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpServerConfig
{
public:
  CcHttpServerConfig(uint16 uiPort = CcCommonPorts::HTTP);

  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);
  size_t writeBinary(IIo& pStream);

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

  const CcDateTime& getComTimeout()
    { return m_oComTimeout; }
  void setComTimeout(const CcDateTime& oComTimeout)
    { m_oComTimeout = oComTimeout; }

  uint32 getMaxWorkerCount()
    { return m_uiMaxWorker; }
  void setMaxWorkerCount(uint32 uiMaxWorker)
    { m_uiMaxWorker = uiMaxWorker; }

  uint32 getMaxTransferPacketSize()
    { return m_uiMaxTransferPacketSize; }
  void setMaxTransferPacketSize(uint32 uiTransferPacketSize)
    { m_uiMaxTransferPacketSize = uiTransferPacketSize; }

private:
  CcSocketAddressInfo     m_oAddressInfo;
  CcString                m_sWorkingDir;
  CcString                m_sSslKey = s_sDefaultSslKey;
  CcString                m_sSslCertificate = s_sDefaultSslCertificate;
  CcDateTime              m_oComTimeout = s_oDefaultTimeout;
  bool                    m_bSslEnabled = false;
  CcHttpTransferEncoding  m_oDefaultEncoding;
  uint32                  m_uiMaxWorker = s_uiDefaultMaxWorker;
  uint32                  m_uiMaxTransferPacketSize = s_uiDefaultMaxTransferPacketSize;

  static const CcString s_sDefaultSslKey;
  static const CcString s_sDefaultSslCertificate;
  static const CcDateTime s_oDefaultTimeout;
  static const bool s_bDefaultSslEnabled;
  static const uint32 s_uiDefaultMaxWorker;
  static const uint32 s_uiDefaultMaxTransferPacketSize;
};

#endif // H_CcHttpServerConfig_H_
