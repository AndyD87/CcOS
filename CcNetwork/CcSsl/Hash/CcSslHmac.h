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
 * @page      CcSsl
 * @subpage   CcSslHmac
 *
 * @page      CcSslHmac
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSslHmac
 */
#ifndef _CcSslHmac_H_
#define _CcSslHmac_H_

#include "CcBase.h"
#include "CcSsl.h"
#include "CcByteArray.h"
#include "Types/Hash/CcHash.h"
#include "ESslHashAlgorithm.h"

class CcString;

class CcSslHmacPrivate;

/**
 * @brief The CcSslHmac is a Hash class wich will be able to
 *        hash data data with a secret and different hash algortihms.
 *
 *        CcSslHmacs functionality is based on openssls HMAC_xxx methods
 */
class CcSslSHARED CcSslHmac : public CcHashAbstract
{
public:
  /**
   * @brief Constructor for a Hmac Object
   */
  CcSslHmac();

  /**
   * @brief ~CcSslHmac
   */
  virtual ~CcSslHmac();

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSslHmac& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSslHmac& oToCompare) const;

  /**
   * @brief Directly generate hash from ByteArray
   * @param oByteArray: ByteArray to generate hash from
   * @return this
   */
  CcSslHmac& operator=(const CcByteArray& oByteArray);

  /**
   * @brief Directly generate hash from String
   * @param sString: String to generate hash from
   * @return this
   */
  CcSslHmac& operator=(const CcString& sString);

  /**
   * @brief getValue Get current stored result value;
   * @return reference to stored result.
   */
  virtual const CcByteArray& getValue() const override
   { return m_oResult; }
  //! @copydoc CcSslHmac::getValue
  virtual CcByteArray& getValue() override
   { return m_oResult; }

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

  /**
   * @brief Set target Hash Algorthim for this hmac
   * @param eAlgorithm: Algorithm defined as enum
   * @return Return can be one of the following
   *         - EStatus::Success if Algoritm was set successfully
   *         - EStatus::Error if initalizing failed.
   *         - EStatus::CommandInvalidParameter if eAlgorithm was a invalid or not implemented value
   */
  CcStatus setHashAlgorithm(ESslHashAlgoritm eAlgorithm);

  //! @copydoc
  virtual CcSslHmac& generate(const void *data, size_t size) override;

  //! @copydoc
  virtual CcSslHmac& append(const void *data, size_t size) override;

  //! @copydoc
  virtual CcSslHmac& finalize(const void *data, size_t size) override;
  
  inline CcSslHmac& generate(const CcByteArray& oByteArray)
    { return generate(oByteArray.getArray(), oByteArray.size());}
  inline CcSslHmac& append(const CcByteArray& oByteArray)
    { return append(oByteArray.getArray(), oByteArray.size());}
  inline CcSslHmac& finalize(const CcByteArray& oByteArray)
    { return finalize(oByteArray.getArray(), oByteArray.size());}
  inline CcSslHmac& finalize()
    { return finalize(nullptr, 0);}

private:
  void initValues();
private:
  CcSslHmacPrivate* m_pPrivate = nullptr;
  CcByteArray m_oSecret;
  CcByteArray m_oResult;
};

#endif /* _CcSslHmac_H_ */
