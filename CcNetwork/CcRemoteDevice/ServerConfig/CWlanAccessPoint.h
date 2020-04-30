/*
 * This file is part of CWlanAccessPoint.
 *
 * CWlanAccessPoint is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CWlanAccessPoint is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CWlanAccessPoint.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CWlanAccessPoint
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CWlanAccessPoint
 **/
#ifndef H_CWlanAccessPoint_H_
#define H_CWlanAccessPoint_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CcJson/CcJsonObject.h"
#include "IIo.h"
#include "CcConfig/CcConfigBinary.h"

namespace NRemoteDeviceServerConfig
{
/**
 * @brief CWlanAccessPoint implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CWlanAccessPoint
{
public:
  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);
  size_t writeBinary(IIo& pStream);

public:
  CcString sSsid;
  CcPassword oPassword;
  bool bEnable = true;
  bool bDhcp = true;
};
}

#endif // H_CWlanAccessPoint_H_
