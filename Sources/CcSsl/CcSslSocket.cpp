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

#ifdef LINUX
#include <err.h>
#endif

class CcSslSocket::CPrivate
{
public:
  CcHandle<ssl_st>      m_pSsl = nullptr;
  CcHandle<ssl_ctx_st>  m_pSslCtx = nullptr;
  bool                  m_bSslCtxOwner = false;
  CcSharedPointer<ISocket>  m_pParentSocket = nullptr;
};

CcSslSocket::CcSslSocket() :
  ISocket(ESocketType::TCP)
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->m_pParentSocket = CcKernel::getSocket(ESocketType::TCP);
  CcSslControl::initSsl();
}

CcSslSocket::CcSslSocket(ISocket* pParentSocket) :
  ISocket(ESocketType::TCP)
{
  CCNEW(m_pPrivate, CPrivate);
  m_pPrivate->m_pParentSocket = pParentSocket;
  CcSslControl::initSsl();
}

CcSslSocket::~CcSslSocket()
{
  deinit();
  CCDELETE(m_pPrivate);
}

void CcSslSocket::operator=(CcSslSocket&& rSocket)
{
  if(this != &rSocket)
  {
    if(m_pPrivate != nullptr)
    {
      deinit();
      CCDELETE(m_pPrivate);
    }
    m_pPrivate = rSocket.m_pPrivate;
    rSocket.m_pPrivate = nullptr;
  }
}

CcStatus CcSslSocket::open(EOpenFlags)
{
  return false;
}

CcStatus CcSslSocket::close()
{
  if(m_pPrivate->m_pParentSocket != nullptr)
  {
    m_pPrivate->m_pParentSocket->close();
  }
  if (m_pPrivate->m_pSsl != nullptr)
  {
    CCMONITORDELETE(m_pPrivate->m_pSsl.ptr());
    SSL_free(m_pPrivate->m_pSsl.ptr());
    m_pPrivate->m_pSsl = nullptr;
  }
  return true;
}

CcStatus CcSslSocket::cancel()
{
  return close();
}

size_t CcSslSocket::write(const void *pBuffer, size_t uBufferSize)
{
  size_t uiReturn = 0;
  if (uBufferSize > 0)
  {
    /* Send the request */
    int uiSendNumber = SSL_write(m_pPrivate->m_pSsl.ptr(), pBuffer, (int) uBufferSize);
    if (uiSendNumber > 0)
    {
      uiReturn = (size_t) uiSendNumber;
    }
    else
    {
      uiReturn = SIZE_MAX;
      int err = SSL_get_error(m_pPrivate->m_pSsl.ptr(), uiSendNumber);
      switch (err)
      {
        case SSL_ERROR_NONE:
          CCDEBUG("SSL_ERROR_NONE");
          break;
        case SSL_ERROR_ZERO_RETURN:
          // peer disconnected...
          break;
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
          // no data available right now, wait a few seconds in case new data arrives...
          CCDEBUG("SSL_ERROR_WANT... so retry to write");
          uiReturn = write(pBuffer, uBufferSize);
          break;
        default:
          CCDEBUG("unknown error on SSL_write");
          break;
      }
    }
  }
  return uiReturn;
}

size_t CcSslSocket::read(void *pBuffer, size_t uBufferSize)
{
  size_t uiReturn = 0;
  if (uBufferSize > 0)
  {
    int uiReadNumber = SSL_read(m_pPrivate->m_pSsl.ptr(), pBuffer, (int) uBufferSize);
    if (uiReadNumber > 0)
    {
      uiReturn = (size_t) uiReadNumber;
    }
    else
    {
      uiReturn = SIZE_MAX;
      int err = SSL_get_error(m_pPrivate->m_pSsl.ptr(), uiReadNumber);
      switch (err)
      {
        case SSL_ERROR_NONE:
          CCDEBUG("SSL_ERROR_NONE");
          break;
        case SSL_ERROR_ZERO_RETURN:
          // peer disconnected...
          break;
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
        {
          // no data available right now, wait a few seconds in case new data arrives...
          size_t uiCounter = 200;
          while(!dataAvailable() && uiCounter-- > 0)
            CcKernel::sleep(10);
          uiReturn = read(pBuffer, uBufferSize);
          break;
        }
        case SSL_ERROR_SYSCALL:
        {
          CCDEBUG_LASTERROR("unknown error on SSL_read");
          break;
        }
        default:
          CCDEBUG("unknown error on SSL_read");
          break;
      }
    }
  }
  return uiReturn;
}

