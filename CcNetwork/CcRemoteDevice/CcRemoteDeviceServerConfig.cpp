/*
 * This file is part of CcRemoteDeviceServerConfig.
 *
 * CcRemoteDeviceServerConfig is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServerConfig is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServerConfig.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceServerConfig
 */
#include "CcRemoteDeviceServerConfig.h"
#include "Network/CcCommonPorts.h"
#include "Devices/IEeprom.h"
#include "IDevice.h"
#include "CcKernel.h"
#include "CcMemoryMonitor.h"
#include "CcUuidKnown.h"
#include "CcJson/CcJsonObject.h"

#ifdef GENERIC
#include "Resources/CcRemoteDeviceGeneric.json.h"
#endif

CcRemoteDeviceServerConfig::CcRemoteDeviceServerConfig(bool bLoadConfig)
{
  CcDeviceHandle pEepromDevice = CcKernel::getDevice(EDeviceType::Eeprom);
  if(bLoadConfig == true &&
     pEepromDevice.isValid())
  {
    bool bWriteConfig = false;
    CcHandle<IEeprom> pDevice = pEepromDevice.cast<IEeprom>();
    CCDEBUG("EEPROM found, use for config");
    if(pDevice->open(EOpenFlags::Read))
    {
      CcByteArray oData = pDevice->readAll();
      pDevice->close();
      if(oData[0] == '{')
      {
        oJsonDocument.parseDocument(oData);
        if(oJsonDocument.getJsonData().isNotNull())
        {
          if(oJsonDocument.getJsonData().isObject()&&
             oJsonDocument.getJsonData().object()["Version"].isNotNull())
          {
          }
          else
          {
            CCERROR("Json found but no Version tag");
            // Write default config
            bWriteConfig = true;
          }
        }
        else
        {
          CCERROR("No json object found");
          // Write default config
          bWriteConfig = true;
        }
      }
      else
      {
        CCERROR("No json indicator found");
        // Write default config
        bWriteConfig = true;
      }
    }
    if(bWriteConfig)
    {
#ifdef GENERIC
      if(pDevice->open(EOpenFlags::Write))
      {
        pDevice->write(CcRemoteDeviceGeneric_json, CcRemoteDeviceGeneric_json_Length);
        pDevice->close();
      }
      oJsonDocument.parseDocument(CcRemoteDeviceGeneric_json);
#endif
    }
    parseJson(oJsonDocument.getJsonNode());
  }
}

CcRemoteDeviceServerConfig::~CcRemoteDeviceServerConfig()
{
}

void CcRemoteDeviceServerConfig::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::System)
          oSystem.parseJson(rNode);
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::Events)
          oEvents.parseJson(rNode);
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::Startup)
          oStartup.parseJson(rNode);
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == CcRemoteDeviceGlobals::Config::Version &&
           rNode.value().isString())
        {
          oVersion = rNode.value().getString();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::VendorId &&
                rNode.value().isString())
        {
          oVendorId = rNode.value().getString();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::DeviceId &&
                rNode.value().isString())
        {
          oDeviceId = rNode.value().getString();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::Variant &&
                rNode.value().isString())
        {
          sVariant = rNode.value().getString();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SerialNr &&
                rNode.value().isInt())
        {
          uiSerialNr = rNode.value().getString().toUint64();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::HwVersion &&
                rNode.value().isString())
        {
          oHwVersion = rNode.value().getString();
        }
        else if(rNode.getName() == CcRemoteDeviceGlobals::Config::SwVersion &&
                rNode.value().isString())
        {
          oSwVersion = rNode.value().getString();
        }
      }
    }
  }
}

CcString CcRemoteDeviceServerConfig::writeJson()
{
  CcJsonDocument oDoc;
  oDoc.getJsonNode().setJsonObject();
  if(oDoc.getJsonNode().isObject())
  {
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::Version, oVersion.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::VendorId, oVendorId.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::DeviceId, oDeviceId.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::Variant, sVariant));
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SerialNr, uiSerialNr));
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::HwVersion, oHwVersion.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(CcRemoteDeviceGlobals::Config::SwVersion, oSwVersion.getString()));
    CcJsonNode oSystemNode(EJsonDataType::Object);
    oSystemNode.setName(CcRemoteDeviceGlobals::Config::System);
    oSystem.writeJson(oSystemNode);
    oDoc.getJsonNode().object().append(oSystemNode);
  }
  return oDoc.getDocument(true);
}
