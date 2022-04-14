/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVariant
 */
#pragma once

#include "CcBase.h"

// Forward declarations
class CcString;
class CcStringList;
class CcByteArray;
class CcDateTime;
class CcVariantList;
class CcVersion;
class CcUuid;
class CcIp;
class IIo;

#ifdef WINDOWS
#include <windows.h>
#include <oaidl.h>
#endif

/**
 * @brief Class containing Data of various Type.
 *        It is able to get value converted to correct type.
 *        Complex datatypes are represented as pointer.
 */
class CcKernelSHARED CcVariant
{
public:
  //! @brief Supported types for CcVarian
  enum class EType : uint32
  {
    NoType,
    Bool,
    Switch, // Switch will be handled like bool
    Int8,
    Uint8,
    Int16,
    Uint16,
    Int32,
    Uint32,
    Int64,
    Uint64,
    Size,
    Float,      //! @todo implement!!!
    Double,     //! @todo implement!!!
    DateTime,
    String,
    ByteArray,
    StringList,
    Pointer,
    VariantList,
    Version,
    Uuid,
    Ip,
  };

  /**
   * @brief Constructor
   */
  CcVariant();
  //! @param eType: Init variant with specific type
  CcVariant(CcVariant::EType eType);
  //! @param bVal: Init variant with bool value
  CcVariant(bool bVal);
  //! @param uiToCopy: Init variant with unsigned char value
  CcVariant(uint8  uiToCopy );
  //! @param uiToCopy: Init variant with uint16 value
  CcVariant(uint16 uiToCopy);
  //! @param uiToCopy: Init variant with uint32 value
  CcVariant(uint32 uiToCopy);
  //! @param uiToCopy: Init variant with uint64 value
  CcVariant(uint64 uiToCopy);
  //! @param iToCopy: Init variant with int8 value
  CcVariant(int8  iToCopy);
  //! @param iToCopy: Init variant with int16 value
  CcVariant(int16 iToCopy);
  //! @param iToCopy: Init variant with int32 value
  CcVariant(int32 iToCopy);
  //! @param iToCopy: Init variant with int64 value
  CcVariant(int64 iToCopy);
#ifdef WINDOWS
  //! @param i32Val: Init variant with integer value
  CcVariant(int i32Val) :
    CcVariant(static_cast<int32>(i32Val))
  {}
  //! @param ui32Val: Init variant with unsigned integer value
  CcVariant(uint ui32Val) :
    CcVariant(static_cast<uint32>(ui32Val))
  {}
#endif

  //! @param uiToCopy: Init variant with float value
  CcVariant(float uiToCopy);
  //! @param uiToCopy: Init variant with double value
  CcVariant(double uiToCopy);
  //! @param sToCopy: Init variant with String
  CcVariant(const CcString& sToCopy);
  //! @param oToCopy: Init variant with DateTime
  CcVariant(const CcDateTime& oToCopy);
  //! @param oToCopy: Init variant with StringList
  CcVariant(const CcStringList& oToCopy);
  //! @param oToCopy: Init variant with ByteArray
  CcVariant(const CcByteArray& oToCopy);
  //! @param oToCopy: Init variant with VariantList
  CcVariant(const CcVariantList& oToCopy);
  //! @param oVersion: Init variant with Version
  CcVariant(const CcVersion& oVersion);
  //! @param oVersion: Init variant with Uuid
  CcVariant(const CcUuid& oVersion);
  //! @param oIp: Init variant with IP
  CcVariant(const CcIp& oIp);

  /**
   * @brief Copy Constructor
   */
  CcVariant( const CcVariant &copy );

  /**
   * @brief Copy Constructor
   */
  CcVariant(CcVariant&& sToMove) NOEXCEPT
  { *this = CCMOVE(sToMove);}

  /**
   * @brief Copy Constructor
   */
  CcVariant( const char* pcToCopy );

#ifdef WINDOWS
  /**
   * @brief Create a Variant by initializing with Windows type of Variant;
   * @param winVariant: Variant to Set
   * @param winVariantType: Type of Variant as enum
   */
  CcVariant(VARIANT &winVariant, VARENUM winVariantType);
#endif

  /**
   * @brief Destructor
   */
  ~CcVariant();

  /**
   * @brief Get current Type of this Variant
   * @return current type as enum
   */
  inline CcVariant::EType getType() const
    { return m_eType; }
  
  /**
   * @brief Delete Value stored in Class and set Type to eNoType
   */
  void clear();

