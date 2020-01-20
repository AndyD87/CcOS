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
 * @page      CBinaryFormat
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CBinaryFormat
 **/
#ifndef H_CBinaryFormat_H_
#define H_CBinaryFormat_H_

#include "CcRemoteDevice.h"
#include "CcVector.h"
#include "CcVariant.h"

class CcString;

namespace NRemoteDeviceServerConfig
{

class CcRemoteDeviceSHARED CBinaryFormat
{
public:
  enum class EType : uint32
  {
    End = 0,
    Version = 1,
    Name,
    VendorId,
    DeviceId,
    Variant,
    SerialNr,
    HwVersion,
    SwVersion,
    Detectable,
    System,
    Interfaces,
    Events,
    Startup,
    Application,
    WlanAccessPoint,
    WlanClient,
    SSID,
    Password,
    Dhcp,
    Enable,
    KnownAccessPoints,
    RestApiEnabled,
    RestApi,

    Custom = 0x80000000,
  };

  class CItem
  {
  public:
    inline static uint32 getMaxSize()
      { return UINT32_MAX; }

    inline EType getType() const
      { return eType; }
    inline uint32 getSize() const
      { return uiSize; }
    inline bool isEnd() const
      { return eType == EType::End; }
    bool isInList() const;
    bool getNext(CItem*& pItem, size_t& uiMaxSize);
    bool getNext(const CItem*& pItem, size_t& uiMaxSize) const;

    void* getCurrentBuffer();
    const void* getCurrentBuffer() const;

    CcString getString() const;
    CcVersion getVersion() const;
    CcUuid getUuid() const;
    CcIp getIp() const;
    bool getBool() const;
    int8 getInt8() const;
    int16 getInt16() const;
    int32 getInt32() const;
    int64 getInt64() const;
    uint8 getUint8() const;
    uint16 getUint16() const;
    uint32 getUint32() const;
    uint64 getUint64() const;

    bool write(EType eType, const CcVariant& oVariant = nullptr, size_t uiMaxSize = 0);

    inline void setSize(uint32 uiSize)
      { this->uiSize = uiSize; }
    EType             eType;
    uint32            uiSize;
    CcVariant::EType  eVariantType;
    const CcString*   pName;
  };
};

}

#endif // H_CBinaryFormat_H_
