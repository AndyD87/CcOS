/*
 * This file is part of CBinaryFormat.
 *
 * CBinaryFormat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CBinaryFormat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CBinaryFormat.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CBinaryFormat
 */
#include "CBinaryFormat.h"
#include "CcRemoteDeviceGlobals.h"
#include "CcVersion.h"
#include "CcUuid.h"
#include "CcIp.h"
#include "CcStatic.h"

namespace NRemoteDeviceServerConfig
{
  const CBinaryFormat::CItem CBinaryFormat_oBinaryConfigMap[] =
  {
    {CBinaryFormat::EType::End,               0,                  CcVariant::EType::NoType,       nullptr},
    {CBinaryFormat::EType::Version,           sizeof(CcVersion),  CcVariant::EType::Version,      &CcRemoteDeviceGlobals::Config::Version},
    {CBinaryFormat::EType::Name,              UINT32_MAX,         CcVariant::EType::String,       &CcRemoteDeviceGlobals::Config::SystemNs::Name},
    {CBinaryFormat::EType::VendorId,          sizeof(CcUuid),     CcVariant::EType::Uuid,         &CcRemoteDeviceGlobals::Config::VendorId},
    {CBinaryFormat::EType::DeviceId,          sizeof(CcUuid),     CcVariant::EType::Uuid,         &CcRemoteDeviceGlobals::Config::DeviceId},
    {CBinaryFormat::EType::Variant,           UINT32_MAX,         CcVariant::EType::String,       &CcRemoteDeviceGlobals::Config::Variant},
    {CBinaryFormat::EType::SerialNr,          sizeof(uint32),     CcVariant::EType::Uint32,       &CcRemoteDeviceGlobals::Config::SerialNr},
    {CBinaryFormat::EType::HwVersion,         sizeof(CcVersion),  CcVariant::EType::Version,      &CcRemoteDeviceGlobals::Config::HwVersion},
    {CBinaryFormat::EType::SwVersion,         sizeof(CcVersion),  CcVariant::EType::Version,      &CcRemoteDeviceGlobals::Config::SwVersion},
    {CBinaryFormat::EType::Detectable,        1,                  CcVariant::EType::Bool,         &CcRemoteDeviceGlobals::Config::Detectable},
    {CBinaryFormat::EType::System,            UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::System},
    {CBinaryFormat::EType::Interfaces,        UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::Interfaces},
    {CBinaryFormat::EType::Events,            UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::Events},
    {CBinaryFormat::EType::Startup,           UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::Startup},
    {CBinaryFormat::EType::Application,       UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::Application},
    {CBinaryFormat::EType::WlanAccessPoint,   UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPoint},
    {CBinaryFormat::EType::WlanClient,        UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::SystemNs::WlanClient},
    {CBinaryFormat::EType::SSID,              UINT32_MAX,         CcVariant::EType::String,       &CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::SSID},
    {CBinaryFormat::EType::Password,          UINT32_MAX,         CcVariant::EType::String,       &CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Password},
    {CBinaryFormat::EType::Dhcp,              sizeof(CcIp),       CcVariant::EType::Ip,           &CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Dhcp},
    {CBinaryFormat::EType::Enable,            1,                  CcVariant::EType::Bool,         &CcRemoteDeviceGlobals::Config::SystemNs::WlanAccessPointNs::Enable},
    {CBinaryFormat::EType::KnownAccessPoints, UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::SystemNs::WlanClientNs::KnownAccessPoints},
    {CBinaryFormat::EType::RestApiEnabled,    1,                  CcVariant::EType::Bool,         &CcRemoteDeviceGlobals::Config::InterfacesNs::RestApiEnabled},
    {CBinaryFormat::EType::RestApi,           UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::InterfacesNs::RestApi},
    {CBinaryFormat::EType::Custom,            UINT32_MAX,         CcVariant::EType::ByteArray,    &CcRemoteDeviceGlobals::Config::InterfacesNs::RestApi}
  };
  const size_t CBinaryFormat_oBinaryConfigMapSize = sizeof(CBinaryFormat_oBinaryConfigMap) / sizeof(CBinaryFormat_oBinaryConfigMap[0]);

  bool CBinaryFormat::CItem::isInList() const
  {
    return static_cast<size_t>(getType()) < CBinaryFormat_oBinaryConfigMapSize && CBinaryFormat_oBinaryConfigMap[static_cast<size_t>(getType())].getType() == getType();
  }

