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
 * @brief     Implemtation of class CcSslCertificate
 */
#include "CcSslCertificate.h"
#include "CcString.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include "CcSslControl.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <openssl/evp.h>
#include <openssl/asn1.h>
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

class CcSslCertificate::CPrivate
{
public:
  void cleanKeys()
  {
    //! @todo: Find a reason, why EVP_PKEY_free and RSA_free will SIGABRT
    if(pEvpPkey != nullptr)
      EVP_PKEY_free(pEvpPkey);
    if(pRsa != nullptr)
      RSA_free(pRsa);
    if(pX509Cert != nullptr)
      X509_free(pX509Cert);
    pX509Cert = nullptr;
    pEvpPkey  = nullptr;
    pRsa      = nullptr;
  }

public:
  CcString sName;
  X509*     pX509Cert = nullptr;
  EVP_PKEY* pEvpPkey  = nullptr;
  RSA*      pRsa      = nullptr;
};

CcSslCertificate::CcSslCertificate()
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcSslCertificate::~CcSslCertificate()
{
  m_pPrivate->cleanKeys();
  CCDELETE(m_pPrivate);
}

bool CcSslCertificate::create(const CcString& sCertFilePath, const CcString& sKeyFilePath)
{
  bool bRet = false;

  // remove stored keys
  m_pPrivate->cleanKeys();

  m_pPrivate->pX509Cert = X509_new();
  if(m_pPrivate->pX509Cert != nullptr)
  {
    m_pPrivate->pEvpPkey = EVP_PKEY_new();
    if(m_pPrivate->pEvpPkey != nullptr)
    {
      m_pPrivate->pRsa = RSA_new();
      if(m_pPrivate->pRsa != nullptr)
      {
        if(mkcert(2048, 0, 365))
        {
          CcByteArray oPrivateKeyPemBuffer;
          BIO *pBioBuffer = BIO_new(BIO_s_mem());
          if(pBioBuffer != nullptr)
          {
            bRet = true;
            if (PEM_write_bio_PrivateKey(pBioBuffer, m_pPrivate->pEvpPkey, NULL, NULL, 0, NULL, NULL))
            {
              CcByteArray oPemBuffer = CcSslControl::getBioData(pBioBuffer);
              oPrivateKeyPemBuffer = oPemBuffer;
              CcFile oKeyFile(sKeyFilePath);
              if (oKeyFile.open(EOpenFlags::Write))
              {
                oKeyFile.writeArray(oPemBuffer);
                oKeyFile.close();
              }
            }

            BIO_free(pBioBuffer);

            pBioBuffer = BIO_new(BIO_s_mem());

            if (PEM_write_bio_X509(pBioBuffer, m_pPrivate->pX509Cert))
            {
              CcByteArray oPemBuffer = CcSslControl::getBioData(pBioBuffer);
              CcFile oCertFile(sCertFilePath);
              if (oCertFile.open(EOpenFlags::Write))
              {
                oCertFile.writeArray(oPrivateKeyPemBuffer);
                oCertFile.writeArray(oPemBuffer);
                oCertFile.close();
              }
            }
            BIO_free(pBioBuffer);
          }
        }
      }
      else
      {
        bRet = false;
      }
    }
    else
    {
      bRet = false;
    }
  }
  else
  {
    bRet = false;
  }

  // if failed, no reason to keep keys in ram
  if(bRet == false)
  {
    m_pPrivate->cleanKeys();
  }
  return bRet;

}


int CcSslCertificate::add_ext(int nid, char *value)
{
  X509_EXTENSION *ex;
  X509V3_CTX ctx;
  // This sets the 'context' of the extensions. */
  // No configuration database */
  X509V3_set_ctx_nodb(&ctx);

  // Issuer and subject certs: both the target since it is self signed,
  // no request and no CRL
  X509V3_set_ctx(&ctx, m_pPrivate->pX509Cert, m_pPrivate->pX509Cert, NULL, NULL, 0);
  ex = X509V3_EXT_conf_nid(NULL, &ctx, nid, value);
  if (!ex)
    return 0;

  X509_add_ext(m_pPrivate->pX509Cert, ex, -1);
  X509_EXTENSION_free(ex);
  return 1;
}

bool CcSslCertificate::mkcert(int bits, int serial, int days)
{
  bool bRet = false;
  X509_NAME *name = NULL;
  if (m_pPrivate->pRsa == nullptr)
  {
    CCDEBUG("RSA not created");
  }
  else if (m_pPrivate->pX509Cert == nullptr)
  {
    CCDEBUG("ERROR wrong paramaters");
  }
  else if (m_pPrivate->pEvpPkey == nullptr)
  {
    CCDEBUG("ERROR wrong paramaters");
  }
  else
  {
#if (OPENSSL_VERSION_NUMBER < 0x10000000)
    rsa = RSA_generate_key(bits, RSA_F4, CcSslCertificate::callback, NULL);
#else
    BIGNUM *e;
    e = BN_new();
    BN_set_word(e, 65537);

    RSA_generate_key_ex(m_pPrivate->pRsa, bits, e, NULL);

    BN_free(e);
    e = NULL;
#endif
    if (EVP_PKEY_assign_RSA(m_pPrivate->pEvpPkey, m_pPrivate->pRsa))
    {
      X509_set_version(m_pPrivate->pX509Cert, 2);
      ASN1_INTEGER_set(X509_get_serialNumber(m_pPrivate->pX509Cert), serial);
      X509_gmtime_adj(X509_get_notBefore(m_pPrivate->pX509Cert), 0);
      X509_gmtime_adj(X509_get_notAfter(m_pPrivate->pX509Cert), (long) 60 * 60 * 24 * days);
      X509_set_pubkey(m_pPrivate->pX509Cert, m_pPrivate->pEvpPkey);

      name = X509_get_subject_name(m_pPrivate->pX509Cert);

      // This function creates and adds the entry, working out the
      // correct string type and performing checks on its length.
      // Normally we'd check the return value for errors...
      X509_NAME_add_entry_by_txt(name, "C",
        MBSTRING_ASC, (unsigned char*)"UK", -1, -1, 0);
      X509_NAME_add_entry_by_txt(name, "CN",
        MBSTRING_ASC, (unsigned char*)"OpenSSL Group", -1, -1, 0);

      // Its self signed so set the issuer name to be the same as the
      // subject.
      X509_set_issuer_name(m_pPrivate->pX509Cert, name);

      /* Add various extensions: standard extensions */
      add_ext(NID_basic_constraints, &CcString("critical,CA:TRUE")[0]);
      add_ext(NID_key_usage, &CcString("critical,keyCertSign,cRLSign")[0]);

      add_ext(NID_subject_key_identifier, &CcString("hash")[0]);

      /* Some Netscape specific extensions */
      add_ext(NID_netscape_cert_type, &CcString("sslCA")[0]);

      add_ext(NID_netscape_comment, &CcString("example comment extension")[0]);
      int iSize = X509_sign(m_pPrivate->pX509Cert, m_pPrivate->pEvpPkey, EVP_md5());
      if (iSize > 0)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

void CcSslCertificate::callback(int p, int n, void *arg)
{
  CCUNUSED(n);
  CCUNUSED(arg);
  char c = 'B';

  if (p == 0) c = '.';
  if (p == 1) c = '+';
  if (p == 2) c = '*';
  if (p == 3) c = '\n';
  fputc(c, stderr);
}
