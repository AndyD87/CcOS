/*
 * This file is part of CcConfigBinary.
 *
 * CcConfigBinary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConfigBinary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConfigBinary.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcConfigBinary
 */
#include "CcConfigBinary.h"
#include "NDocumentsGlobals.h"
#include "CcVersion.h"
#include "CcUuid.h"
#include "CcIp.h"
#include "CcStatic.h"
#include "CcDateTime.h"

const CcConfigBinary::CItem CcConfigBinary_oBinaryConfigMap[] =
{
  {CcConfigBinary::EType::End,                0,                  CcVariant::EType::NoType,       nullptr},
  {CcConfigBinary::EType::Version,            sizeof(CcVersion),  CcVariant::EType::Version,      &NDocumentsGlobals::NConfig::Version},
  {CcConfigBinary::EType::Name,               UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::Name},
  {CcConfigBinary::EType::VendorId,           sizeof(CcUuid),     CcVariant::EType::Uuid,         &NDocumentsGlobals::NConfig::VendorId},
  {CcConfigBinary::EType::DeviceId,           sizeof(CcUuid),     CcVariant::EType::Uuid,         &NDocumentsGlobals::NConfig::DeviceId},
  {CcConfigBinary::EType::Variant,            UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::Variant},
  {CcConfigBinary::EType::SerialNr,           sizeof(uint32),     CcVariant::EType::Uint32,       &NDocumentsGlobals::NConfig::SerialNr},
  {CcConfigBinary::EType::HwVersion,          sizeof(CcVersion),  CcVariant::EType::Version,      &NDocumentsGlobals::NConfig::HwVersion},
  {CcConfigBinary::EType::SwVersion,          sizeof(CcVersion),  CcVariant::EType::Version,      &NDocumentsGlobals::NConfig::SwVersion},
  {CcConfigBinary::EType::Detectable,         sizeof(bool),       CcVariant::EType::Bool,         &NDocumentsGlobals::NConfig::Detectable},
  {CcConfigBinary::EType::System,             UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::System},
  {CcConfigBinary::EType::Interfaces,         UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::Interfaces},
  {CcConfigBinary::EType::Events,             UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::Events},
  {CcConfigBinary::EType::Startup,            UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::Startup},
  {CcConfigBinary::EType::Application,        UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::Application},
  {CcConfigBinary::EType::WlanAccessPoint,    UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::WlanAccessPoint},
  {CcConfigBinary::EType::WlanClient,         UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::WlanClient},
  {CcConfigBinary::EType::WlanCredential,     UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::WlanCredential},
  {CcConfigBinary::EType::SSID,               UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::SSID},
  {CcConfigBinary::EType::Password,           UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::Password},
  {CcConfigBinary::EType::Dhcp,               sizeof(CcIp),       CcVariant::EType::Ip,           nullptr},
  {CcConfigBinary::EType::Enable,             sizeof(bool),       CcVariant::EType::Bool,         &NDocumentsGlobals::NConfig::Enable},
  {CcConfigBinary::EType::DhcpEnable,         sizeof(bool),       CcVariant::EType::Bool,         &NDocumentsGlobals::NConfig::DhcpEnable},
  {CcConfigBinary::EType::KnownAccessPoints,  UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::KnownAccessPoints},
  {CcConfigBinary::EType::RestApiEnabled,     1,                  CcVariant::EType::Bool,         &NDocumentsGlobals::NConfig::RestApiEnabled},
  {CcConfigBinary::EType::RestApi,            UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::RestApi},
  {CcConfigBinary::EType::HttpServer,         UINT32_MAX,         CcVariant::EType::ByteArray,    &NDocumentsGlobals::NConfig::HttpServer},
  {CcConfigBinary::EType::WorkingDirectory,   UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::WorkingDirectory},
  {CcConfigBinary::EType::DefaultEncoding,    UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::DefaultEncoding},
  {CcConfigBinary::EType::SslEnable,          sizeof(bool),       CcVariant::EType::Bool,         &NDocumentsGlobals::NConfig::SslEnable},
  {CcConfigBinary::EType::SslPrivateKey,      UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::SslPrivateKey},
  {CcConfigBinary::EType::SslCertificate,     UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::SslCertificate},
  {CcConfigBinary::EType::SslPrivateKeyPath,  UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::SslPrivateKeyPath},
  {CcConfigBinary::EType::SslCertificatePath, UINT32_MAX,         CcVariant::EType::String,       &NDocumentsGlobals::NConfig::SslCertificatePath},
  {CcConfigBinary::EType::Timeout,            sizeof(CcDateTime), CcVariant::EType::DateTime,     &NDocumentsGlobals::NConfig::Timeout},
  {CcConfigBinary::EType::BufferSize,         sizeof(uint64),     CcVariant::EType::Uint64,       &NDocumentsGlobals::NConfig::BufferSize},
  {CcConfigBinary::EType::MaxThreads,         sizeof(uint32),     CcVariant::EType::Uint32,       &NDocumentsGlobals::NConfig::MaxThreads},
};
const size_t CcConfigBinary_oBinaryConfigMapSize = sizeof(CcConfigBinary_oBinaryConfigMap) / sizeof(CcConfigBinary_oBinaryConfigMap[0]);

