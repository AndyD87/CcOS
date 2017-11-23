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
 * @file      CcSslSocket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcSslSocket
 */
#include "CcSslSocket.h"
#include "CcKernel.h"
#include "CcSslControl.h"
#include "CcKernel.h"

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/opensslconf.h>
#include "CcSharedPointer.h"

class CcSslSocketPrivate
{
public:
  CcHandle<ssl_st>      m_pSsl = nullptr;
  CcHandle<ssl_ctx_st>  m_pSslCtx = nullptr;
  bool                  m_bSslCtxOwner = false;
  CcSharedPointer<CcSocketAbstract>  m_pParentSocket = nullptr;
};

CcSslSocket::CcSslSocket(void) : 
  CcSocketAbstract(ESocketType::TCP)
{
  m_pPrivate = new CcSslSocketPrivate;
  CCMONITORNEW(m_pPrivate);
  CcSslControl::initSsl();
}

CcSslSocket::CcSslSocket(CcSocketAbstract* pParentSocket) :
  CcSocketAbstract(ESocketType::TCP)
{
  m_pPrivate = new CcSslSocketPrivate;
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->m_pParentSocket = pParentSocket;
  CcSslControl::initSsl();
}

CcSslSocket::~CcSslSocket(void) 
{
  if (m_pPrivate != nullptr)
  {
    deinit();
    CCMONITORDELETE(m_pPrivate);
    delete m_pPrivate;
    m_pPrivate = nullptr;
  }
}

CcStatus CcSslSocket::open(EOpenFlags)
{
  return false;
}

CcStatus CcSslSocket::close()
{
  m_pPrivate->m_pParentSocket->close();
  if (m_pPrivate->m_pSsl != nullptr)
  {
    CCMONITORDELETE(m_pPrivate->m_pSsl);
    SSL_free(m_pPrivate->m_pSsl.ptr());
    m_pPrivate->m_pSsl = nullptr;
  }
  return true;
}

CcStatus CcSslSocket::cancel(void)
{
  close();
  return true;
}

size_t CcSslSocket::write(const void *buf, size_t bufSize)
{
  size_t uiReturn;
  /* Send the request */
  int uiSendNumber = SSL_write(m_pPrivate->m_pSsl.ptr(), buf, (int)bufSize);
  if (uiSendNumber < 0)
  {
    uiReturn = SIZE_MAX;
#ifdef DEBUG
    char string1000[1000];
    int iErrorNr = SSL_get_error(m_pPrivate->m_pSsl.ptr(), uiSendNumber);
    ERR_error_string_n(iErrorNr, string1000, 1000);
    CCDEBUG("SSL write error: " + CcString(string1000) + " No. " + CcString::fromNumber(iErrorNr));
#endif
  }
  else
  {
    uiReturn = (size_t)uiSendNumber;
  }
  return uiReturn;
}

size_t CcSslSocket::read(void *buf, size_t bufSize)
{
  size_t uiReturn;
  int uiReadNumber = SSL_read(m_pPrivate->m_pSsl.ptr(), buf, (int)bufSize);
  if (uiReadNumber < 0)
  {
    uiReturn = SIZE_MAX;
#ifdef DEBUG
    char string1000[1000];
    int iErrorNr = SSL_get_error(m_pPrivate->m_pSsl.ptr(), uiReadNumber);
    ERR_error_string_n(iErrorNr, string1000, 1000);
    CCDEBUG("SSL read error: " + CcString(string1000) + " No. " + CcString::fromNumber(iErrorNr));
#endif
  }
  else
  {
    uiReturn = (size_t)uiReadNumber;
  }
  return uiReturn;
}

CcStatus CcSslSocket::bind(const CcSocketAddressInfo& oAddrInfo)
{
  m_pPrivate->m_pParentSocket = CcKernel::getSocket(ESocketType::TCP);
  return m_pPrivate->m_pParentSocket->bind(oAddrInfo);
}

CcStatus CcSslSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  bool bRet = false;
  if (m_pPrivate->m_pSslCtx != nullptr)
  {
    /* Setup the connection */
    m_pPrivate->m_pParentSocket = CcKernel::getSocket(ESocketType::TCP);
    if (m_pPrivate->m_pParentSocket->connect(oAddressInfo))
    {
      m_pPrivate->m_pSsl = SSL_new(m_pPrivate->m_pSslCtx.ptr());
      CCMONITORNEW(m_pPrivate->m_pSsl.ptr());
      SSL_set_fd(m_pPrivate->m_pSsl.ptr(), m_pPrivate->m_pParentSocket->getSocketFD());
      int iSocketReturn = SSL_connect(m_pPrivate->m_pSsl.ptr());
      if (iSocketReturn == -1)
      {
        bRet = false;
      }
      else
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

CcStatus CcSslSocket::listen(void)
{
  return m_pPrivate->m_pParentSocket->listen();
}

CcSocketAbstract* CcSslSocket::accept(void)
{
  CcSslSocket* newSocket = nullptr;
  CcSocketAbstract* ClientSocket = m_pPrivate->m_pParentSocket->accept();
  if (ClientSocket != nullptr)
  {
    newSocket = new CcSslSocket(ClientSocket);
    CCMONITORNEW(newSocket);
    newSocket->m_pPrivate->m_pSslCtx = m_pPrivate->m_pSslCtx;
    if (newSocket->finalizeAccept() == false)
    {
      CCMONITORDELETE(newSocket);
      delete newSocket;
      newSocket = nullptr;
    }
  }
  return newSocket;
}

CcSocketAddressInfo CcSslSocket::getHostByName(const CcString& hostname)
{
  //Retrieve AddressInfo from default Sockets
  CcSocketAbstract* sSocket = CcKernel::getSocket(ESocketType::TCP);
  CcSocketAddressInfo sRetInfo(sSocket->getHostByName(hostname));
  CCMONITORDELETE(sSocket); 
  delete sSocket;
  return CcSocketAddressInfo(sRetInfo);
}

void CcSslSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  m_pPrivate->m_pParentSocket->setTimeout(uiTimeValue);
}

CcSocketAddressInfo CcSslSocket::getPeerInfo(void)
{
  return m_pPrivate->m_pParentSocket->getPeerInfo();
}

void CcSslSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_pPrivate->m_pParentSocket->setPeerInfo(oPeerInfo);
}


