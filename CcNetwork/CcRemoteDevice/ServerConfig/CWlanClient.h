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
 * @page      CWlanClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CWlanClient
 **/
#ifndef H_CWlanClient_H_
#define H_CWlanClient_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CcVector.h"
#include "CcJson/CcJsonObject.h"
#include "IIo.h"
#include "ServerConfig/CBinaryFormat.h"

namespace NRemoteDeviceServerConfig
{

class CCredentials
{
public:
  CcString sSsid;
  CcPassword oPassword;

  bool operator==(const CCredentials& oToCompare) const
    { return sSsid == oToCompare.sSsid && oPassword == oToCompare.oPassword; }
  inline bool operator!=(const CCredentials& oToCompare) const
    { return !operator==(oToCompare); }
};

#ifdef _MSC_VER
template class CcRemoteDeviceSHARED CcVector<CCredentials>;
#endif
/**
 * @brief CWlanClient implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CWlanClient
{
public:
  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  void parseBinary(const CBinaryFormat::CItem*& pItem, size_t& uiMaxSize);
  size_t writeBinary(CBinaryFormat::CItem*& pItem, size_t& uiMaxSize);

public:
  bool bEnable = true;
  bool bDhcp   = true;
  CcVector<CCredentials> oKnownAccessPoints;
};
}

#endif // H_CWlanClient_H_