  /**
   * @brief Get bool Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as bool
   */
  bool getBool(bool *bOk = nullptr) const;

  /**
   * @brief Get uint8 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int8
   */
  int8 getInt8(bool *bOk = nullptr) const;

  /**
   * @brief Get uint8 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint8
   */
  uint8 getUint8(bool *bOk = nullptr) const;

  /**
   * @brief Get uint16 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int16
   */
  int16 getInt16(bool *bOk = nullptr) const;

  /**
   * @brief Get uint16 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint16
   */
  uint16 getUint16(bool *bOk = nullptr) const;

  /**
   * @brief Get uint32 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int16
   */
  int32 getInt32(bool *bOk = nullptr) const;

  /**
   * @brief Get uint32 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint32
   */
  uint32 getUint32(bool *bOk = nullptr) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int64
   */
  int64 getInt64(bool *bOk = nullptr) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint64
   */
  uint64 getUint64(bool *bOk = nullptr) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int64
   */
  int getInt(bool *bOk = nullptr) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint64
   */
  uint getUint(bool *bOk = nullptr) const;

  /**
   * @brief Get size_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as size_t
   */
  size_t getSize(bool *bOk = nullptr) const;

  /**
   * @brief Get size_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as size_t
   */
  float getFloat(bool *bOk = nullptr) const;

  /**
   * @brief Get size_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as size_t
   */
  double getDouble(bool *bOk = nullptr) const;

  /**
   * @brief Get time_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as time_t
   */
  CcDateTime getTime(bool *bOk = nullptr) const;

  /**
   * @brief Get CcString Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcString
   */
  CcString getString(bool *bOk = nullptr) const;

  /**
   * @brief Get editable CcString Value from Variant
   *        Type must be checked befor to avoid heap corruption
   * @return value as CcString
   */
  const CcString& getStringRef() const
  { return *m_Data.String; }

  /**
   * @brief Get CcStringList Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcStringList
   */
  CcStringList getStringList(bool *bOk = nullptr) const;

  /**
   * @brief Get CcString Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcString
   */
  CcByteArray getByteArray(bool *bOk = nullptr) const;

  /**
   * @brief Get CcString Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcString
   */
  void* getVoid(bool *bOk = nullptr) const;

  /**
   * @brief Get variant list value
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcVariantList
   */
  CcVariantList getVariantList(bool *bOk = nullptr) const;

  /**
   * @brief Get editable CcVariantList value from Variant
   *        Type must be checked befor to avoid heap corruption
   * @return Reference an VariantList
   */
  const CcVariantList& getVariantListRef() const
  { return *m_Data.VariantList; }

  /**
   * @brief Get version value
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcVersion
   */
  CcVersion getVersion(bool *bOk = nullptr) const;

  /**
   * @brief Get Uuid value
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcUuid
   */
  CcUuid getUuid(bool *bOk = nullptr) const;

  /**
   * @brief Get Ip value
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcUuid
   */
  CcIp getIp(bool *bOk = nullptr) const;

#ifdef WINDOWS
  /**
   * @brief Create a Variant by initializing with Windows type of Variant;
   * @param winVariant: Variant to Set
   * @param winVariantType: Type of Variant as enum
   */
  VARIANT getWinVariant( VARENUM& winVariantType );
#endif

  /**
   * @brief Get size of stored item in variant
   * @return Current size
   */
  inline size_t getDataSize() const
    { return writeData(nullptr, 0); }

  /**
   * @brief Get size of stored item in variant
   * @param pBuffer:      Buffer to write data from Variant to or null to retrieve required size
   * @param uiBufferSize: Maximum size the buffer can take.
   * @return Written or required size, depending on pBuffer.
   *         SIZE_MAX will be returned if an error occured.
   */
  size_t writeData(void* pBuffer, size_t uiBufferSize) const;

  /**
   * @brief Write conent of variant to IIoStream
   * @param oIo: Stream to write to
   * @return Number of bytes written
   */
  size_t writeData(IIo& oIo) const;

  //! @return Get size of data required to write current object
  size_t getWriteDataSize() const;