bool CcSslSocket::initClient()
{
  bool bRet = false;
  /* Set up the SSL context */
  m_pPrivate->m_pSslCtx = SSL_CTX_new(SSLv23_client_method());
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_1_client_method());
  }
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_client_method());
  }
#ifndef OPENSSL_NO_SSL3
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pPrivate->m_pSslCtx = SSL_CTX_new(SSLv3_client_method());
  }
#endif
  if (m_pPrivate->m_pSslCtx != nullptr)
  {
    CCMONITORNEW(m_pPrivate->m_pSslCtx);
    bRet = true;
    m_pPrivate->m_bSslCtxOwner = true;
  }
  return bRet;
}

bool CcSslSocket::initServer()
{
  bool bRet = false;
#if OPENSSL_VERSION_NUMBER < 0x10100000
  m_pPrivate->m_pSslCtx = SSL_CTX_new(SSLv23_server_method());
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    ERR_print_errors_fp(stdout);
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_1_server_method());
  }
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    ERR_print_errors_fp(stdout);
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_server_method());
  }
#ifndef OPENSSL_NO_SSL3
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    m_pPrivate->m_pSslCtx = SSL_CTX_new(SSLv3_server_method());
  }
#endif
#else
  m_pPrivate->m_pSslCtx = SSL_CTX_new(TLS_server_method());
#endif
  if (m_pPrivate->m_pSslCtx == nullptr)
  {
    bRet = false;
  }
  else
  {
    bRet = true;
    m_pPrivate->m_bSslCtxOwner = true;
  }
  return bRet;
}

void CcSslSocket::deinit()
{
  if (m_pPrivate->m_pSslCtx != nullptr &&
      m_pPrivate->m_bSslCtxOwner == true)
  {
    SSL_CTX_free(m_pPrivate->m_pSslCtx.ptr());
    m_pPrivate->m_pSslCtx = nullptr;
  }
  if (m_pPrivate->m_pSsl != nullptr)
  {
    SSL_free(m_pPrivate->m_pSsl.ptr());
    m_pPrivate->m_pSsl = nullptr;
  }
}

bool CcSslSocket::loadCertificate(const CcString& sCertificateFile)
{
  bool bRet = true;
  if (SSL_CTX_use_certificate_file(m_pPrivate->m_pSslCtx.ptr(), sCertificateFile.getOsPath().getCharString(), SSL_FILETYPE_PEM) != 1)
  {
    uint32 uiError = ERR_get_error();
    char pcErrorBuf[256];
    ERR_error_string(uiError, pcErrorBuf);
    CCDEBUG(CcString("SSL-Error: ") + pcErrorBuf + " Nr: " + CcString::fromNumber(uiError));
    bRet = false;
  }
  return bRet;
}

bool CcSslSocket::loadKey(const CcString& sKeyFile)
{
  bool bRet = true;
  if (SSL_CTX_use_PrivateKey_file(m_pPrivate->m_pSslCtx.ptr(), sKeyFile.getOsPath().getCharString(), SSL_FILETYPE_PEM) != 1)
  {
    CCDEBUG(ERR_func_error_string(ERR_get_error()));
    bRet = false;
  }
  return bRet;
}
bool CcSslSocket::finalizeAccept()
{
  bool bRet = false;
  m_pPrivate->m_pSsl = SSL_new(m_pPrivate->m_pSslCtx.ptr());
  CCMONITORNEW(m_pPrivate->m_pSsl);
  int iStatus = SSL_set_fd(m_pPrivate->m_pSsl.ptr(), m_pPrivate->m_pParentSocket->getSocketFD());
  if (iStatus != 0)
  {
    iStatus = SSL_accept(m_pPrivate->m_pSsl.ptr());
    if (iStatus <= 0)
    {
      char string1000[1000];
      int iErrorNr = SSL_get_error(m_pPrivate->m_pSsl.ptr(), iStatus);
      ERR_error_string_n(iErrorNr, string1000, 1000);
      CCERROR("Error on ssl accept: " + CcString(string1000) + " No. " + CcString::fromNumber(iErrorNr));
    }
    else
    {
      bRet = true;
    }
  }
  return bRet;
}
 
