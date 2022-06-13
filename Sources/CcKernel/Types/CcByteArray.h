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
 * @brief     Class CcByteArray
 **/
#pragma once

#include "CcBase.h"
#include "CcVector.h"

#ifdef _MSC_VER
template CcKernelSHARED class CcVector<char>;
#endif

class CcString;
class CcCrc32;

/**
 * @brief Dynamic Storage for char's
 */
class CcKernelSHARED CcByteArray : public CcVector<char>
{
public:
  CcByteArray() = default;

  /**
   * @brief Constructor wich can append a 0 terminated char array
   * @param pToAppend: pointer to constant /0 terminiated char array
   */
  CcByteArray(const char* pToAppend);

  /**
   * @brief Constructor with an allready existing memory to copy.
   * @param pToAppend: Pointer to target memory
   * @param uiLen: Length of memory on pToAppend
   */
  CcByteArray(const char* pToAppend, size_t uiLen);

  /**
   * @brief Constructor with an allready existing memory to copy.
   * @param pToAppend: Pointer to target memory
   * @param uiLen: Length of memory on pToAppend
   */
  CcByteArray(const unsigned char* pToAppend, size_t uiLen);

  /**
   * @brief Copy Constructor
   * @param oToAppend: ByteArray to be copied
   */
  CcByteArray(const CcByteArray& oToAppend);

  /**
   * @brief Move Constructor
   *        Source object will no longer be usable!
   * @param oToMove: ByteArray to be moved from
   */
  CcByteArray(CcByteArray&& oToMove);

  /**
   * @brief Construtor with data from a string as init data
   * @param sToAppend: String to copy to ByteArray
   */
  CcByteArray(const CcString& sToAppend);

  /**
   * @brief Constructor with predefined size.
   *        The Object will be created with an init size of uiReserved
   * @param uiReserverd: Number of bytes to be preallocate.
   */
  CcByteArray(size_t uiReserverd);

  /**
   * @brief Destructor
   */
  ~CcByteArray() = default;

  /**
   * @brief check if item is allready added to List
   * @return true if list contains item, otherwise false
   */
  size_t find(const CcString& toFind, size_t uiBegin = 0, size_t uiDistance = SIZE_MAX) const;

  /**
   * @brief Write a part of stored chars to buffer
   * @param buf:     buffter to write chars to
   * @param bufSize: maximum buffer Size
   * @param from:    set startpoint for charray in storage
   * @return count of written chars to buffer
   */
  size_t getCharArray(char *buf, size_t bufSize, size_t from = 0);

  /**
   * @brief Import ByteArray from an HexString like 0x1234fe or 1234fe
   * @param sHexString: Hex string to be filled in ByteArray
   * @return true if parsing succeeded, otherwise false.
   */
  bool setHexString(const CcString& sHexString);
  
  /**
   * @brief Append a String to byte array.
   * @param sString: String to append
   * @return true if parsing succeeded, otherwise false.
   */
  void appendString(const CcString& sString);

  /**
   * @brief Get content from ByteArray as an HexString without leading 0x
   * @return Hex formated string.
   */
  CcString getHexString()const;

  /**
   * @brief Get content from ByteArray as an OctalString in format \12
   * @return Hex formated string.
   */
  CcString getOctalString();

  /**
   * @brief This Method will create an crc32 hash value over content.
   * @return Hash value of stored data.
   */
  CcCrc32 getCrc32();

  /**
   * @brief Check if this binary data starts with a specific binary
   * @param pData:  Byte array to compare to.
   * @param uiSize: Size or length of pData to compare to.
   * @return true if containing data starts with this data.
   */
  bool startsWith(const char* pData, size_t uiSize) const;

  /**
   * @brief Swap all bytes from beginning to end.
   */
  void swapEndian();

  /**
   * @brief Swap within every 4 bytes in Array.
   */
  void swapEndian32();

  /**
   * @brief Move Operator
   *        Source Object will no longer be usable.
   * @param oToMove: Object to move from
   * @return Reference to this object.
   */
  CcByteArray& operator=(CcByteArray&& oToMove);

  /**
   * @brief Copy Operator
   * @param oToCopy: Object to copy from
   * @return Reference to this object.
   */
  CcByteArray& operator=(const CcByteArray& oToCopy);

  /**
   * @brief Move content of string to this byte array
   * @param oToMove: String to move from
   * @return Reference to this object.
   */
  CcByteArray& operator=(CcString&& oToMove) NOEXCEPT;

  /**
   * @brief Compare internal buffer with \0 terminated string
   * @param pData: String to compare with
   * @return true if string and content is same
   */
  bool operator==(const char* pData) const;

  inline bool operator==(const CcByteArray& oData) const
  { return CcVector<char>::operator ==(oData); }

  /**
   * @brief Execute an ordinary read on buffer with size and offset
   * @param pBuffer:  Buffer to write data to
   * @param uSize:    Maximum size of buffer to write
   * @param uiOffset: Offest within byte array to start read.
   * @return Number fo bytes read from byte array
   */
  size_t read(void* pBuffer, size_t uSize, size_t uiOffset = 0);

  /**
   * @brief Execute an ordinary write on buffer with size and offset
   * @param pBuffer:  Buffer to read data from
   * @param uSize:    Maximum size of buffer to read
   * @param uiOffset: Offest within byte array to start read.
   * @return Number fo bytes written from buffer to byte array
   */
  size_t write(const void* pBuffer, size_t uSize, size_t uiOffset = 0);


  //! @return Get buffer of byte array already casted
  template<class T>
  inline T* cast(size_t uiOffset = 0)
  { return reinterpret_cast<T*>(getArray(uiOffset)); }

  static const CcByteArray& getEmpty()
  { return s_oEmpty; }

private:
  static CcByteArray s_oEmpty;
};