CcStatus CcSslSocket::setAddressInfo(const CcSocketAddressInfo& oAddrInfo)
{
  return m_pPrivate->m_pParentSocket->setAddressInfo(oAddrInfo);
}

CcSocketAddressInfo& CcSslSocket::getAddressInfo()
{
  return m_pPrivate->m_pParentSocket->getAddressInfo();
}

CcStatus CcSslSocket::bind()
{
  return m_pPrivate->m_pParentSocket->bind();
}

CcStatus CcSslSocket::connect()
{
  bool bRet = false;
  if (m_pPrivate->m_pSslCtx != nullptr)
  {
    /* Setup the connection */
    if (m_pPrivate->m_pParentSocket->connect())
    {
      m_pPrivate->m_pSsl = SSL_new(m_pPrivate->m_pSslCtx.ptr());
      CCMONITORNEW(m_pPrivate->m_pSsl.ptr());
      if (SSL_set_fd(m_pPrivate->m_pSsl.ptr(), m_pPrivate->m_pParentSocket->getSocketFD()))
      {
        SSL_ctrl(m_pPrivate->m_pSsl.ptr(), SSL_CTRL_SET_TLSEXT_HOSTNAME, TLSEXT_NAMETYPE_host_name, (void*)m_sHostname.getCharString());
        int iSocketReturn = SSL_connect(m_pPrivate->m_pSsl.ptr());
        if (iSocketReturn < 0)
        {
          getError(iSocketReturn);
          CcSslControl::printAllErrors();
        }
        else
        {
          bRet = true;
        }
      }
    }
  }
  return bRet;
}

CcStatus CcSslSocket::listen()
{
  return m_pPrivate->m_pParentSocket->listen();
}

ISocket* CcSslSocket::accept()
{
  CcSslSocket* newSocket = nullptr;
  ISocket* ClientSocket = m_pPrivate->m_pParentSocket->accept();
  if (ClientSocket != nullptr)
  {
    CCNEW(newSocket, CcSslSocket, ClientSocket);
    newSocket->m_pPrivate->m_pSslCtx = m_pPrivate->m_pSslCtx;
    if (newSocket->finalizeAccept() == false)
    {
      CCDELETE(newSocket);
    }
  }
  return newSocket;
}

CcSocketAddressInfo CcSslSocket::getHostByName(const CcString& sHostname)
{
  //Retrieve AddressInfo from default Sockets
  ISocket* sSocket = CcKernel::getSocket(ESocketType::TCP);
  CcSocketAddressInfo sRetInfo(sSocket->getHostByName(sHostname));
  CCDELETE(sSocket);
  return CcSocketAddressInfo(sRetInfo);
}

CcStatus CcSslSocket::setTimeout(const CcDateTime& uiTimeValue, ERwMode eMode)
{
  return m_pPrivate->m_pParentSocket->setTimeout(uiTimeValue, eMode);
}

CcSocketAddressInfo& CcSslSocket::getPeerInfo()
{
  return m_pPrivate->m_pParentSocket->getPeerInfo();
}

void CcSslSocket::setPeerInfo(const CcSocketAddressInfo& oPeerInfo)
{
  m_pPrivate->m_pParentSocket->setPeerInfo(oPeerInfo);
}

CcStatus CcSslSocket::dataAvailable()
{
  return m_pPrivate->m_pParentSocket->dataAvailable();
}

