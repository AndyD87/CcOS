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

#include "CcConsole.h"

bool CcSslControl::s_bIsInitialized = false;

bool CcSslControl::initSsl()
{
  if (s_bIsInitialized == false)
  {
    // Do not check return values of loading ssl
    // Not all versions of openss does support this.
    // Later it should be checked again with OPENSSL Version definitions
    int iSslInit = SSL_load_error_strings();
    if (iSslInit < 0)
    {

    }
    else
    {
      iSslInit = ERR_load_BIO_strings();
      if (iSslInit < 0)
      {

      }
      else
      {
        iSslInit = SSL_library_init();
        if (iSslInit < 0)
        {

        }
        else
        {
          iSslInit = OpenSSL_add_all_algorithms();
          if (iSslInit < 0)
          {

          }
          else
          {
            s_bIsInitialized = true;
          }
        }
      }
    }
  }
  return s_bIsInitialized;
}

bool CcSslControl::createCertFiles(const CcString& sCertFilePath, const CcString& sKeyFilePath)
{
  CcSslCertificate oCertificate;
  return oCertificate.createFiles(sCertFilePath, sKeyFilePath);
}

bool CcSslControl::createCertStrings(CcString& sCert, CcString& sKey)
{
  CcSslCertificate oCertificate;
  return oCertificate.createString(sCert, sKey);
}

CcString CcSslControl::getErrorString(uint32 uiErrorCode)
{
  CcString sError(ERR_lib_error_string(uiErrorCode));
  return sError;
}

CcByteArray CcSslControl::getBioData(bio_st* pBioData)
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

void CcSslControl::printAllErrors()
{
  ERR_print_errors_cb(CcSslControl::printCallback, nullptr);
}

int CcSslControl::printCallback(const char *str, size_t len, void* CCUNUSED_PARAM(pContext))
{
  CcConsole::writeLine(CcString(str, len));
  return 0;
}