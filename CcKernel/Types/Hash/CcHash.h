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
#ifndef _CcHash_H_
#define _CcHash_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcByteArray.h"

/**
 * @brief All known hashing algorithims within CcOS
 */
enum class EHashType
{
  Unknown = 0, //!< Unknown Hash Type
  Crc32,
  Md5,
  Sha256,
};

/**
 * @brief Abstract class for all hashing algorithms.
 */
class CcKernelSHARED CcHashAbstract
{
public:
  /**
   * @brief Destructor
   */
  virtual ~CcHashAbstract()
    {}

  /**
   * @brief Get calculated hash value as ByteArray.
   *        This Method must be supported from every implementing hash algorithm.
   * @return
   */
  virtual const CcByteArray& getValue() const = 0;
  virtual CcByteArray& getValue() = 0;

  /**
   * @brief Fully generate hash value from buffer.
   *        This method has to reinit if required an finalizes if required.
   * @param pcData: Data to generate hash from.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual CcHashAbstract& generate(const void* pcData, size_t uiLen) = 0;

  /**
   * @brief Append buffer to current object.
   * @param pcData: Data to append to current hash from.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual CcHashAbstract& append(const void* pcData, size_t uiLen) = 0;

  /**
   * @brief Finalize hash generating.
   *        After calling this method, the stored Value for getValue is valid.
   * @param pcData: Last data to append to hash value.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual CcHashAbstract& finalize(const void* pcData, size_t uiLen) = 0;
};

/**
 * @brief Create Crc32 Hashes with this class.
 *        It is addtionally possible to continue an older Crc32 Session.
 */
class CcKernelSHARED CcHash : public CcHashAbstract
{
public:
  /**
   * @brief Constructor
   */
  CcHash()
    {}

  /**
   * @brief Constructor with predefined Hashing type
   */
  CcHash(EHashType eHashType);

  /**
   * @brief Destructor
   */
  virtual ~CcHash();

  /**
   * @brief Fully generate hash value from buffer.
   *        This method has to reinit if required an finalizes if required.
   * @param pcData: Data to generate hash from.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual CcHash& generate(const void* pData, size_t uiSize) override;

  /**
   * @brief Append buffer to current object.
   * @param pcData: Data to append to current hash from.
   * @param uiLen: Size of pcData
   * @return Handle to this object
   */
  virtual CcHash& append(const void* pData, size_t uiSize) override;

  /**
   * @brief Finalize hash generating.
   *        After calling this method, the stored Value for getValue is valid.
   * @param pcData: Last data to append to hash value.
   * @return Handle to this object
   */
  virtual CcHash& finalize(const void* pData, size_t uiSize) override;

  /**
   * @brief Get calculated hash value as ByteArray.
   *        This Method must be supported from every implementing hash algorithm.
   * @return
   */
  virtual const CcByteArray& getValue() const override;
  virtual CcByteArray& getValue() override;

  /**
   * @brief Get currently set Hashtype
   * @return HashType as enum
   */
  EHashType getHashType() const;

  /**
   * @brief Set type of Hash algorithm to load to this Object.
   * @param eHashType: Type of Hash algorithm as enum
   * @return true if HashType was selected successfully
   */
  bool setHashType(EHashType eHashType);

  /**
   * @brief Set type of Hash algorithm to load to this Object.
   *        The type has to be associated with existing types in EHashType.
   *        The string will be compared caseinsensitive.
   * @param sHashType: Type of Hash algorithm as string.
   *                    Possible values can be found at CcGlobalStrings::Types::Hash
   * @return true if HashType was selected successfully
   */
  bool setHashType(const CcString& sHashType);

  /**
   * @brief Fully generate hash value from buffer.
   *        This method has to reinit if required an finalizes if required.
   * @param oByteArray: Data to generate hash from, stored in ByteArray.
   * @return Handle to this object
   */
  inline CcHash& generate(const CcByteArray& oByteArray)
    { return generate(oByteArray.getArray(), oByteArray.size());}

  /**
   * @brief Append buffer to current object.
   * @param oByteArray: Data to append, stored in ByteArray.
   * @return Handle to this object
   */
  CcHash& append(const CcByteArray& oByteArray)
    { return append(oByteArray.getArray(), oByteArray.size());}

  /**
   * @brief Finalize hash generating.
   *        After calling this method, the stored Value for getValue is valid.
   * @param pcData: Last data to append to hash value.
   * @return Handle to this object
   */
  inline CcHash& finalize(const CcByteArray& oByteArray)
    { return finalize(oByteArray.getArray(), oByteArray.size());}

  /**
   * @brief Finalize hash generating, and do not append any more data.
   */
  inline CcHash& finalize()
    { return finalize(nullptr, 0);}

private:
  EHashType m_eHashType = EHashType::Unknown; //!< Current object loaded at m_pHashObject
  CcHashAbstract* m_pHashObject = nullptr;    //!< HashObject with implemented hash algorithm. It is set to nullptr if m_eHashType is EHashType::Unknown.
};


#endif /* _CcHash_H_ */