bool CcConfigBinary::CItem::isInList() const
{
  return static_cast<size_t>(getType()) < CcConfigBinary_oBinaryConfigMapSize && CcConfigBinary_oBinaryConfigMap[static_cast<size_t>(getType())].getType() == getType();
}

bool CcConfigBinary::CItem::getNext(CItem*& pItem, size_t& uiMaxSize)
{
  bool bSuccess = false;
  if (isInList())
  {
    const CItem* pCurrentListItem = CcConfigBinary_oBinaryConfigMap + static_cast<size_t>(getType());
    if (pCurrentListItem->getSize() != CItem::getMaxSize())
    {
      bSuccess = true;
      char* pcItem = CCVOIDPTRCAST(char*, pItem) + pCurrentListItem->getSize();
      pcItem += sizeof(eType);
      pcItem += sizeof(uiSize);
      uiMaxSize -= pcItem - CCVOIDPTRCAST(char*, pItem);
      pItem = CCVOIDPTRCAST(CItem*, pcItem);
    }
    else
    {
      // Get size from income item because it is not defined.
      if (pItem->getSize() <= uiMaxSize)
      {
        bSuccess = true;
        char* pcItem = CCVOIDPTRCAST(char*, pItem) + pItem->getSize();
        pcItem += sizeof(eType);
        pcItem += sizeof(uiSize);
        uiMaxSize -= pcItem - CCVOIDPTRCAST(char*, pItem);
        pItem = CCVOIDPTRCAST(CItem*, pcItem);
      }
    }
  }
  else
  {
    // Get size from income item because it is not defined.
    if (pItem->getSize() <= uiMaxSize)
    {
      bSuccess = true;
      char* pcItem = CCVOIDPTRCAST(char*, pItem) + pItem->getSize();
      pcItem += sizeof(eType);
      pcItem += sizeof(uiSize);
      uiMaxSize -= pcItem - CCVOIDPTRCAST(char*, pItem);
      pItem = CCVOIDPTRCAST(CItem*, pcItem);
    }
  }
  return bSuccess;
}

bool CcConfigBinary::CItem::getNext(const CItem*& pItem, size_t& uiMaxSize) const
{
  bool bSuccess = false;
  if (isInList())
  {
    const CItem* pCurrentListItem = CcConfigBinary_oBinaryConfigMap + static_cast<size_t>(getType());
    if (pCurrentListItem->getSize() != CItem::getMaxSize())
    {
      bSuccess = true;
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + pCurrentListItem->getSize();
      pcItem += sizeof(eType);
      pcItem += sizeof(uiSize);
      uiMaxSize -= pcItem - CCVOIDPTRCONSTCAST(char*, pItem);
      pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
    }
    else
    {
      // Get size from income item because it is not defined.
      if (pItem->getSize() <= uiMaxSize)
      {
        bSuccess = true;
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + getSize();
        pcItem += sizeof(eType);
        pcItem += sizeof(uiSize);
        uiMaxSize -= pcItem - CCVOIDPTRCONSTCAST(char*, pItem);
        pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
      }
    }
  }
  else
  {
    // Get size from income item because it is not defined.
    if (pItem->getSize() <= uiMaxSize)
    {
      bSuccess = true;
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + pItem->getSize();
      pcItem += sizeof(eType);
      pcItem += sizeof(uiSize);
      uiMaxSize -= pcItem - CCVOIDPTRCONSTCAST(char*, pItem);
      pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
    }
  }
  return bSuccess;
}

bool CcConfigBinary::CItem::getInner(CItem*& pItem, size_t& uiMaxSize)
{
  bool bSuccess = false;
  size_t uiToAdd = sizeof(eType) + sizeof(uiSize);
  if (uiMaxSize >= uiToAdd)
  {
    bSuccess = true;
    uiMaxSize -= uiToAdd;
    char* pcItem = CCVOIDPTRCAST(char*, pItem) + uiToAdd;
    pItem = CCVOIDPTRCAST(CItem*, pcItem);
  }
  return bSuccess;
}