CcStatus CcSslSocket::setOption(ESocketOption eOption, void* pData, size_t uiDataLen)
{
  if (m_pPrivate->m_pParentSocket != nullptr)
  {
    return m_pPrivate->m_pParentSocket->setOption(eOption, pData, uiDataLen);
  }
  return false;
}

CcStatus CcSslSocket::setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen)
{
  if (m_pPrivate->m_pParentSocket != nullptr)
  {
    return m_pPrivate->m_pParentSocket->setOptionRaw(iLevel, iOptName, pData, uiDataLen);
  }
  return false;
}


bool CcSslSocket::initClient()
{
  bool bRet = false;
  /* Set up the SSL context */
  #if OPENSSL_VERSION_NUMBER < 0x10100000
    m_pPrivate->m_pSslCtx = SSL_CTX_new(SSLv23_client_method());
    if (m_pPrivate->m_pSslCtx == nullptr)
    {
      CCDEBUG(getErrorString(ERR_get_error()));
      m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_1_client_method());
    }
    if (m_pPrivate->m_pSslCtx == nullptr)
    {
      CCDEBUG(getErrorString(ERR_get_error()));
      m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_client_method());
    }
    #ifndef OPENSSL_NO_SSL3
      if (m_pPrivate->m_pSslCtx == nullptr)
      {
        CCDEBUG(getErrorString(ERR_get_error()));
        m_pPrivate->m_pSslCtx = SSL_CTX_new(SSLv3_client_method());
      }
    #endif
  #else
    m_pPrivate->m_pSslCtx = SSL_CTX_new(TLS_client_method());
  #endif
  if (m_pPrivate->m_pSslCtx != nullptr)
  {
    SSL_CTX_set_mode(m_pPrivate->m_pSslCtx.ptr(), SSL_MODE_AUTO_RETRY);
    CCMONITORNEW(m_pPrivate->m_pSslCtx.ptr());
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
      CCDEBUG(getErrorString(ERR_get_error()));
      m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_1_server_method());
    }
    if (m_pPrivate->m_pSslCtx == nullptr)
    {
      ERR_print_errors_fp(stdout);
      CCDEBUG(getErrorString(ERR_get_error()));
      m_pPrivate->m_pSslCtx = SSL_CTX_new(TLSv1_server_method());
    }
  #ifndef OPENSSL_NO_SSL3
    if (m_pPrivate->m_pSslCtx == nullptr)
    {
      CCDEBUG(getErrorString(ERR_get_error()));
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
    SSL_CTX_set_mode(m_pPrivate->m_pSslCtx.ptr(), SSL_MODE_AUTO_RETRY);
    //SSL_CTX_set_verify(m_pPrivate->m_pSslCtx.ptr(), SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, nullptr);
    m_pPrivate->m_bSslCtxOwner = true;
  }
  return bRet;
}

void CcSslSocket::deinit()
{
  close();
  if (m_pPrivate != nullptr)
  {
    if (m_pPrivate->m_pSslCtx != nullptr &&
      m_pPrivate->m_bSslCtxOwner == true)
    {
      CCMONITORDELETE(m_pPrivate->m_pSslCtx.ptr());
      SSL_CTX_free(m_pPrivate->m_pSslCtx.ptr());
      m_pPrivate->m_pSslCtx = nullptr;
    }
    if (m_pPrivate->m_pSsl != nullptr)
    {
      CCMONITORDELETE(m_pPrivate->m_pSsl.ptr());
      SSL_free(m_pPrivate->m_pSsl.ptr());
      m_pPrivate->m_pSsl = nullptr;
    }
  }
}

void CcSslSocket::setHostname(const CcString& sName)
{
  m_sHostname = sName;
}

int CcSslSocket::getError(int iReturnCode)
{
  int iErrorCode = SSL_get_error(m_pPrivate->m_pSsl.ptr(), iReturnCode);
  CCDEBUG(getErrorString(iReturnCode));
  return iErrorCode;
}

