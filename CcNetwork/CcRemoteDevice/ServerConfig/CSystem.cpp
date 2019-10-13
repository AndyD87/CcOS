/*
 * This file is part of CSystem.
 *
 * CSystem is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CSystem is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CSystem.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CSystem
 */
#include "CSystem.h"

namespace NsRemoteDeviceServerConfig
{

void CSystem::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPoint)
          oWlanAccessPoint.parseJson(rNode);
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClient)
          oWlanClient.parseJson(rNode);
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::Name &&
           rNode.value().isString())
        {
          sName = rNode.value().getString();
          CCDEBUG("CcRemoteDevice device name: " + sName);
        }
      }
    }
  }
}

void CSystem::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::Name, sName));
    CcJsonNode oWlanAccessPointNode(EJsonDataType::Object);
    oWlanAccessPointNode.setName(CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPoint);
    oWlanAccessPoint.writeJson(oWlanAccessPointNode);
    rNode.object().append(oWlanAccessPointNode);
    CcJsonNode oWlanClientNode(EJsonDataType::Object);
    oWlanClientNode.setName(CcRemoteDeviceGlobals::Config::SystemNs::WlanClient);
    oWlanClient.writeJson(oWlanClientNode);
    rNode.object().append(oWlanClientNode);
  }
}
}
