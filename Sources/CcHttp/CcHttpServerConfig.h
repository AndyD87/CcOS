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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpServerConfig
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcDateTime.h"
#include "CcHttpTransferEncoding.h"
#include "Network/CcSocketAddressInfo.h"
#include "Network/CcCommonPorts.h"
#include "Config/CcConfigBinary.h"

class CcJsonNode;

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpServerConfig
{
public:
  /**
   * @brief Create default config with port
   * @param uiPort: Target http port
   */
  CcHttpServerConfig(uint16 uiPort = CcCommonPorts::HTTP);

  /**
   * @brief Parse configuration by json
   * @param rJson: Json node to get config from
   */
  void parseJson(CcJsonNode& rJson);
  /**
   * @brief Write config to json node
   * @param rNode: Target nod to write data to.
   */
  void writeJson(CcJsonNode& rNode);

  /**
   * @brief Parse binary input for configuration
   * @param pItem: Item to start read at
   * @param uiMaxSize: Max size left to read from @p pItem
   * @return Next item after all config is read.
   */
  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);

  /**
   * @brief Generate binary config from current configuration.
   * @param pStream: Stream to write binary config to
   * @return Size of bytes written to pStream
   */
  size_t writeBinary(IIo& pStream);

  //! @return Get current working directory
  const CcString& getWorkingDir() const
  { return m_sWorkingDir; }
  //! @return Get current working directory
  CcString& getWorkingDir()
  { return m_sWorkingDir; }
  //! @return Get current address, this server is listening on
  const CcSocketAddressInfo& getAddressInfo() const
  { return m_oAddressInfo; }
  //! @return Get current address, this server is listening on
  CcSocketAddressInfo& getAddressInfo()
  { return m_oAddressInfo; }
  //! @return True if ssl was enabled
  bool isSslEnabled() const
  { return m_bSslEnabled; }
  //! @return Get path to ssl key
  const CcString& getSslKey()
  { return m_sSslKey; }
  //! @return Get path to ssl certificate
  const CcString& getSslCertificate()
  { return m_sSslCertificate; }
  //! @return Get timeout for receiveing or transmitting data
  const CcDateTime& getComTimeout()
  { return m_oComTimeout; }
  //! @return Maximum number of worker
  uint32 getMaxWorkerCount()
  { return m_uiMaxWorker; }
  //! @return Maximum size of packet
  uint32 getMaxTransferPacketSize()
  { return m_uiMaxTransferPacketSize; }

  //! @param oComTimeout: Set connection timeout for receiving and transmitting
  void setComTimeout(const CcDateTime& oComTimeout)
  { m_oComTimeout = oComTimeout; }
  //! @param uiMaxWorker: Sex Maximum number of workers on server
  void setMaxWorkerCount(uint32 uiMaxWorker)
  { m_uiMaxWorker = uiMaxWorker; }
  //! @param uiTransferPacketSize: Set Maximum Transfer packet size for sending and receiving
  void setMaxTransferPacketSize(uint32 uiTransferPacketSize)
  { m_uiMaxTransferPacketSize = uiTransferPacketSize; }
  //! @param sWorkingDir: Target directory to operate in.
  void setWorkingDir(const CcString& sWorkingDir)
  { m_sWorkingDir = sWorkingDir; }
  //! @param bOnOff: Set True to enable ssl.
  void setSslEnabled(bool bOnOff)
  { m_bSslEnabled = bOnOff; }
  //! @param sAddressInfo: Target address to listen on local interfaces
  void setAddressInfo(const CcSocketAddressInfo& sAddressInfo)
  { m_oAddressInfo = sAddressInfo; }
  //! @param sKey: Set ssl key for https communication
  void setSslKey(const CcString& sKey)
  { m_sSslKey = sKey; }
  //! @param sCertificate: Set ssl certificate for https communication
  void setSslCertificate(const CcString& sCertificate)
  { m_sSslCertificate = sCertificate; }
private:
  CcSocketAddressInfo     m_oAddressInfo;
  CcString                m_sWorkingDir;
  CcString                m_sSslKey = s_sDefaultSslKey;
  CcString                m_sSslCertificate = s_sDefaultSslCertificate;
  CcDateTime              m_oComTimeout = s_oDefaultTimeout;
  bool                    m_bSslEnabled = true;
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