CcString CcSslSocket::getErrorString(int iReturnCode)
{
  if (m_pPrivate->m_pSsl.isValid())
  {
    int iErrorCode = SSL_get_error(m_pPrivate->m_pSsl.ptr(), iReturnCode);
    return "openssl-error " + CcString::fromInt(iReturnCode) + ": " + CcString(ERR_reason_error_string(iErrorCode)) + ": " + CcString(ERR_reason_error_string(iErrorCode));
  }
  else
  {
    return "openssl-error Unknown: " + CcString::fromInt(iReturnCode);
  }
}

bool CcSslSocket::loadCertificateFile(const CcString& sCertificateFile)
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

bool CcSslSocket::loadCertificateString(const CcString& sCertificate)
{
  bool bRet = true;
  BIO		*in;
  if ((in = BIO_new_mem_buf(sCertificate.getCharString(), static_cast<int>(sCertificate.length()))) == NULL)
  {
    bRet = false;
  }
  else
  {
    X509* pKey = PEM_read_bio_X509(in, NULL, nullptr, nullptr);
    if (pKey)
    {
      if (SSL_CTX_use_certificate(m_pPrivate->m_pSslCtx.ptr(), pKey) != 1)
      {
        CCDEBUG(getErrorString(ERR_get_error()));
        bRet = false;
      }
      else
      {
        bRet = true;
      }
      X509_free(pKey);
    }
    BIO_free(in);
  }
  return bRet;
}

bool CcSslSocket::loadKeyFile(const CcString& sKeyFile)
{
  bool bRet = true;
  if (SSL_CTX_use_PrivateKey_file(m_pPrivate->m_pSslCtx.ptr(), sKeyFile.getOsPath().getCharString(), SSL_FILETYPE_PEM) != 1)
  {
    CCDEBUG(getErrorString(ERR_get_error()));
    bRet = false;
  }
  else
  {
    // Check pvt key
    if (1 != SSL_CTX_check_private_key(m_pPrivate->m_pSslCtx.ptr()))
    {
      CCDEBUG(getErrorString(ERR_get_error()));
    }
  }
  return bRet;
}

bool CcSslSocket::loadKeyString(const CcString& sKeyFile)
{
  bool bRet = true;
  BIO		*in;
  if ((in = BIO_new_mem_buf(sKeyFile.getCharString(), static_cast<int>(sKeyFile.length()))) == NULL)
  {
    bRet = false;
  }
  else
  {
    EVP_PKEY* pKey = PEM_read_bio_PrivateKey(in, NULL, nullptr, nullptr);
    if (pKey)
    {
      if (SSL_CTX_use_PrivateKey(m_pPrivate->m_pSslCtx.ptr(), pKey) != 1)
      {
        CCDEBUG(getErrorString(ERR_get_error()));
        bRet = false;
      }
      else
      {
        // Check pvt key
        if (1 != SSL_CTX_check_private_key(m_pPrivate->m_pSslCtx.ptr()))
        {
          CCDEBUG(getErrorString(ERR_get_error()));
        }
        else
        {
          bRet = true;
        }
      }
      EVP_PKEY_free(pKey);
    }
    BIO_free(in);
  }
  return bRet;
}

bool CcSslSocket::finalizeAccept()
{
  bool bRet = false;
  m_pPrivate->m_pSsl = SSL_new(m_pPrivate->m_pSslCtx.ptr());
  CCMONITORNEW(m_pPrivate->m_pSsl.ptr());
  int iStatus = SSL_set_fd(m_pPrivate->m_pSsl.ptr(), m_pPrivate->m_pParentSocket->getSocketFD());
  if (iStatus != 0)
  {
    iStatus = SSL_accept(m_pPrivate->m_pSsl.ptr());
    if (iStatus <= 0)
    {
      char pcMessage[1000];
      int iErrorNr = ERR_get_error();
      ERR_error_string_n(iErrorNr, pcMessage, sizeof(pcMessage));
      CCERROR("Error on ssl accept: " + CcString(pcMessage) + " No. " + CcString::fromNumber(iErrorNr));
    }
    else
    {
      bRet = true;
    }
  }
  return bRet;
}