  //! @param bVal: Set boolean value to this variant
  void set(bool bVal);
  //! @param ui8Val: Set signed 8 bit integer value to this variant
  void set(int8 ui8Val);
  //! @param ui8Val: Set unsigned 8 bit integer value to this variant
  void set(uint8 ui8Val);
  //! @param ui16Val: Set signed 16 bit integer value to this variant
  void set(int16 ui16Val);
  //! @param ui16Val: Set unsigned 16 bit integer value to this variant
  void set(uint16 ui16Val);
  //! @param ui32Val: Set signed 32 bit integer value to this variant
  void set(int32 ui32Val);
  //! @param ui32Val: Set unsigned 32 bit integer value to this variant
  void set(uint32 ui32Val);
  //! @param ui64Val: Set signed 64 bit integer value to this variant
  void set(int64 ui64Val);
  //! @param ui64Val: Set unsigned 64 bit integer value to this variant
  void set(uint64 ui64Val);
  //! @param fVal: Set float value to this variant
  void set(float fVal);
  //! @param dVal: Set double value to this variant
  void set(double dVal);
  //! @param val: Set string as char array to this variant
  void set(const char* val);
  //! @param val: Set string value to this variant
  void set(const CcString& val);
  //! @param val: Set ByteArrax value to this variant
  void set(const CcByteArray& val);
  //! @param val: Set Stringlist value to this variant
  void set(const CcStringList& val);
  //! @param val: Set DateTime value to this variant
  void set(const CcDateTime& val);
  //! @param val: Set VariantList value to this variant
  void set(const CcVariantList& val);
  //! @param val: Set Version value to this variant
  void set(const CcVersion& val);
  //! @param val: Set Uuid value to this variant
  void set(const CcUuid& val);
  //! @param val: Set Ip value to this variant
  void set(const CcIp& val);
  //! @param val: Set void pointer value to this variant
  void set(void* val);
  //! @param uiSizeVal: Set size_t value to this variant
  void setSize(size_t uiSizeVal);

#ifdef WINDOWS
  void set(int i32Val)
  { set(static_cast<int32>(i32Val)); }
  void set(uint ui32Val)
  { set(static_cast<uint32>(ui32Val)); }
#endif

  //! @return True if current object is nullptr
  bool isNull() const
  {return m_Data.Pointer == nullptr; }
  //! @return True if current object is bool
  bool isBool() const
  {return m_eType == CcVariant::EType::Bool;}
  //! @return True if current object is a integer type of any width
  bool isInt() const;
  //! @return True if current object is a unsigned integer type of any width
  bool isUint() const;
  //! @return True if current object is a float or double
  bool isFloat() const;
  //! @return True if current object is a string
  bool isString() const;
  //! @return True if current object is a ByteArray
  bool isByteArray() const;

  /**
   * @brief Convert current variant to another value
   * @param eType: Target type for converstion
   * @return True if conversion succeeded
   */
  bool convert(CcVariant::EType eType);

  /**
   * @brief Compare two variants
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcVariant& oToCompare) const;

  /**
   * @brief Compare two variants
   * @param oToCompare: Item to compare to
   * @return True if they are not same, otherwise false
   */
  inline bool operator!=(const CcVariant& oToCompare) const
  { return !operator==(oToCompare); }

  /**
   * @brief Move value from another variant to this
   * @param oToMove: Variant to move contents
   * @return Handle to this
   */
  CcVariant& operator=(CcVariant&& oToMove);

  /**
   * @brief Copy value from another variant to this
   * @param oToCopy: Variant to copy contents
   * @return Handle to this
   */
  CcVariant& operator=(const CcVariant& oToCopy);

#ifdef WINDOWS
  /**
   * @brief Create a Variant by initializing with Windows type of Variant;
   * @param winVariant:     Variant to Set
   * @param winVariantType: Type of Variant as enum
   * @param uiIndex:        If not SIZE_MAX, winVariant will be handled has array of values, and specific value will be set.
   */
  bool set(VARIANT &winVariant, VARENUM winVariantType, size_t uiIndex = SIZE_MAX);
#endif

  static const CcString& getStringFromType(EType eType);
  static EType getTypeFromString(const CcString& eType);

private:
  /**
   * @brief Union of all types
   */
  union CcKernelSHARED
  {
    void*           Pointer;
    CcString*       String;
    CcByteArray*    ByteArray;
    CcStringList*   StringList;
    CcDateTime*     DateTime;
    CcVersion*      Version;
    CcVariantList*  VariantList;
    CcUuid*         Uuid;
    CcIp*           Ip;
    bool      bData;
    int8      i8Data;
    uint8     ui8Data;
    int16     i16Data;
    uint16    ui16Data;
    int32     i32Data;
    uint32    ui32Data;
    int64     i64Data;
    uint64    ui64Data = 0;
    size_t    Size;
    float     Float;
    double    Double;
  } m_Data;
  EType m_eType = CcVariant::EType::NoType;

public:
  static CcVariant oNullVariant;
};
