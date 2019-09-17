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
 * @page      CcHash
 * @subpage   Hash
 *
 * @page      CcHash
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Types for making exchanges more efficient or simple.
 */
#ifndef H_IHash_H_
#define H_IHash_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcByteArray.h"

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
   * @return
   */
  virtual const CcByteArray& getValue() const = 0;
  //! @copydoc
  virtual CcByteArray& getValue() = 0;

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
};

#endif // _IHash_H_
