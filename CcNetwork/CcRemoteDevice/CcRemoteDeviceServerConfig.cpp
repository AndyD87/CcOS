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
#include "Resources/CcRemoteDeviceGeneric.json.h"
#include "CcFile.h"
#include "NDocumentsGlobals.h"

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

void CcRemoteDeviceServerConfig::loadJsonFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  if(oFile.open(EOpenFlags::Read))
  {
    CcJsonDocument oDoc;
    oDoc.parseDocument(oFile.readAll());
    parseJson(oDoc.getJsonNode());
    oFile.close();
  }
}

void CcRemoteDeviceServerConfig::loadBinaryFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  if(oFile.open(EOpenFlags::Read))
  {
    CcByteArray oData = oFile.readAll();
    parseBinary(oData.getArray(), oData.size());
    oFile.close();
  }
}

void CcRemoteDeviceServerConfig::writeJsonFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  if(oFile.open(EOpenFlags::Write))
  {
    oFile.writeString(writeJson());
    oFile.close();
  }
}

void CcRemoteDeviceServerConfig::parseJson(CcJsonNode& rJson)
{
  if(rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if(rNode.isObject())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::System)
          oSystem.parseJson(rNode);
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Events)
          oEvents.parseJson(rNode);
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Startup)
          oStartup.parseJson(rNode);
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Interfaces)
          oInterfaces.parseJson(rNode);
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Application)
          parseAppConfig(rNode);
      }
      else if(rNode.isValue())
      {
        if(rNode.getName() == NDocumentsGlobals::NConfig::Version &&
           rNode.value().isString())
        {
          oVersion = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::VendorId &&
                rNode.value().isString())
        {
          oVendorId = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::DeviceId &&
                rNode.value().isString())
        {
          oDeviceId = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Variant &&
                rNode.value().isString())
        {
          sVariant = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::SerialNr &&
                rNode.value().isInt())
        {
          uiSerialNr = rNode.value().getString().toUint32();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::HwVersion &&
                rNode.value().isString())
        {
          oHwVersion = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::SwVersion &&
                rNode.value().isString())
        {
          oSwVersion = rNode.value().getString();
        }
        else if(rNode.getName() == NDocumentsGlobals::NConfig::Detectable &&
                rNode.value().isBool())
        {
          bDetectable = rNode.value().getBool();
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
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::Version, oVersion.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::VendorId, oVendorId.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::DeviceId, oDeviceId.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::Variant, sVariant));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::SerialNr, uiSerialNr));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::HwVersion, oHwVersion.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::SwVersion, oSwVersion.getString()));
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::Detectable, bDetectable));
    CcJsonNode oSystemNode(EJsonDataType::Object);
    oSystemNode.setName(NDocumentsGlobals::NConfig::System);
    oSystem.writeJson(oSystemNode);
    oDoc.getJsonNode().object().append(oSystemNode);
    CcJsonNode oInterfacesNode(EJsonDataType::Object);
    oInterfacesNode.setName(NDocumentsGlobals::NConfig::Interfaces);
    oInterfaces.writeJson(oInterfacesNode);
    oDoc.getJsonNode().object().append(oInterfacesNode);
    CcJsonNode oAppConfig(EJsonDataType::Object);
    oAppConfig.setName(NDocumentsGlobals::NConfig::Application);
    writeAppConfig(oAppConfig);
    if(oAppConfig.isObject() && oAppConfig.object().size())
      oDoc.getJsonNode().object().append(oAppConfig);
  }
  return oDoc.getDocument(true);
}

void CcRemoteDeviceServerConfig::parseBinary(const void* pvItem, size_t uiMaxSize)
{
  const CcConfigBinary::CItem* pItem = static_cast<const CcConfigBinary::CItem*>(pvItem);
  bool bAllOk = true;
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::Version:
        oVersion = pItem->getVersion();
        break;
      case CcConfigBinary::EType::VendorId:
        oVendorId = pItem->getUuid();
        break;
      case CcConfigBinary::EType::DeviceId:
        oDeviceId = pItem->getUuid();
        break;
      case CcConfigBinary::EType::Variant:
        sVariant = pItem->getString();
        break;
      case CcConfigBinary::EType::SerialNr:
        uiSerialNr = pItem->getUint32();
        break;
      case CcConfigBinary::EType::SwVersion:
        oSwVersion = pItem->getVersion();
        break;
      case CcConfigBinary::EType::HwVersion:
        oHwVersion = pItem->getVersion();
        break;
      case CcConfigBinary::EType::Detectable:
        bDetectable = pItem->getBool();
        break;
      case CcConfigBinary::EType::System:
        oSystem.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::Events:
        oEvents.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::Startup:
        oStartup.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::Interfaces:
        oInterfaces.parseBinary(pItem, uiMaxSize);
        break;
      case CcConfigBinary::EType::Application:
        parseAppConfigBinary(static_cast<const void*>(pItem), uiMaxSize);
        break;
      default:
        break;
    }
    if(bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CcRemoteDeviceServerConfig::writeBinary(void* pvItem, size_t uiMaxSize)
{
  size_t uiSizeLeft = uiMaxSize;
  size_t uiWritten = 0;
  CcConfigBinary::CItem* pItem = static_cast<CcConfigBinary::CItem*>(pvItem);
  uiWritten += pItem->write(CcConfigBinary::EType::Version, oVersion, uiSizeLeft);
  if(pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::VendorId, oVendorId, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::DeviceId, oDeviceId, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::Variant, sVariant, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::SerialNr, uiSerialNr, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::SwVersion, oSwVersion, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::HwVersion, oHwVersion, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
    uiWritten += pItem->write(CcConfigBinary::EType::Detectable, bDetectable, uiSizeLeft);
  if (pItem->getNext(pItem, uiSizeLeft))
  {
    uiWritten += oSystem.writeBinary(pItem, uiSizeLeft);
  }
  if (pItem->getNext(pItem, uiSizeLeft))
  {
    uiWritten += oInterfaces.writeBinary(pItem, uiSizeLeft);
  }
  if (pItem->getNext(pItem, uiSizeLeft))
  {
    uiWritten += writeAppConfigBinary(pItem, uiSizeLeft);
  }
  if (pItem->getNext(pItem, uiSizeLeft))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::End, nullptr, uiSizeLeft);
  }
  else
  {
    uiWritten = SIZE_MAX;
  }
  return uiWritten;
}

const char* CcRemoteDeviceServerConfig::getDefaultConfig()
{
  return CcRemoteDeviceGeneric_json;
}

size_t CcRemoteDeviceServerConfig::getDefaultConfigSize()
{
  return CcRemoteDeviceGeneric_json_Length;
}

void CcRemoteDeviceServerConfig::parseAppConfig(CcJsonNode &rJson)
{
  CCUNUSED(rJson);
}

void CcRemoteDeviceServerConfig::writeAppConfig(CcJsonNode &rJson)
{
  CCUNUSED(rJson);
}

void CcRemoteDeviceServerConfig::parseAppConfigBinary(const void* pItem, size_t uiMaxSize)
{
  CCUNUSED(pItem);
  CCUNUSED(uiMaxSize);
}

size_t CcRemoteDeviceServerConfig::writeAppConfigBinary(void* pvItem, size_t uiMaxSize)
{
  CcConfigBinary::CItem* pItem = static_cast<CcConfigBinary::CItem*>(pvItem);
  CcConfigBinary::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CcConfigBinary::EType::Application);
  if (pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::End);
  }
  pThisItem->setSize(uiWritten);
  return uiWritten;
}

