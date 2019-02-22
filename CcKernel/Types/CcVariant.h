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
 * @page      Types
 * @subpage   CcVariant
 *
 * @page      CcVariant
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcVariant
 */
#ifndef _CcVariant_H_
#define _CcVariant_H_

#include "CcBase.h"
#include "CcKernelBase.h"

// Forward declarations
class CcString;
class CcByteArray;
class CcUrl;
class CcDateTime;

#ifdef WIN32
#include "OaIdl.h"
#endif

enum class EVariantType
{
  NoType,
  Bool,
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
  Pointer,
};

/**
 * @brief Class containing Data of various Type.
 *        It is able to get value converted to correct type.
 */
class CcKernelSHARED  CcVariant 
{
public:
  /**
   * @brief Constructor
   */
  CcVariant(void );
  
  CcVariant(EVariantType eType);
  CcVariant(bool bVal);
  CcVariant(uint8  uiToCopy );
  CcVariant(uint16 uiToCopy);
  CcVariant(uint32 uiToCopy);
  CcVariant(uint64 uiToCopy);
  CcVariant(int8  iToCopy);
  CcVariant(int16 iToCopy);
  CcVariant(int32 iToCopy);
  CcVariant(int64 iToCopy);
  CcVariant(float uiToCopy);
  CcVariant(double uiToCopy);
  CcVariant(const CcString& sToCopy);
  CcVariant(const CcByteArray& sToCopy);

  /**
   * @brief Copy Constructor
   */
  CcVariant( const CcVariant &copy );
  
  /**
   * @brief Copy Constructor
   */
  CcVariant(CcVariant&& sToMove)
    { *this = std::move(sToMove);}

  /**
   * @brief Copy Constructor
   */
  CcVariant( const char* pcToCopy );

#ifdef WIN32
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
  ~CcVariant(void );

  /**
   * @brief Get current Type of this Variant
   * @return current type as enum
   */
  inline EVariantType getType(void) const
    { return m_eType; }

  /**
   * @brief Delete Value stored in Class and set Type to eNoType
   */
  void clear(void);

  /**
   * @brief Get bool Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as bool
   */
  bool getBool(bool *bOk = NULL) const;

  /**
   * @brief Get uint8 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int8
   */
  int8 getInt8(bool *bOk = NULL) const;

  /**
   * @brief Get uint8 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint8
   */
  uint8 getUint8(bool *bOk = NULL) const;

  /**
   * @brief Get uint16 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int16
   */
  int16 getInt16(bool *bOk = NULL) const;

  /**
   * @brief Get uint16 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint16
   */
  uint16 getUint16(bool *bOk = NULL) const;

  /**
   * @brief Get uint32 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int16
   */
  int32 getInt32(bool *bOk = NULL) const;

  /**
   * @brief Get uint32 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint32
   */
  uint32 getUint32(bool *bOk = NULL) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int64
   */
  int64 getInt64(bool *bOk = NULL) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint64
   */
  uint64 getUint64(bool *bOk = NULL) const;
  
  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as int64
   */
  int getInt(bool *bOk = NULL) const;

  /**
   * @brief Get uint64 Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as uint64
   */
  uint getUint(bool *bOk = NULL) const;

  /**
   * @brief Get size_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as size_t
   */
  size_t getSize(bool *bOk = NULL) const;
  
  /**
   * @brief Get size_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as size_t
   */
  float getFloat(bool *bOk = NULL) const;
  
  /**
   * @brief Get size_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as size_t
   */
  double getDouble(bool *bOk = NULL) const;

  /**
   * @brief Get time_t Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as time_t
   */
  CcDateTime getTime(bool *bOk = NULL) const;

  /**
   * @brief Get CcString Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcString
   */
  CcString getString(bool *bOk = NULL) const;

  /**
   * @brief Get CcString Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcString
   */
  CcByteArray getByteArray(bool *bOk = NULL) const;
  
  /**
   * @brief Get CcString Value from Variant
   * @param[out] bOk: set to true if variable was converted correctly.
   * @return value as CcString
   */
  void* getVoid(bool *bOk = NULL) const;

#ifdef WIN32
  /**
   * @brief Create a Variant by initializing with Windows type of Variant;
   * @param winVariant: Variant to Set
   * @param winVariantType: Type of Variant as enum
   */
  VARIANT getWinVariant( VARENUM& winVariantType );
#endif

  void set(bool bVal);
  void set(int8 ui8Val);
  void set(uint8 ui8Val);
  void set(int16 ui16Val);
  void set(uint16 ui16Val);
  void set(int32 ui32Val);
  void set(uint32 ui32Val);
  void set(int64 ui64Val);
  void set(uint64 ui64Val);
  void set(float fVal);
  void set(double dVal);
  void set(const char* val);
  void set(const CcString& val);
  void set(const CcByteArray& val);
  void set(const CcDateTime& val);
  void set(void* val);

  bool isNull() const
    {return m_Data.Pointer == nullptr; }
  bool isBool() const
    {return m_eType == EVariantType::Bool;}
  bool isInt() const;
  bool isUint() const;
  bool isFloat() const;
  bool isString() const;
  bool isByteArray() const;

  bool convert(EVariantType eType);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcVariant& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcVariant& toCompare) const 
    {return !operator==(toCompare);}

  CcVariant& operator=(CcVariant&& oToMove);
  CcVariant& operator=(const CcVariant& oToCopy);

#ifdef WIN32
  /**
   * @brief Create a Variant by initializing with Windows type of Variant;
   * @param winVariant: Variant to Set
   * @param winVariantType: Type of Variant as enum
   */
  void set(VARIANT &winVariant, VARENUM winVariantType);
#endif

private:
  union
  {
    void*    Pointer = nullptr;
    CcString*     String;
    CcByteArray*  ByteArray;
    CcDateTime*   Time;
    bool     bData;
    int8     i8Data;
    uint8    ui8Data;
    int16    i16Data;
    uint16   ui16Data;
    int32    i32Data;
    uint32   ui32Data;
    int64    i64Data;
    uint64   ui64Data;
    size_t   Size;
    float    Float;
    double   Double;
  } m_Data;
  EVariantType m_eType = EVariantType::NoType;
};

#endif /* _CcVariant_H_ */
