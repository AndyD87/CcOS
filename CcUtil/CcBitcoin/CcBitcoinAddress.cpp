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
 * @brief     Implementation of Class CcBitcoinAddress
 */
#include "CcBitcoinAddress.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "Hash/CcSha256.h"

CcBitcoinAddress::CcBitcoinAddress()
{
}

CcBitcoinAddress::CcBitcoinAddress(const CcString& sAddress)
{
  setAddress(sAddress);
}

CcBitcoinAddress::~CcBitcoinAddress()
{
}

bool CcBitcoinAddress::setAddress(const CcString&  sAddress)
{
  bool bRet = false;
  if (sAddress.length() == 40)
  {
    m_aAddress.setHexString(sAddress);
    bRet = true;
  }
  else
  {
    m_aAddress = CcStringUtil::decodeBase58(sAddress);
    CcByteArray oChecksum(m_aAddress.getArray() + (m_aAddress.size() - 4), 4);
    m_aAddress.remove(m_aAddress.size() - 4, 4);
    if (m_aAddress[0] == 0)
    {
      // Version 1
      // @todo checksum
      CcSha256 oShaChecksum;
      oShaChecksum.generateByteArray(m_aAddress);
      CcSha256 oShaChecksum1;
      oShaChecksum1.generateByteArray(oShaChecksum.getValue());
      const CcByteArray& rChecksum = oShaChecksum1.getValue();
      if( rChecksum[0] == oChecksum[0] &&
          rChecksum[1] == oChecksum[1] &&
          rChecksum[2] == oChecksum[2] &&
          rChecksum[3] == oChecksum[3])
      {
        bRet = true;
      }
      m_aAddress.remove(1);
    }
  }
  return bRet;
}

CcString CcBitcoinAddress::getAddressString() const
{
  return m_aAddress.getHexString();
}

CcString CcBitcoinAddress::getAddressHashString() const
{
  return CcStringUtil::encodeBase58(m_aAddress);
}
