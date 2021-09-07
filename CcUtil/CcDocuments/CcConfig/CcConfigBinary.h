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
#pragma once

#include "CcDocument.h"
#include "CcVector.h"
#include "CcVariant.h"
#include "CcString.h"
#include "IIo.h"

class CcString;
class CcDateTime;

/**
 * @brief Configuration class for binary storage.
 *        It can be filled with known types to save some bytes, or with custom definitions
 *        to be flexible for changes.
 */
class CcDocumentsSHARED CcConfigBinary
{
public:
  /**
   * @brief Config items type
   */
  enum class EType : uint32
  {
    End = 0,      //!< Default type is end of config.
    Version = 1,  //!< Item is a CcVersion.
    Name,         //!< Item is a name as string.
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
    IpV4,
    IpV4Begin,
    IpV4End,
    User,
    Users,
    Admin,

    Custom = 0x80000000,      //!< Start address of custom / user defined commands
    Unknown = 0xffffffff,     //!< Unknown Type, or invalid
  };

  /**
   * @brief Item class for binary configs
   */
  class CcDocumentsSHARED CItem
  {
  public:
    /**
     * @brief Initialize item with type and size
     * @param eType:  Target Type
     * @param uiSize: Size of element
     */
    CItem(CcConfigBinary::EType eType, uint32 uiSize) :
      eType(eType),
      uiSize(uiSize)
    {}

    //! @return Get max size any config item can have
    inline static uint32 getMaxSize()
    { return UINT32_MAX; }
    //! @return Get type of item stored
    inline EType getType() const
    { return eType; }
    //! @return Get size of current item.
    inline uint32 getSize() const
    { return uiSize; }
    //! @return Get size aligned by this item.
    uint32 getAlignedSize() const;
    //! @return true if current item type is @ref EType::End.
    inline bool isEnd() const
    { return getType() == EType::End; }
    //! @return true if current item type known in @ref EType.
    bool isInList() const;
    /**
     * @brief Get next item relative to this.
     * @param[out]    pItem    : Reference receiving pointer of next item, or nullptr if failed.
     * @param[in,out] uiMaxSize: Size left on pItem to verify if next item is valid.
     *                           It will be updated and reduzed by size of next this item.
     * @return true if next item was received
     */
    bool getNext(CItem*& pItem, size_t& uiMaxSize);

    /**
     * @brief Get next item relative to this. This method will skipp inter items.
     * @param[out]    pItem    : Reference receiving pointer of next item, or nullptr if failed.
     * @param[in,out] uiMaxSize: Size left on pItem to verify if next item is valid.
     *                           It will be updated and reduzed by size of next this item.
     * @return true if next item was received
     */
    bool getNext(const CItem*& pItem, size_t& uiMaxSize) const;

    /**
     * @brief If this item is an object with inner items, the returned item will point
     *        first inner item.
     * @param[out]    pItem    : Reference receiving pointer of first inner item, or nullptr
     *                           if failed.
     * @param[in,out] uiMaxSize: Size left on pItem to verify if next item is valid.
     *                           It will be updated and reduzed by size of next this item.
     * @return true if next item was received and is valid.
     */
    bool getInner(CItem*& pItem, size_t& uiMaxSize);

    /**
     * @brief If this item is an object with inner items, the returned item will point
     *        first inner item.
     * @param[out]    pItem     : Reference receiving pointer of first inner item, or nullptr
     *                            if failed.
     * @param[in,out] uiMaxSize : Size left on pItem to verify if next item is valid.
     *                            It will be updated and reduzed by size of next this item.
     * @return true if next item was received and is valid.
     */
    bool getInner(const CItem*& pItem, size_t& uiMaxSize) const;

    //! @return Get stored string as CcString, validity should be checked before.
    CcString getString() const;
    //! @return Get stored CcVersion, validity should be checked before.
    CcVersion getVersion() const;
    //! @return Get stored CcUuid, validity should be checked before.
    CcUuid getUuid() const;
    //! @return Get stored CcIp, validity should be checked before.
    CcIp getIp() const;
    //! @return Get stored CcDateTime, validity should be checked before.
    CcDateTime getDateTime() const;
    //! @return Get stored boolean, validity should be checked before.
    bool getBool() const;
    //! @return Get stored 8 bit integer, validity should be checked before.
    int8 getInt8() const;
    //! @return Get stored 16 bit integer, validity should be checked before.
    int16 getInt16() const;
    //! @return Get stored 32 bit integer, validity should be checked before.
    int32 getInt32() const;
    //! @return Get stored 64 bit integer, validity should be checked before.
    int64 getInt64() const;
    //! @return Get stored 8 bit unsigned integer, validity should be checked before.
    uint8 getUint8() const;
    //! @return Get stored 16 bit unsigned integer,, validity should be checked before.
    uint16 getUint16() const;
    //! @return Get stored 32 bit unsigned integer,, validity should be checked before.
    uint32 getUint32() const;
    //! @return Get stored 64 bit unsigned integer,, validity should be checked before.
    uint64 getUint64() const;

    /**
     * @brief Write an known item out to stream.
     *        A check of oVariant and eType will be done too.
     * @param[out] rStream: Stream to write data to
     * @param eType:        Type of item to write
     * @param oVariant:     Variant with item to write.
     * @return Number of bytes written, or SIZE_MAX if nothing was written.
     */
    static size_t write(IIo& rStream, EType eType, const CcVariant& oVariant = nullptr);

    /**
     * @brief Get an item from knwon list by Type.
     *        It can be used to get informations like sizes for item.
     * @param eType: Type of item to retreive informations from
     * @return Item in list or nullptr if eType is not valid.
     */
    static const CItem* getConfigItem(EType eType);

    /**
     * @brief Check if eType is in stored in known lists
     * @param eType: Type to verify
     * @return True if type is known.
     */
    static bool isInList(EType eType);

    /**
     * @brief Update size value of this item.
     * @param uiNewSize: new size to set.
     */
    inline void setSize(size_t uiNewSize)
    { this->uiSize = static_cast<uint32>(uiNewSize); }
    /**
     * @brief Get EType by position in Typelist.
     * @param uiPosition: Number of EType to query.
     * @return Found type or EType::Custom if not found.
     */
    static EType knownListGetType(size_t uiPosition);

    /**
     * @brief Get number of known types.
     * @return Number of known types
     */
    static size_t knownListGetSize();

    EType             eType;        //!< Type of this item
    uint32            uiSize;       //!< Sizeo of this item
  };

  static const CcString& getString(EType eType);
};
