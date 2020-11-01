/*
 * This file is part of CStartup.
 *
 * CStartup is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CStartup is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CStartup.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CStartup
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CStartup
 **/
#ifndef H_CStartup_H_
#define H_CStartup_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"
#include "CcJson/CcJsonObject.h"
#include "IIo.h"
#include "CcConfig/CcConfigBinary.h"

namespace NRemoteDeviceServer
{
namespace Config
{

class CcRemoteDeviceSHARED CCommand
{
public:
  CCDEFINE_EQUAL_OPERATORS(CCommand)
  bool bTest;
};

#ifdef _MSC_VER
template class CcRemoteDeviceSHARED CcList<CCommand>;
#endif

/**
 * @brief CStartup implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CStartup
{
public:
  void parseJson(CcJsonNode& rJson);
  void writeJson(CcJsonNode& rNode);

  const CcConfigBinary::CItem *parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize);
  size_t writeBinary(IIo& pStream);

public:
  bool bStopOnError = true;
  CcList<CCommand> oCommands;
};
}
}
#endif // H_CStartup_H_
