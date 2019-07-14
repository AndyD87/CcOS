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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSslHmac
 */
#include "Hash/CcSslHmac.h"
#include "CcByteArray.h"
#include "CcString.h"
#include "CcStatic.h"
#include "openssl/hmac.h"

#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || defined (LIBRESSL_VERSION_NUMBER)
#include <openssl/crypto.h>
/**
 * @brief Implementation from newer openssl version.
 * @param a clean HMAC context struct, or nullptr if failed
 */
static HMAC_CTX *HMAC_CTX_new()
{
  HMAC_CTX *ctx = static_cast<HMAC_CTX*>(OPENSSL_malloc(sizeof(*ctx)));
  if (ctx != nullptr)
    HMAC_CTX_init(ctx);
  return ctx;
}

static void HMAC_CTX_free(HMAC_CTX *ctx)
{
   if (ctx != nullptr) {
       HMAC_CTX_cleanup(ctx);
       OPENSSL_free(ctx);
   }
}
#endif // (OPENSSL_VERSION_NUMBER < 0x10100000L) || defined (LIBRESSL_VERSION_NUMBER

class CcSslHmac::CPrivate
{
public:
  CPrivate()
  {
    pSslHmacCtx = HMAC_CTX_new();
    CCMONITORNEW(pSslHmacCtx);
  }
  ~CPrivate()
  {
    CCMONITORDELETE(pSslHmacCtx);
    HMAC_CTX_free(pSslHmacCtx);
  }

  HMAC_CTX* pSslHmacCtx;
};

CcSslHmac::CcSslHmac():
  m_oResult(16)
{
  CCNEW(m_pPrivate, CPrivate);
  initValues();
}

CcSslHmac::~CcSslHmac()
{
  CCDELETE(m_pPrivate);
}

bool CcSslHmac::operator==(const CcSslHmac& oToCompare) const
{
  return m_oResult == oToCompare.m_oResult;
}

bool CcSslHmac::operator!=(const CcSslHmac& oToCompare) const
{
  return m_oResult != oToCompare.m_oResult;
}

CcSslHmac& CcSslHmac::operator=(const CcByteArray& oByteArray)
{
  return generate(oByteArray.getArray(), oByteArray.size());
}

CcSslHmac& CcSslHmac::operator=(const CcString& sHexString)
{
  return generate(sHexString.getCharString(), sHexString.length());
}

CcString CcSslHmac::getHexString() const
{
  return m_oResult.getHexString();
}

void CcSslHmac::setHexString(const CcString& sHexString)
{
  m_oResult.setHexString(sHexString);
}

bool CcSslHmac::setSecretKey(const CcString& sSecretString)
{
  bool bRet = false;
  if (sSecretString.length() > 0)
  {
    bRet = true;
    m_oSecret = sSecretString;
  }
  return bRet;
}

CcStatus CcSslHmac::setHashAlgorithm(ESslHashAlgoritm eAlgorithm)
{
  CcStatus bRet = true;
  int iRet = -1;
  switch (eAlgorithm)
  {
    case ESslHashAlgoritm::SHA1:
      iRet = HMAC_Init_ex(m_pPrivate->pSslHmacCtx, m_oSecret.getArray(), static_cast<int>(m_oSecret.size()), EVP_sha1(), NULL);
      m_oResult.resize(20);
      break;
    case ESslHashAlgoritm::SHA224:
      iRet = HMAC_Init_ex(m_pPrivate->pSslHmacCtx, m_oSecret.getArray(), static_cast<int>(m_oSecret.size()), EVP_sha224(), NULL);
      m_oResult.resize(28);
      break;
    case ESslHashAlgoritm::SHA256:
      iRet = HMAC_Init_ex(m_pPrivate->pSslHmacCtx, m_oSecret.getArray(), static_cast<int>(m_oSecret.size()), EVP_sha256(), NULL);
      m_oResult.resize(32);
      break;
    case ESslHashAlgoritm::SHA384:
      iRet = HMAC_Init_ex(m_pPrivate->pSslHmacCtx, m_oSecret.getArray(), static_cast<int>(m_oSecret.size()), EVP_sha384(), NULL);
      m_oResult.resize(48);
      break;
    case ESslHashAlgoritm::SHA512:
      iRet = HMAC_Init_ex(m_pPrivate->pSslHmacCtx, m_oSecret.getArray(), static_cast<int>(m_oSecret.size()), EVP_sha512(), NULL);
      m_oResult.resize(64);
      break;
    default:
      bRet.setError(EStatus::CommandInvalidParameter);
      break;
  }
  if(iRet == 0)
  {
    bRet = false;
  }
  else if(iRet > 0)
  {
    bRet = true;
  }
  return bRet;
}

CcSslHmac& CcSslHmac::generate(const void *pData, size_t uiSize)
{
  initValues();
  return finalize(pData, uiSize);
}

CcSslHmac& CcSslHmac::append(const void *pData, size_t uiSize)
{
  HMAC_Update(m_pPrivate->pSslHmacCtx, static_cast<const unsigned char*>(pData), uiSize);
  return *this;
}

CcSslHmac& CcSslHmac::finalize(const void *pData, size_t uiSize)
{
  HMAC_Update(m_pPrivate->pSslHmacCtx, static_cast<const unsigned char*>(pData), uiSize);
  unsigned int uiLen;
  HMAC_Final(m_pPrivate->pSslHmacCtx, CCVOIDPTRCAST(unsigned char*, m_oResult.getArray()) , &uiLen);
  return *this;
}

void CcSslHmac::initValues()
{
}
