/*
 * This file is part of CConfig.
 *
 * CConfig is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CConfig is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CConfig.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CConfig
 */
#include "CConfig.h"
#include "Network/CcCommonPorts.h"
#include "Devices/IEeprom.h"
#include "CcDevice.h"
#include "CcKernel.h"
#include "CcMemoryMonitor.h"
#include "CcUuidKnown.h"
#include "CcJson/CcJsonObject.h"
#include "Resources/CcRemoteDeviceGeneric.json.h"
#include "CcFile.h"
#include "NDocumentsGlobals.h"
#include "CcBinaryStream.h"
#include "Network/CcSocket.h"
#include "Network/CcCommonIps.h"
#include "Devices/CcDeviceEeprom.h"

using namespace NRemoteDevice::Server;

const char CConfig::c_aBinaryTag[8] = {'C','C','R','D','S','C','F','G'};

CConfig::CConfig(bool bLoadConfig) :
  oAddressInfo(ESocketType::UDP, CcCommonIps::AnyAddress, CcCommonPorts::CcRemoteDevice)
{
  init(bLoadConfig);
}

CConfig::~CConfig()
{
}

CcStatus CConfig::init(bool bLoadDefault)
{
  CcStatus oStatus = false;
  if(bLoadDefault == true)
  {
    CcDeviceEeprom oEepromDevice = CcKernel::getDevice(EDeviceType::Eeprom);
    if(oEepromDevice.isValid())
    {
      bool bWriteConfig = false;
      CCDEBUG("EEPROM found, use for config");
      if(oEepromDevice.open(EOpenFlags::Read))
      {
        oEepromDevice.setPosition(0);
        CcByteArray oData = oEepromDevice.readAll();
        char* pData = oData.getArray();
        oEepromDevice.close();
        CCDEBUG("Read Binary: " + CcString::fromSize(oData.size()));
        if(parseBinary(pData, oData.size()))
        {
          oStatus = true;
          m_eSource = ESource::EepromBinary;
        }
        else if(oData[0] == '{')
        {
          CCDEBUG("Read JSON");
          m_eSource = ESource::EepromJson;
          CcJsonDocument oJsonDocument;
          oJsonDocument.parseDocument(oData);
          if(oJsonDocument.getJsonData().isNotNull())
          {
            if(oJsonDocument.getJsonData().isObject()&&
               oJsonDocument.getJsonData().object()["Version"].isNotNull())
            {
              oStatus = true;
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
          parseJson(oJsonDocument.getJsonNode());
        }
        else
        {
          CCERROR("No valid config found");
          bWriteConfig = true;
        }
      }
      if(bWriteConfig)
      {
        oEepromDevice.setPosition(0);
        writeBinary(oEepromDevice);
      }
    }
  }
  else
  {
    oStatus = true;
  }
  return oStatus;
}

void CConfig::read(const CcString& sPath)
{
  CcFile oFile(sPath);
  if(oFile.open(EOpenFlags::Read))
  {
    CcByteArray oData = oFile.readAll();
    readData(oData);
    oFile.close();
  }
}

void CConfig::readData(const CcByteArray& oData)
{
  if(oData.startsWith(c_aBinaryTag, sizeof(c_aBinaryTag)))
  {
    parseBinary(oData.getArray(), oData.size());
  }
  else if(oData.size() > 0 &&
          oData[0] == '{')
  {
    CcJsonDocument oDoc;
    oDoc.parseDocument(oData);
    parseJson(oDoc.getJsonNode());
  }
}

void CConfig::write(ESource eSource, const CcString& sPath)
{
  if (eSource == ESource::Unknown) eSource = m_eSource;
  if (m_eSource == ESource::Unknown) m_eSource = eSource;
  switch(eSource)
  {
    case ESource::EepromJson:
      CCFALLTHROUGH;
    case ESource::EepromBinary:
      writeEeprom(m_eSource);
      break;
    case ESource::FileJson:
      CCFALLTHROUGH;
    case ESource::FileBinary:
      if(sPath.length())
      {
        writeFile(m_eSource, sPath);
      }
      else
      {
        writeFile(m_eSource, m_sFilePath);
      }
      break;
    default:
      break;
  }
}

void CConfig::writeData(ESource eSource, CcByteArray& oData)
{
  CcBinaryStream oStream(oData);
  switch (eSource)
  {
    case ESource::FileJson:
      writeJson(oStream);
      break;
    case ESource::FileBinary:
      writeBinary(oStream);
      break;
    default:
      break;
  }
}

void CConfig::parseJson(CcJsonNode& rJson)
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
        else if (rNode.getName() == NDocumentsGlobals::NConfig::Port &&
          rNode.value().isInt())
        {
          oAddressInfo.setPort(rNode.getValue().getUint16());
        }
      }
    }
  }
}

void CConfig::writeJson(IIo& pStream)
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
    oDoc.getJsonNode().object().append(CcJsonNode(NDocumentsGlobals::NConfig::Port, CcVariant(oAddressInfo.getPort())));
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
  oDoc.writeDocument(pStream);
}

