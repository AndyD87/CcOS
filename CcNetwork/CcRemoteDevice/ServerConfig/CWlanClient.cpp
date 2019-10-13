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
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CWlanClient
 */
#include "CWlanClient.h"
#include "CcJson/CcJsonArray.h"

namespace NsRemoteDeviceServerConfig
{

void CWlanClient::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
      }
      else if(rNode.isArray())
      {
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::KnownAccessPoints)
        {
          for (CcJsonNode& rAps : rNode.array())
          {
            if(rAps.isArray() &&
               rAps.array().size() == 2 &&
               rAps.array()[0].isValue() &&
               rAps.array()[1].isValue() &&
               rAps.array()[0].value().isString() &&
               rAps.array()[1].value().isString())
            {
              CCredentials oCreds;
              oCreds.sSsid = rAps.array()[0].value().getString();
              oCreds.oPassword = rAps.array()[1].value().getString();
              oKnownAccessPoints.add(oCreds);
            }
          }
        }
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::Enable &&
           rNode.value().isBool())
        {
          bEnable = rNode.value().getBool();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::Dhcp &&
                rNode.value().isBool())
        {
          bDhcp = rNode.value().getBool();
        }
      }
    }
  }
}

void CWlanClient::writeJson(CcJsonNode& rNode)
{
  if(rNode.isObject())
  {
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::Dhcp, bDhcp));
    rNode.object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::Enable, bEnable));

    CcJsonNode oKnown(EJsonDataType::Array);
    oKnown.setName(CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::KnownAccessPoints);
    for(const CCredentials& oCredential : oKnownAccessPoints)
    {
      CcJsonNode oCredentialArray(EJsonDataType::Array);
      CcJsonNode oSsid("", oCredential.sSsid);
      CcJsonNode oPassword("", oCredential.oPassword.getString());
      oCredentialArray.array().append(oSsid);
      oCredentialArray.array().append(oPassword);
      oKnown.array().append(oCredentialArray);

    }
    rNode.object().append(oKnown);
  }
}
}