bool CcConfigBinary::CItem::getInner(const CItem*& pItem, size_t& uiMaxSize) const
{
  bool bSuccess = false;
  size_t uiToAdd = sizeof(eType) + sizeof(uiSize);
  if (uiMaxSize >= uiToAdd)
  {
    bSuccess = true;
    uiMaxSize -= uiToAdd;
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + uiToAdd;
    pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
  }
  return bSuccess;
}

CcString CcConfigBinary::CItem::getString() const
{
  CcString sReturn;
  const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
  sReturn.set(pcItem, getSize());
  return sReturn;
}

CcVersion CcConfigBinary::CItem::getVersion() const
{
  CcVersion oReturn;
  if (isInList()  &&
      getSize() >= sizeof(CcVersion))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    CcStatic::memcpy(&oReturn, pcItem, sizeof(CcVersion));
  }
  return oReturn;
}

CcUuid CcConfigBinary::CItem::getUuid() const
{
  CcUuid oReturn;
  if (isInList()  &&
      getSize() >= sizeof(CcUuid))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    CcStatic::memcpy(&oReturn, pcItem, sizeof(CcUuid));
  }
  return oReturn;
}

CcIp CcConfigBinary::CItem::getIp() const
{
  CcIp oReturn;
  if (isInList()  &&
      getSize() >= sizeof(CcIp))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    CcStatic::memcpy(&oReturn, pcItem, sizeof(CcIp));
  }
  return oReturn;
}

CcDateTime CcConfigBinary::CItem::getDateTime() const
{
  CcDateTime oReturn;
  if (isInList()  &&
      getSize() >= sizeof(CcDateTime))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    CcStatic::memcpy(&oReturn, pcItem, sizeof(CcDateTime));
  }
  return oReturn;
}

bool CcConfigBinary::CItem::getBool() const
{
  bool oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(bool))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *pcItem != 0;
  }
  return oReturn;
}

int8 CcConfigBinary::CItem::getInt8() const
{
  int8 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(int8))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *pcItem;
  }
  return oReturn;
}

int16 CcConfigBinary::CItem::getInt16()  const
{
  int16 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(uint16))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(uint16*, pcItem);
  }
  return oReturn;
}

int32 CcConfigBinary::CItem::getInt32() const
{
  int32 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(int32))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(int32*, pcItem);
  }
  return oReturn = 0;
}

int64 CcConfigBinary::CItem::getInt64() const
{
  int64 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(int64))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(int64*, pcItem);
  }
  return oReturn;
}

uint8 CcConfigBinary::CItem::getUint8() const
{
  uint8 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(uint8))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(uint8*, pcItem);
  }
  return oReturn;
}

uint16 CcConfigBinary::CItem::getUint16() const
{
  uint16 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(uint16))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(uint16*, pcItem);
  }
  return oReturn;
}

uint32 CcConfigBinary::CItem::getUint32() const
{
  uint32 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(uint32))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(uint32*, pcItem);
  }
  return oReturn;
}

uint64 CcConfigBinary::CItem::getUint64() const
{
  uint64 oReturn = 0;
  if (isInList()  &&
      getSize() >= sizeof(uint64))
  {
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    oReturn = *CCVOIDPTRCONSTCAST(uint64*, pcItem);
  }
  return oReturn;
}

size_t CcConfigBinary::CItem::write(EType eNewType, const CcVariant& oVariant, size_t uiMaxSize)
{
  size_t uiWritten = SIZE_MAX;
  this->eType = eNewType;
  if(!isInList())
  {
    CCERROR("Wrong type used for this configuration");
  }
  else
  {
    uiWritten = sizeof(eType) + sizeof(uiSize);
    char* pcItem = CCVOIDPTRCAST(char*, this) + uiWritten;
    size_t uiVariantWritten = oVariant.writeData(pcItem, uiMaxSize - uiWritten);
    setSize(static_cast<uint32>(uiVariantWritten));
    uiWritten += uiVariantWritten;
  }
  return uiWritten;
}

CcConfigBinary::EType CcConfigBinary::CItem::knownListGetType(size_t uiPosition)
{
  if(uiPosition < CcConfigBinary_oBinaryConfigMapSize)
  {
    return CcConfigBinary_oBinaryConfigMap[uiPosition].getType();
  }
  return EType::Custom;
}

size_t CcConfigBinary::CItem::knownListGetSize()
{
  return CcConfigBinary_oBinaryConfigMapSize;
}
