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
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConfigBinary
 **/
#ifndef H_CcConfigBinary_H_
#define H_CcConfigBinary_H_

#include "CcDocument.h"
#include "CcVector.h"
#include "CcVariant.h"
#include "CcString.h"
#include "IIo.h"

class CcString;
class CcDateTime;

class CcDocumentsSHARED CcConfigBinary
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
    WlanCredential,
    SSID,
    Password,
    Dhcp,
    Enable,
    DhcpEnable,
    KnownAccessPoints,
    RestApiEnabled,
    RestApi,
    HttpServer,
    WorkingDirectory,
    DefaultEncoding,
    SslEnable,
    SslPrivateKey,
    SslCertificate,
    SslPrivateKeyPath,
    SslCertificatePath,
    Timeout,
    BufferSize,
    MaxThreads,
    DownloadDirectory,
    Port,

    Custom = 0x80000000,
  };

  class CcDocumentsSHARED CItem
  {
  public:
    inline static uint32 getMaxSize()
      { return UINT32_MAX; }

    inline EType getType() const
      { return eType; }
    inline uint32 getSize() const
      { return uiSize; }
    uint32 getAlignedSize() const;
    inline bool isEnd() const
      { return getType() == EType::End; }
    bool isInList() const;
    bool getNext(CItem*& pItem, size_t& uiMaxSize);
    bool getNext(const CItem*& pItem, size_t& uiMaxSize) const;
    bool getInner(CItem*& pItem, size_t& uiMaxSize);
    bool getInner(const CItem*& pItem, size_t& uiMaxSize) const;

    CcString getString() const;
    CcVersion getVersion() const;
    CcUuid getUuid() const;
    CcIp getIp() const;
    CcDateTime getDateTime() const;
    bool getBool() const;
    int8 getInt8() const;
    int16 getInt16() const;
    int32 getInt32() const;
    int64 getInt64() const;
    uint8 getUint8() const;
    uint16 getUint16() const;
    uint32 getUint32() const;
    uint64 getUint64() const;

    static size_t write(IIo& rStream, EType eType, const CcVariant& oVariant = nullptr);
    static const CItem* getConfigItem(EType eType);
    static bool isInList(EType eType);

    inline void setSize(size_t uiNewSize)
      { this->uiSize = static_cast<uint32>(uiNewSize); }

    static EType knownListGetType(size_t uiPosition);
    static size_t knownListGetSize();

    EType             eType;
    uint32            uiSize;
    CcVariant::EType  eVariantType;
    const CcString*   pName;
  };
};

#endif // H_CcConfigBinary_H_
