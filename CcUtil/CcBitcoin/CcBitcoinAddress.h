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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcBitcoinAddress
 */
#ifndef H_CcBitcoinAddress_H_
#define H_CcBitcoinAddress_H_

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

  CcBitcoinAddress(const CcString& sAddress);

  /**
   * @brief Destructor
   */
  ~CcBitcoinAddress();

  bool setAddress(const CcString& sAddress);
  const CcByteArray& getAddress() const
    { return m_aAddress; }
  CcString getAddressString() const;
  CcString getAddressHashString() const;

private:
  CcByteArray m_aAddress;
  CcByteArray m_aPrivateKey;
};

#endif // H_CcBitcoinAddress_H_
