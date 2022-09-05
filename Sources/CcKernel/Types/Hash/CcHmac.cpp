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
 * @brief     Class CcHmac
 *            Implementation like https://github.com/h5p9sl/hmac_sha256
 */
#include "CcHmac.h"
#include "Hash/CcHash.h"
#include "CcStatic.h"

CcHmac::CcHmac()
{
  initValues();
}

CcHmac::CcHmac(EHashType eType) :
  CcHmac()
{
  setHashAlgorithm(eType);
}

CcHmac::CcHmac(IHash* pHash) :
  m_pHash(pHash),
  m_bOwner(false)
{
}

CcHmac::~CcHmac()
{
  if(m_bOwner) CCDELETE(m_pHash);
}

bool CcHmac::operator==(const CcHmac& oToCompare) const
{
  return m_oResult == oToCompare.m_oResult;
}

bool CcHmac::operator!=(const CcHmac& oToCompare) const
{
  return m_oResult != oToCompare.m_oResult;
}

CcHmac& CcHmac::operator=(const CcByteArray& oByteArray)
{
  return generate(oByteArray.getArray(), oByteArray.size());
}

CcHmac& CcHmac::operator=(const CcString& sHexString)
{
  return generate(sHexString.getCharString(), sHexString.length());
}

CcString CcHmac::getHexString() const
{
  return m_oResult.getHexString();
}

void CcHmac::setHexString(const CcString& sHexString)
{
  m_oResult.setHexString(sHexString);
}

bool CcHmac::setSecretKey(const CcString& sSecretString)
{
  bool bRet = false;
  if (sSecretString.length() > 0)
  {
    bRet = true;
    m_oSecret = sSecretString;
  }
  return bRet;
}

CcHmac& CcHmac::generate(const void *pData, size_t uiSize)
{
  initValues();
  return finalize(pData, uiSize);
}

CcHmac& CcHmac::append(const void *pData, size_t uiSize)
{
  return generate(pData, uiSize);
}

CcStatus CcHmac::setHashAlgorithm(EHashType eAlgorithm)
{
  CcStatus bRet = false;
  if(m_bOwner) CCDELETE(m_pHash);
  CCNEWTYPE(pHash, CcHash);
  if (pHash->setHashType(eAlgorithm))
  {
    m_pHash = pHash;
    m_oResult.resize(m_pHash->getHashSize());
  }
  else
  {
    CCDELETE(pHash);
  }

  return bRet;
}

CcHmac& CcHmac::finalize(const void *pData, size_t uiSize)
{
  CcByteArray k(getBlockSize());
  CcByteArray k_ipad(getBlockSize());
  CcByteArray k_opad(getBlockSize());
  CcByteArray ihash(getHashSize());
  CcByteArray ohash(getHashSize());
  size_t sz;

  CcStatic::memset(k.getArray(), 0, sizeof(k));
  CcStatic::memset(k_ipad.getArray(), 0x36, getBlockSize());
  CcStatic::memset(k_opad.getArray(), 0x5c, getBlockSize());

  if (m_oSecret.size() > getBlockSize()) 
  {
    // If the key is larger than the hash algorithm's
    // block size, we must digest it first.
    m_pHash->generate(m_oSecret.getArray(), m_oSecret.size());
    k = m_pHash->getValue();
  }
  else 
  {
    k.write(m_oSecret.getArray(), m_oSecret.size());
  }

  for (size_t i = 0; i < getBlockSize(); i++) 
  {
    k_ipad[i] ^= k[i];
    k_opad[i] ^= k[i];
  }

  // Perform HMAC algorithm: ( https://tools.ietf.org/html/rfc2104 )
  //      `H(K XOR opad, H(K XOR ipad, data))`
  doHmac(k_ipad, pData, uiSize, ihash);
  doHmac(k_opad, ihash.getArray(), ihash.size(), ohash);

  sz = (m_oResult.size() > getHashSize()) ? getHashSize() : m_oResult.size();
  m_oResult.write(ohash.getArray(), sz);
  return *this;
}

void CcHmac::initValues()
{
}

void CcHmac::doHmac(CcByteArray& rPad, const void *pData, size_t uiSize, CcByteArray& oOut)
{
  CcByteArray oBuf;
  oBuf.resize(rPad.size() + uiSize);

  oBuf.write(rPad.getArray(), rPad.size());
  oBuf.write(pData, uiSize, rPad.size());

  m_pHash->generate(oBuf.getArray(), oBuf.size());
  oOut = m_pHash->getValue();
}

size_t CcHmac::getBlockSize()
{
  if (m_pHash)
  {
    return m_pHash->getBlockSize();
  }
  return 0;
}

size_t CcHmac::getHashSize()
{
  if (m_pHash)
  {
    return m_pHash->getHashSize();
  }
  return 0;
}