bool CConfig::parseBinary(const void* pvItem, size_t uiMaxSize)
{
  bool bAllOk = false;
  if(uiMaxSize <= sizeof(c_aBinaryTag))
  {
    CCERROR("Not enough space for binary config");
  }
  else
  {
    if(0 != CcStatic::memcmp(pvItem, c_aBinaryTag, sizeof(c_aBinaryTag)))
    {
      CCERROR("Invalid beginning for binary config");
    }
    else
    {
      bAllOk = true;
      uiMaxSize -= sizeof(c_aBinaryTag);
      pvItem=CCVOIDPTRCONSTADD(pvItem, sizeof(c_aBinaryTag));
      const CcConfigBinary::CItem* pItem = static_cast<const CcConfigBinary::CItem*>(pvItem);
      while (pItem->isEnd() == false && bAllOk)
      {
        CCDEBUG("Next value: " + CcString::fromSize(static_cast<size_t>(pItem->getType())));
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
          case CcConfigBinary::EType::Port:
            oAddressInfo.setPort(pItem->getUint16());
            break;
          case CcConfigBinary::EType::System:
            pItem = oSystem.parseBinary(pItem, uiMaxSize);
            break;
          case CcConfigBinary::EType::Events:
            pItem = oEvents.parseBinary(pItem, uiMaxSize);
            break;
          case CcConfigBinary::EType::Startup:
            pItem = oStartup.parseBinary(pItem, uiMaxSize);
            break;
          case CcConfigBinary::EType::Interfaces:
            pItem = oInterfaces.parseBinary(pItem, uiMaxSize);
            break;
          case CcConfigBinary::EType::Application:
            pItem = parseAppConfigBinary(static_cast<const void*>(pItem), uiMaxSize);
            break;
          default:
            bAllOk = false;
            CCERROR("Error occured, unknown type");
            break;
        }
        if(bAllOk)
        {
          bAllOk = pItem->getNext(pItem, uiMaxSize);
        }
      }
    }
  }
  return bAllOk;
}

size_t CConfig::writeBinary(IIo& pStream)
{
  CCDEBUG("Write Binary");
  size_t uiWritten = pStream.write(c_aBinaryTag, sizeof(c_aBinaryTag));
  if(uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Version, oVersion);
  if(uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::VendorId, oVendorId);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::DeviceId, oDeviceId);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Variant, sVariant);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SerialNr, uiSerialNr);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SwVersion, oSwVersion);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::HwVersion, oHwVersion);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Detectable, bDetectable);
  if (uiWritten != SIZE_MAX)
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Port, oAddressInfo.getPort());
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += oSystem.writeBinary(pStream);
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += oInterfaces.writeBinary(pStream);
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += writeAppConfigBinary(pStream);
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End, nullptr);
  }
  else
  {
    uiWritten = SIZE_MAX;
  }
  return uiWritten;
}

const char* CConfig::getDefaultConfig()
{
  return CcRemoteDeviceGeneric_json;
}

size_t CConfig::getDefaultConfigSize()
{
  return CcRemoteDeviceGeneric_json_Length;
}

// Use this as example implementation
void CConfig::parseAppConfig(CcJsonNode &rJson)
{
  if (rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if (rNode.isObject())
      {

      }
      else if (rNode.isValue())
      {

      }
    }
  }
}

void CConfig::writeAppConfig(CcJsonNode &rJson)
{
  if (rJson.isObject())
  {
    //rJson.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Version, oVersion.getString()));
  }
}

const CcConfigBinary::CItem* CConfig::parseAppConfigBinary(const void* pItem, size_t uiMaxSize)
{
  CCUNUSED(pItem);
  CCUNUSED(uiMaxSize);
  return static_cast<const CcConfigBinary::CItem*>(pItem);
}

size_t CConfig::writeAppConfigBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Application);
  if (uiWritten != SIZE_MAX)
  {
    uiWritten += CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
  }
  return uiWritten;
}

void CConfig::writeEeprom(ESource eSource)
{
  CcDevice oEepromDevice = CcKernel::getDevice(EDeviceType::Eeprom);
  if(oEepromDevice.isValid())
  {
    switch (eSource)
    {
      case ESource::EepromJson:
        writeJson(*oEepromDevice.getDevice<IEeprom>());
        break;
      case ESource::EepromBinary:
        writeBinary(*oEepromDevice.getDevice<IEeprom>());
        break;
      default:
        break;
    }
  }
}

void CConfig::writeFile(ESource eSource, const CcString& sPath)
{
  CcFile oFile(sPath);
  if(oFile.open(EOpenFlags::Write))
  {
    switch (eSource)
    {
      case ESource::FileJson:
        writeJson(oFile);
        break;
      case ESource::FileBinary:
        writeBinary(oFile);
        break;
      default:
        break;
    }
    oFile.close();
  }
}
