/*
 * This file is part of CWlanClient.
 *
 * CWlanClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CWlanClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CWlanClient.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CWlanClient
 **/
#pragma once

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CcVector.h"
#include "CcJson/CcJsonObject.h"
#include "IIo.h"
#include "CcConfig/CcConfigBinary.h"

namespace NRemoteDevice
{
namespace Server
{
namespace Config
{

/**
 * @brief Wlan credentials storage
 */
class CWlanCredentials
{
public:
  CcString sSsid;         //!< Ssid to login
  CcPassword oPassword;   //!< Password to login

  /**
   * @brief Compare Ssid and password if they are same
   * @param oToCompare: Object with credentials to compare
   * @return True if they are same
   */
  bool operator==(const CWlanCredentials& oToCompare) const
  { return sSsid == oToCompare.sSsid && oPassword == oToCompare.oPassword; }

  /**
   * @brief Compare Ssid and password if they are not same
   * @param oToCompare: Object with credentials to compare
   * @return True if they are not same
   */
  inline bool operator!=(const CWlanCredentials& oToCompare) const
  { return !operator==(oToCompare); }

  /**
   * @brief Parse binary config file
   * @param pItem:      First item to search for this config
   * @param uiMaxSize:  Max size of binary left to read
   * @return Next item after this parse is done.
   */
  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);

  /**
   * @brief Write credentials to binary config stream
   * @param pStream: Target stream
   * @return Number of bytes written
   */
  size_t writeBinary(IIo& pStream);
};

#ifdef _MSC_VER
template class CcRemoteDeviceSHARED CcVector<CWlanCredentials>;
#endif
/**
 * @brief CWlanClient implementation
 */
class CcRemoteDeviceSHARED CWlanClient
{
public:
  /**
   * @brief Parse json node to get client config
   * @param rJson: Json to get config from
   */
  void parseJson(CcJsonNode& rJson);

  /**
   * @brief Write config out to Json node
   * @param rNode: Node to add data to.
   */
  void writeJson(CcJsonNode& rNode);

  /**
   * @brief Parse binary config file
   * @param pItem:      First item to search for this config
   * @param uiMaxSize:  Max size of binary left to read
   * @return Next item after this parse is done.
   */
  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);

  /**
   * @brief Write credentials to binary config stream
   * @param pStream: Target stream
   * @return Number of bytes written
   */
  size_t writeBinary(IIo& pStream);

public:
  bool bEnable = true;                           //!< Enable client if true
  bool bDhcp   = true;                           //!< If use dhcp if true
  CcVector<CWlanCredentials> oKnownAccessPoints; //!< All available credentials for login
};

}
}
}
