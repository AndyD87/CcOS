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
 * @brief     Class CcBitcoinAddress
 */
#pragma once

#include "CcBitcoin.h"
#include "CcBase.h"
#include "CcByteArray.h"

class CcString;

/**
 * @brief Example Class implementation
 */
class CcBitcoinSHARED CcBitcoinAddress {
public:
  /**
   * @brief Constructor
   */
  CcBitcoinAddress();

  /**
   * @brief Import bitcoin address and store it binary
   * @param sAddress: Address to convert to byte array
   */
  CcBitcoinAddress(const CcString& sAddress);

  /**
   * @brief Destructor
   */
  ~CcBitcoinAddress();

  /**
   * @brief Import bitcoin address and store it binary
   * @param sAddress: Address to convert to byte array
   * @return True if conversion succeeded
   */
  bool setAddress(const CcString& sAddress);

  /**
   * @brief Get address as binary buffer
   * @return ByteArray with converted address
   */
  const CcByteArray& getAddress() const
  { return m_aAddress; }
  //! @return Get address as Base58 encoded String
  CcString getAddressString() const;
  //! @return Get address as hex string from binary address
  CcString getAddressHashString() const;

private:
  CcByteArray m_aAddress;
  CcByteArray m_aPrivateKey;
};