  bool CBinaryFormat::CItem::getNext(CItem*& pItem, size_t& uiMaxSize)
  {
    bool bSuccess = false;
    CItem* pCurrentItem = pItem;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->getSize() != CItem::getMaxSize())
      {
        // Get size from stored item
        if (pCurrentListItem->getSize() <= uiMaxSize)
        {
          bSuccess = true;
          char* pcItem = CCVOIDPTRCAST(char*, pItem) + pCurrentListItem->getSize();
          pcItem += sizeof(CBinaryFormat::EType);
          uiMaxSize -= pcItem - CCVOIDPTRCAST(char*, pItem);
          pItem = CCVOIDPTRCAST(CItem*, pcItem);
        }
      }
      else
      {
        // Get size from income item because it is not defined.
        if (pCurrentItem->getSize() <= uiMaxSize)
        {
          bSuccess = true;
          char* pcItem = CCVOIDPTRCAST(char*, pItem) + pCurrentItem->getSize();
          pcItem += sizeof(CBinaryFormat::EType);
          pcItem += sizeof(CBinaryFormat::CItem::uiSize);
          uiMaxSize -= pcItem - CCVOIDPTRCAST(char*, pItem);
          pItem = CCVOIDPTRCAST(CItem*, pcItem);
        }
      }
    }
    else
    {
      // Get size from income item because it is not defined.
      if (pCurrentItem->getSize() <= uiMaxSize)
      {
        bSuccess = true;
        char* pcItem = CCVOIDPTRCAST(char*, pItem) + pCurrentItem->getSize();
        pcItem += sizeof(CBinaryFormat::EType);
        pcItem += sizeof(CBinaryFormat::CItem::uiSize);
        uiMaxSize -= pcItem - CCVOIDPTRCAST(char*, pItem);
        pItem = CCVOIDPTRCAST(CItem*, pcItem);
      }
    }
    return bSuccess;
  }

  bool CBinaryFormat::CItem::getNext(const CItem*& pItem, size_t& uiMaxSize) const
  {
    bool bSuccess = false;
    const CItem* pCurrentItem = pItem;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->getSize() != CItem::getMaxSize())
      {
        // Get size from stored item
        if (pCurrentListItem->getSize() <= uiMaxSize)
        {
          const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + pCurrentListItem->getSize();
          pcItem += sizeof(CBinaryFormat::EType);
          uiMaxSize -= pcItem - CCVOIDPTRCONSTCAST(char*, pItem);
          pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
        }
      }
      else
      {
        // Get size from income item because it is not defined.
        if (pCurrentItem->getSize() <= uiMaxSize)
        {
          const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + pCurrentItem->getSize();
          pcItem += sizeof(CBinaryFormat::EType);
          pcItem += sizeof(CBinaryFormat::CItem::uiSize);
          uiMaxSize -= pcItem - CCVOIDPTRCONSTCAST(char*, pItem);
          pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
        }
      }
    }
    else
    {
      // Get size from income item because it is not defined.
      if (pCurrentItem->getSize() <= uiMaxSize)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, pItem) + pCurrentItem->getSize();
        pcItem += sizeof(CBinaryFormat::EType);
        pcItem += sizeof(CBinaryFormat::CItem::uiSize);
        uiMaxSize -= pcItem - CCVOIDPTRCONSTCAST(char*, pItem);
        pItem = CCVOIDPTRCONSTCAST(CItem*, pcItem);
      }
    }
    return bSuccess;
  }

  void* CBinaryFormat::CItem::getCurrentBuffer()
  {
    void* pBuffer = nullptr;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->uiSize != getMaxSize())
      {
        pBuffer = CCVOIDPTRCAST(char*, this) + sizeof(eType);
      }
      else
      {
        pBuffer = CCVOIDPTRCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      }
    }
    else
    {
      pBuffer = CCVOIDPTRCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    }
    return pBuffer;
  }


  const void* CBinaryFormat::CItem::getCurrentBuffer() const
  {
    const void* pBuffer = nullptr;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->uiSize != getMaxSize())
      {
        pBuffer = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
      }
      else
      {
        pBuffer = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      }
    }
    else
    {
      pBuffer = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    }
    return pBuffer;
  }

  CcString CBinaryFormat::CItem::getString() const
  {
    CcString sReturn;
    const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
    sReturn.set(pcItem, getSize());
    return sReturn;
  }

  CcVersion CBinaryFormat::CItem::getVersion() const
  {
    CcVersion oReturn;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Version)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        CcStatic::memcpy(&oReturn, pcItem, sizeof(CcVersion));
      }
    }
    else if(getSize() >= sizeof(CcVersion))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      CcStatic::memcpy(&oReturn, pcItem, sizeof(CcVersion));
    }
    return oReturn;
  }

  CcUuid CBinaryFormat::CItem::getUuid() const
  {
    CcUuid oReturn;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Uuid)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        CcStatic::memcpy(&oReturn, pcItem, sizeof(CcUuid));
      }
    }
    else if (getSize() >= sizeof(CcUuid))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      CcStatic::memcpy(&oReturn, pcItem, sizeof(CcUuid));
    }
    return oReturn;
  }

  CcIp CBinaryFormat::CItem::getIp() const
  {
    CcIp oReturn;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Ip)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        CcStatic::memcpy(&oReturn, pcItem, sizeof(CcIp));
      }
    }
    else if (getSize() >= sizeof(CcIp))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      CcStatic::memcpy(&oReturn, pcItem, sizeof(CcIp));
    }
    return oReturn;
  }

  bool CBinaryFormat::CItem::getBool() const
  {
    bool oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Bool)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *pcItem != 0;
      }
    }
    else if (getSize() >= sizeof(CcIp))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *pcItem != 0;
    }
    return oReturn;
  }

  int8 CBinaryFormat::CItem::getInt8() const
  {
    int8 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Int8)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *pcItem;
      }
    }
    else if (getSize() >= sizeof(int8))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *pcItem;
    }
    return oReturn;
  }

  int16 CBinaryFormat::CItem::getInt16()  const
  {
    int16 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Int16)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(uint16*, pcItem);
      }
    }
    else if (getSize() >= sizeof(uint16))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(uint16*, pcItem);
    }
    return oReturn;
  }
  
  int32 CBinaryFormat::CItem::getInt32() const
  {
    int32 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Int32)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(int32*, pcItem);
      }
    }
    else if (getSize() >= sizeof(int32))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(int32*, pcItem);
    }
    return oReturn = 0;
  }

  int64 CBinaryFormat::CItem::getInt64() const
  {
    int64 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Int64)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(int64*, pcItem);
      }
    }
    else if (getSize() >= sizeof(int64))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(int64*, pcItem);
    }
    return oReturn;
  }

  uint8 CBinaryFormat::CItem::getUint8() const
  {
    uint8 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Uint8)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(uint8*, pcItem);
      }
    }
    else if (getSize() >= sizeof(uint8))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(uint8*, pcItem);
    }
    return oReturn;
  }

  uint16 CBinaryFormat::CItem::getUint16() const
  {
    uint16 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Uint16)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(uint16*, pcItem);
      }
    }
    else if (getSize() >= sizeof(uint16))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(uint16*, pcItem);
    }
    return oReturn;
  }

  uint32 CBinaryFormat::CItem::getUint32() const
  {
    uint32 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Uint32)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(uint32*, pcItem);
      }
    }
    else if (getSize() >= sizeof(uint32))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(uint32*, pcItem);
    }
    return oReturn;
  }

  uint64 CBinaryFormat::CItem::getUint64() const
  {
    uint64 oReturn = 0;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == CcVariant::EType::Uint64)
      {
        const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType);
        oReturn = *CCVOIDPTRCONSTCAST(uint64*, pcItem);
      }
    }
    else if (getSize() >= sizeof(uint64))
    {
      const char* pcItem = CCVOIDPTRCONSTCAST(char*, this) + sizeof(eType) + sizeof(uiSize);
      oReturn = *CCVOIDPTRCONSTCAST(uint64*, pcItem);
    }
    return oReturn;
  }

  bool CBinaryFormat::CItem::write(EType eNewType, const CcVariant& oVariant, size_t uiMaxSize)
  {
    bool bOk = false;
    this->eType = eNewType;
    if (isInList())
    {
      const CItem* pCurrentListItem = CBinaryFormat_oBinaryConfigMap + static_cast<size_t>(getType());
      if (pCurrentListItem->eVariantType == oVariant.getType())
      {
        void* pBuffer = getCurrentBuffer();
        if (pBuffer)
        {
          bOk = true;
          size_t uiSizeLeft = static_cast<char*>(pBuffer) - CCVOIDPTRCAST(char*, this);
          oVariant.writeData(pBuffer, uiMaxSize - uiSizeLeft);
        }
      }
    }
    else
    {
      void* pBuffer = getCurrentBuffer();
      if (pBuffer)
      {
        bOk = true;
        size_t uiSizeLeft = static_cast<char*>(pBuffer) - CCVOIDPTRCAST(char*, this);
        uiSize = static_cast<uint32>(oVariant.writeData(pBuffer, uiMaxSize - uiSizeLeft));
      }
    }
    return bOk;
  }
}
