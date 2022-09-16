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
#include "IHash.h"

/**
 * @brief Create Crc32 Hashes with this class.
 *        It is addtionally possible to continue an older Crc32 Session.
 */
class CcKernelSHARED CcHmac : public IHash
{
public:
  /**
   * @brief Constructor
   */
  CcHmac();

  /**
   * @brief Constructor with predefined Hashing type
   * @param eHashType: Target Hashtype to use
   */
  CcHmac(EHashType eHashType);
  
  /**
   * @brief Constructor with predefined Hashing type
   * @param pHashAlgorithm: Predefined Hash algorithm to user
   */
  CcHmac(IHash* pHashAlgorithm);
  
  /**
   * @brief ~CcHmac
   */
  virtual ~CcHmac();

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcHmac& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcHmac& oToCompare) const;

  /**
   * @brief Directly generate hash from ByteArray
   * @param oByteArray: ByteArray to generate hash from
   * @return this
   */
  CcHmac& operator=(const CcByteArray& oByteArray);

  /**
   * @brief Directly generate hash from String
   * @param sString: String to generate hash from
   * @return this
   */
  CcHmac& operator=(const CcString& sString);

  /**
   * @brief Get current stored result value;
   * @return reference to stored result.
   */
  virtual const CcByteArray& getValue() override
  {
    return m_oResult;
  }

  /**
   * @brief Get result as Hex-String
   * @return resulting string or empty if failed
   */
  CcString getHexString() const;

  /**
   * @brief Overwrite result with new hexstring
   * @param sHexString: Hex data to store in result
   */
  void setHexString(const CcString& sHexString);

  /**
   * @brief Set secret value for hmac
   * @param sSecretString: Secret data
   * @return true if data was set successfully
   */
  bool setSecretKey(const CcString& sSecretString);
  
  virtual IHash& setKey(const void* pcData, size_t uiLen) override
  { m_oSecret.set(static_cast<const char*>(pcData), uiLen); return *this; }

  /**
   * @brief Set target Hash Algorthim for this hmac
   * @param eAlgorithm: Algorithm defined as enum
   * @return Return can be one of the following
   *         - EStatus::Success if Algoritm was set successfully
   *         - EStatus::Error if initalizing failed.
   *         - EStatus::CommandInvalidParameter if eAlgorithm was a invalid or not implemented value
   */
  CcStatus setHashAlgorithm(EHashType eAlgorithm);

  virtual CcHmac& generate(const void *data, size_t size) override;
  virtual CcHmac& append(const void *data, size_t size) override;
  virtual CcHmac& finalize(const void *data, size_t size) override;
  virtual size_t getBlockSize() override;
  virtual size_t getHashSize() override;

private: // methods
  void initValues();
  void doHmac(CcByteArray& rPad, const void *pData, size_t uiSize, CcByteArray& oOut);
private: // Types
  class CPrivate;
private: // Member
  IHash*      m_pHash = nullptr;
  bool        m_bOwner = true;
  CcByteArray m_oSecret;
  CcByteArray m_oResult;
  EHashType   m_eHashType = EHashType::Unknown;
};
