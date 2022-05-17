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
 * @brief     Types for making exchanges more efficient or simple.
 */
#pragma once

#include "CcBase.h"
#include "CcByteArray.h"
#include "CcString.h"

/**
 * @brief All known hashing algorithims within CcOS
 */
enum class EHashType
{
  String = 0,  //!< Password is as simple string
  Unknown = 0, //!< Unknown Hash Type, handle it like a string
  Crc32,
  Md5,
  Sha256,
  SqlEnDecode,
};

/**
 * @brief Abstract class for all hashing algorithms.
 */
class CcKernelSHARED IHash
{
public:
  /**
   * @brief Destructor
   */
  virtual ~IHash() = default;

  /**
   * @brief Get calculated hash value as ByteArray.
   *        This Method must be supported from every implementing hash algorithm.
   * @return Get store result
   */
  virtual const CcByteArray& getValue() = 0;

  /**
   * @brief Fully generate hash value from buffer.
   *        This method has to reinit if required an finalizes if required.
   * @param pcData: Data to generate hash from.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual IHash& generate(const void* pcData, size_t uiLen) = 0;

  /**
   * @brief Append buffer to current object.
   * @param pcData: Data to append to current hash from.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual IHash& append(const void* pcData, size_t uiLen) = 0;

  /**
   * @brief Finalize hash generating.
   *        After calling this method, the stored Value for getValue is valid.
   * @param pcData: Last data to append to hash value.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual IHash& finalize(const void* pcData, size_t uiLen) = 0;

  /**
   * @brief Set key if it is supported.
   * @param pcData: Key data to set
   * @param uiLen:  Size of Key data
   * @return Handle to this object
   */
  virtual IHash& setKey(const void* pcData, size_t uiLen);

  /**
   * @brief Encode data and get result as byte array
   * @param pcData: Data to encode
   * @param uiLen:  Number of bytes in data to encode
   * @return Encoded data
   */
  virtual const CcByteArray &encode(const void* pcData, size_t uiLen);

  /**
   * @brief Decode data and get result as byte array if supported
   * @param pcData: Data to decode
   * @param uiLen:  Number of bytes in data to decode
   * @return Decoded data or empty if not supported
   */
  virtual const CcByteArray& decode(const void* pcData, size_t uiLen);

  /**
   * @brief Forward byte array to generation with buffers
   * @param oByteArray: ByteArray to add to generation
   * @return Handle to this
   */
  inline IHash& generateByteArray(const CcByteArray& oByteArray)
  { return generate(oByteArray.getArray(), oByteArray.size());}

  /**
   * @brief Forward byte array to append with buffers
   * @param oByteArray: ByteArray to append
   * @return Handle to this
   */
  inline IHash& appendByteArray(const CcByteArray& oByteArray)
  { return append(oByteArray.getArray(), oByteArray.size());}

  /**
   * @brief Forward byte array to finalize with buffers
   * @param oByteArray: ByteArray to finalize with
   * @return Handle to this
   */
  inline IHash& finalizeByteArray(const CcByteArray& oByteArray)
  { return finalize(oByteArray.getArray(), oByteArray.size());}

  /**
   * @brief Forward string to generation with buffers
   * @param oString: String to add to generation
   * @return Handle to this
   */
  inline IHash& generateString(const CcString& oString)
  { return generate(oString.getCharString(), oString.size());}

  /**
   * @brief Forward string to append with buffers
   * @param oString: String to append
   * @return Handle to this
   */
  inline IHash& appendString(const CcString& oString)
  { return append(oString.getCharString(), oString.size());}

  /**
   * @brief Forward string to finalize with buffers
   * @param oString: String to finalize with
   * @return Handle to this
   */
  inline IHash& finalizeString(const CcString& oString)
  { return finalize(oString.getCharString(), oString.size());}

  /**
   * @brief Finalize with no new buffer, just finish.
   * @return Handle to this
   */
  inline IHash& finalizeEmpty()
  { return finalize(nullptr, 0);}
};
