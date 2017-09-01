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
 * @brief     Implemtation of class CcSslData
 */
#include "CcSslData.h"
#include "CcString.h"

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/opensslconf.h>

CcSslData::CcSslData( void )
{
}

CcSslData::~CcSslData( void )
{
}

bool CcSslData::initClient()
{
  bool bRet = false;
  /* Set up the SSL context */
  m_pSslCtx = SSL_CTX_new(SSLv23_client_method());
  if (m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pSslCtx = SSL_CTX_new(TLSv1_1_client_method());
  }
  if (m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pSslCtx = SSL_CTX_new(TLSv1_client_method());
  }
#ifndef OPENSSL_NO_SSL3
  if (m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pSslCtx = SSL_CTX_new(SSLv3_client_method());
  }
#endif
  if (m_pSslCtx != nullptr)
    bRet = true;
  return bRet;
}

bool CcSslData::initServer()
{
  bool bRet = false;
#if OPENSSL_VERSION_NUMBER < 0x10100000
  m_pSslCtx = SSL_CTX_new(SSLv23_server_method());
  if (m_pSslCtx == nullptr)
  {
    ERR_print_errors_fp(stdout);
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pSslCtx = SSL_CTX_new(TLSv1_1_server_method());
  }
  if (m_pSslCtx == nullptr)
  {
    ERR_print_errors_fp(stdout);
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pSslCtx = SSL_CTX_new(TLSv1_server_method());
  }
#ifndef OPENSSL_NO_SSL3
  if (m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pSslCtx = SSL_CTX_new(SSLv3_server_method());
  }
#endif
#else
  m_pSslCtx = SSL_CTX_new(TLS_server_method());
#endif
  if (m_pSslCtx == nullptr)
  {
    bRet = false;
  }
  else
  {
    bRet = true;
  }
  return bRet;
}

bool CcSslData::loadCertificate(const CcString& sCertificateFile)
{
  bool bRet = true;
  if (SSL_CTX_use_certificate_file(m_pSslCtx, sCertificateFile.getOsPath().getCharString(), SSL_FILETYPE_PEM) != 1)
  {
    uint32 uiError = ERR_get_error();
    char pcErrorBuf[256];
    ERR_error_string(uiError, pcErrorBuf);
    CCDEBUG(CcString("SSL-Error: ") + pcErrorBuf + " Nr: " + CcString::fromNumber(uiError));
    bRet = false;
  }
  return bRet;
}

bool CcSslData::loadKey(const CcString& sKeyFile)
{
  bool bRet = true;
  if (SSL_CTX_use_PrivateKey_file(m_pSslCtx, sKeyFile.getOsPath().getCharString(), SSL_FILETYPE_PEM) != 1)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    bRet = false;
  }
  return bRet;
}