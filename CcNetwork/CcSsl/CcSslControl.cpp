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
 * @brief     Implemtation of class CcSslControl
 */
#include "CcSslControl.h"
#include "CcString.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include "CcSslCertificate.h"
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

class CcSslControlPrivate
{
public:
};

bool CcSslControl::s_bIsInitialized = false;

CcSslControl::CcSslControl()
{
}

CcSslControl::~CcSslControl()
{
}

bool CcSslControl::initSsl()
{
  if (s_bIsInitialized == false)
  {
      // Do not check return values of loading ssl
      // Not all versions of openss does support this.
      // Later it should be checked again with OPENSSL Version definitions
      SSL_library_init();
      ERR_load_BIO_strings();
      SSL_load_error_strings();
      OpenSSL_add_all_algorithms();
      s_bIsInitialized = true;
  }
  return s_bIsInitialized;
}

bool CcSslControl::createCert(const CcString& sCertFilePath, const CcString& sKeyFilePath)
{
  CcSslCertificate oCertificate;
  return oCertificate.create(sCertFilePath, sKeyFilePath);
}

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
  while (size > 0 && size <= 1024)
  {
    oByteArray.append(buffer, size);
    size = BIO_read(pBioData, buffer, 1024);
  }
  return oByteArray;
}
