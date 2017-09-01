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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcSslControl
 */
#include "CcSslControl.h"
#include "CcString.h"
#include "CcFile.h"
#include "CcByteArray.h"
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

#define MemBioCreate()    BIO_new(BIO_s_mem())
#define MemBioDelete(Bio) BIO_free(Bio)
#define MemBioReset(Bio)  MemBioDelete(Bio); Bio = MemBioCreate();

bool CcSslControl::s_bIsInitialized = false;

int mkcert(X509 **x509p, EVP_PKEY **pkeyp, int bits, int serial, int days);
int add_ext(X509 *cert, int nid, char *value);


CcSslControl::CcSslControl( void )
{
}

CcSslControl::~CcSslControl( void )
{
}

bool CcSslControl::initSsl()
{
  bool bRet = false;
  if (s_bIsInitialized == false)
  {
    SSL_library_init();
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    s_bIsInitialized = true;
  }
  else
  {
    bRet = true;
  }
  return bRet;
}

bool CcSslControl::createCert(const CcString& sCertFilePath, const CcString& sKeyFilePath)
{
  // on commandline for comparision:
  //openssl req -config cnf/openssl.cnf -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 10
  bool bRet = true;
  BIO *pBioBuffer = MemBioCreate();
  X509 *x509 = NULL;
  EVP_PKEY *pkey = NULL;

  mkcert(&x509, &pkey, 2048, 0, 365);

  CcByteArray oPrivateKeyPemBuffer;
  if (PEM_write_bio_PrivateKey(pBioBuffer, pkey, NULL, NULL, 0, NULL, NULL))
  {
    CcByteArray oPemBuffer = getBioData(pBioBuffer);
    oPrivateKeyPemBuffer = oPemBuffer;
    CcFile oKeyFile(sKeyFilePath);
    if (oKeyFile.open(EOpenFlags::Write))
    {
      oKeyFile.writeArray(oPemBuffer);
      oKeyFile.close();
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
  MemBioReset(pBioBuffer);

  if (PEM_write_bio_X509(pBioBuffer, x509))
  {
    CcByteArray oPemBuffer = getBioData(pBioBuffer);
    CcFile oCertFile(sCertFilePath);
    if (oCertFile.open(EOpenFlags::Write))
    {
      oCertFile.writeArray(oPrivateKeyPemBuffer);
      oCertFile.writeArray(oPemBuffer);
      oCertFile.close();
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
  MemBioDelete(pBioBuffer);

  X509_free(x509);
  EVP_PKEY_free(pkey);

  return bRet;
}

#if (OPENSSL_VERSION_NUMBER < 0x10000000)
static void callback(int p, int n, void *arg)
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
#endif

CcString CcSslControl::getErrorString(uint32 uiErrorCode)
{
  CcString sError(ERR_lib_error_string(uiErrorCode));
  return sError;
}

CcByteArray CcSslControl::getBioData(BIO * pBioData)
{
  CcByteArray oByteArray;
  char buffer[1024];
  int size = BIO_read(pBioData, buffer, 1024);
  while (size > 0)
  {
    oByteArray.append(buffer, size);
    size = BIO_read(pBioData, buffer, 1024);
  }
  return oByteArray;
}



int mkcert(X509 **x509p, EVP_PKEY **pkeyp, int bits, int serial, int days)
{
  X509 *x;
  EVP_PKEY *pk;
  RSA* rsa = RSA_new();
  X509_NAME *name = NULL;

  if ((pkeyp == NULL) || (*pkeyp == NULL))
  {
    if ((pk = EVP_PKEY_new()) == NULL)
    {
      return(0);
    }
  }
  else
    pk = *pkeyp;

  if ((x509p == NULL) || (*x509p == NULL))
  {
    if ((x = X509_new()) == NULL)
      goto err;
  }
  else
    x = *x509p;

#if (OPENSSL_VERSION_NUMBER < 0x10000000)
  rsa = RSA_generate_key(bits, RSA_F4, callback, NULL);
#else
  BIGNUM *e;
  e = BN_new();
  BN_set_word(e, 65537);

  RSA_generate_key_ex(rsa, bits, e, NULL);

  BN_free(e);
  e = NULL;
#endif
  if (!EVP_PKEY_assign_RSA(pk, rsa))
  {
    abort();
    goto err;
  }
  rsa = NULL;

  X509_set_version(x, 2);
  ASN1_INTEGER_set(X509_get_serialNumber(x), serial);
  X509_gmtime_adj(X509_get_notBefore(x), 0);
  X509_gmtime_adj(X509_get_notAfter(x), (long) 60 * 60 * 24 * days);
  X509_set_pubkey(x, pk);

  name = X509_get_subject_name(x);

  /* This function creates and adds the entry, working out the
  * correct string type and performing checks on its length.
  * Normally we'd check the return value for errors...
  */
  X509_NAME_add_entry_by_txt(name, "C",
    MBSTRING_ASC, (unsigned char*)"UK", -1, -1, 0);
  X509_NAME_add_entry_by_txt(name, "CN",
    MBSTRING_ASC, (unsigned char*)"OpenSSL Group", -1, -1, 0);

  /* Its self signed so set the issuer name to be the same as the
  * subject.
  */
  X509_set_issuer_name(x, name);

  /* Add various extensions: standard extensions */
  add_ext(x, NID_basic_constraints, &CcString("critical,CA:TRUE")[0]);
  add_ext(x, NID_key_usage, &CcString("critical,keyCertSign,cRLSign")[0]);

  add_ext(x, NID_subject_key_identifier, &CcString("hash")[0]);

  /* Some Netscape specific extensions */
  add_ext(x, NID_netscape_cert_type, &CcString("sslCA")[0]);

  add_ext(x, NID_netscape_comment, &CcString("example comment extension")[0]);


#ifdef CUSTOM_EXT
  /* Maybe even add our own extension based on existing */
  {
    int nid;
    nid = OBJ_create("1.2.3.4", "MyAlias", "My Test Alias Extension");
    X509V3_EXT_add_alias(nid, NID_netscape_comment);
    add_ext(x, nid, "example comment alias");
  }
#endif

  if (!X509_sign(x, pk, EVP_md5()))
    goto err;
  RSA_free(rsa);
  *x509p = x;
  *pkeyp = pk;
  return(1);
err:
  return(0);
}

/**
 * Add extension using V3 code: we can set the config file as NULL
 * because we wont reference any other sections.
 */
int add_ext(X509 *cert, int nid, char *value)
{
  X509_EXTENSION *ex;
  X509V3_CTX ctx;
  /* This sets the 'context' of the extensions. */
  /* No configuration database */
  X509V3_set_ctx_nodb(&ctx);
  /* Issuer and subject certs: both the target since it is self signed,
  * no request and no CRL
  */
  X509V3_set_ctx(&ctx, cert, cert, NULL, NULL, 0);
  ex = X509V3_EXT_conf_nid(NULL, &ctx, nid, value);
  if (!ex)
    return 0;

  X509_add_ext(cert, ex, -1);
  X509_EXTENSION_free(ex);
  return 1;
}
