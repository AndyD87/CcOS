/*
 * This file is part of CRestApi.
 *
 * CRestApi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CRestApi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CRestApi.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CRestApi
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CRestApi
 **/
#ifndef H_CRestApi_H_
#define H_CRestApi_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CWlanAccessPoint.h"
#include "CWlanClient.h"
#include "CcJson/CcJsonObject.h"
#include "CcHttpServerConfig.h"
#include "ServerConfig/CBinaryFormat.h"

namespace NRemoteDeviceServerConfig
{
/**
 * @brief CRestApi implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CRestApi
{
public:
  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  void parseBinary(const CBinaryFormat::CItem* pItem, size_t uiMaxSize);
  size_t writeBinary(CBinaryFormat::CItem* pItem, size_t& uiMaxSize);

};
}

#endif // H_CRestApi_H_
