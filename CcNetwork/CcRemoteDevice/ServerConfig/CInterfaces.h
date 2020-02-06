/*
 * This file is part of CInterfaces.
 *
 * CInterfaces is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CInterfaces is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CInterfaces.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CInterfaces
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CInterfaces
 **/
#ifndef H_CInterfaces_H_
#define H_CInterfaces_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CWlanAccessPoint.h"
#include "CWlanClient.h"
#include "CcJson/CcJsonObject.h"
#include "CRestApi.h"
#include "CcConfig/CcConfigBinary.h"
#include "CcHttpServerConfig.h"

namespace NRemoteDeviceServerConfig
{
/**
 * @brief CInterfaces implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CInterfaces
{
public:
  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  void parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);
  size_t writeBinary(CcConfigBinary::CItem* pItem, size_t& uiMaxSize);

public:
  bool                bRestApiEnabled = true;
  CRestApi            oRestApi;
  CcHttpServerConfig  oHttpServer;
};
}

#endif // H_CInterfaces_H_
