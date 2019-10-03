/*
 * This file is part of CWlan.
 *
 * CWlan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CWlan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CWlan.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CWlan
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CWlan
 **/
#ifndef H_CWlan_H_
#define H_CWlan_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcHttpServerConfig.h"
#include "CcString.h"
#include "CcList.h"
#include "CcPassword.h"

namespace NsRemoteDeviceServerConfig
{
/**
 * @brief CWlan implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CWlan
{
public:
  CcString sClientSsid;
  CcPassword oClientPassword;
  bool bClientEnabled = true;
  CcString sServerSsid;
  CcPassword oServerPassword;
  bool bServerEnabled = true;
};
}

#endif // H_CWlan_H_
