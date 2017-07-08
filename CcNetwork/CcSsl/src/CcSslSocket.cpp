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


CcSslSocket::CcSslSocket(void) : CcSocket(ESocketType::TCP)
{
  CcSslControl::initSsl();
}

CcSslSocket::CcSslSocket(CcSocket* pParentSocket) :
  CcSocket(ESocketType::TCP),
  m_pParentSocket(pParentSocket)
{
  CcSslControl::initSsl();
}

CcSslSocket::~CcSslSocket(void) 
{
}

bool CcSslSocket::open(EOpenFlags)
{
  return false;
}

bool CcSslSocket::close()
{
  m_pParentSocket->close();
  return true;
}

bool CcSslSocket::cancel(void)
{
  close();
  return true;
}

size_t CcSslSocket::write(const char *buf, size_t bufSize)
{
  size_t uiReturn;
  /* Send the request */
  int uiSendNumber = SSL_write(m_oSslData.ssl(), buf, (int)bufSize);
  if (uiSendNumber <= 0)
  {
    uiReturn = SIZE_MAX;
#ifdef DEBUG
    char string1000[1000];
    int iErrorNr = SSL_get_error(m_oSslData.ssl(), uiSendNumber);
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

size_t CcSslSocket::read(char *buf, size_t bufSize)
{
  size_t uiReturn;
  int uiReadNumber = SSL_read(m_oSslData.ssl(), buf, (int)bufSize);
  if (uiReadNumber <= 0)
  {
    uiReturn = SIZE_MAX;
#ifdef DEBUG
    char string1000[1000];
    int iErrorNr = SSL_get_error(m_oSslData.ssl(), uiReadNumber);
    ERR_error_string_n(iErrorNr, string1000, 1000);
    //CCDEBUG("SSL read error: " + CcString(string1000) + " No. " + CcString::fromNumber(iErrorNr));
#endif
  }
  else
  {
    uiReturn = (size_t)uiReadNumber;
  }
  return uiReturn;
}

bool CcSslSocket::bind(uint16 Port)
{
  m_pParentSocket = CcKernel::getSocket(ESocketType::TCP);
  return m_pParentSocket->bind(Port);
}

bool CcSslSocket::connect(const CcSocketAddressInfo& oAddressInfo)
{
  bool bRet;
  m_oConnectionInfo = oAddressInfo;
  bRet = connect(oAddressInfo.getIPv4String(), oAddressInfo.getPortString());
  return bRet;
}

bool CcSslSocket::connect(const CcString& hostName, const CcString& hostPort)
{
  bool bRet = false;
  if (m_oSslData.ctx() != nullptr)
  {
    /* Setup the connection */
    m_pParentSocket = CcKernel::getSocket(ESocketType::TCP);
    if (m_pParentSocket->connect(hostName, hostPort))
    {
      m_oSslData.ssl() = SSL_new(m_oSslData.ctx());
      SSL_set_fd(m_oSslData.ssl(), m_pParentSocket->getSocketFD());
      int iSocketReturn = SSL_connect(m_oSslData.ssl());
      if (iSocketReturn == -1)
      {
        bRet = false;
      }
      else
      {
        bRet = true;
      }
    }

    //m_oSslData.bio() = BIO_new_ssl_connect(m_oSslData.ctx());
    /* Set the SSL_MODE_AUTO_RETRY flag */
    //BIO_get_ssl(m_oSslData.bio(), &m_oSslData.ssl());
    //SSL_set_mode(m_oSslData.ssl(), SSL_MODE_AUTO_RETRY);

    //BIO_set_conn_hostname(m_oSslData.bio(), hostName.getCharString());
    //BIO_set_conn_port(m_oSslData.bio(), hostPort.getCharString());

    //int32 iResult = BIO_do_connect(m_oSslData.bio());
    //if (iResult != 1)
    //{
    //  uint32 uiError = ERR_get_error();
    //  CCDEBUG(ERR_func_error_string(uiError));
    //  CCDEBUG("CcSslSocket::connect failed, ErrorNr: " + CcString(uiError));
    //  close();
    //}
    //else
    //{
    //  uint32 uiResult = SSL_get_verify_result(m_oSslData.ssl());
    //  /* Check the certificate */
    //  if (uiResult != X509_V_OK)
    //  {
    //    CCDEBUG("CcSslSocket::connect, Certificate error: " + CcSslControl::getErrorString(uiResult));
    //  }
    //  bRet = true;
    //}
  }
  return bRet;
}

bool CcSslSocket::listen(void)
{
  return m_pParentSocket->listen();
}

CcSocket* CcSslSocket::accept(void)
{
  CcSocket* ClientSocket = m_pParentSocket->accept();
  CcSslSocket* newSocket = new CcSslSocket(ClientSocket); 
  CCMONITORNEW(newSocket);
  newSocket->m_oSslData.ctx() = m_oSslData.ctx();
  if (newSocket->finalizeAccept() == false)
  {
    CCMONITORDELETE(newSocket); delete newSocket;
    CCMONITORDELETE(ClientSocket); delete ClientSocket;
    newSocket = nullptr;
  }
  return newSocket;
}

CcSocketAddressInfo CcSslSocket::getHostByName(const CcString& hostname)
{
  //Retrieve AddressInfo from default Sockets
  CcSocket* sSocket = CcKernel::getSocket(ESocketType::TCP);
  CcSocketAddressInfo sRetInfo(sSocket->getHostByName(hostname));
  CCMONITORDELETE(sSocket); delete sSocket;
  return CcSocketAddressInfo(sRetInfo);
}

void CcSslSocket::setTimeout(const CcDateTime& uiTimeValue)
{
  m_pParentSocket->setTimeout(uiTimeValue);
}

bool CcSslSocket::finalizeAccept()
{
  bool bRet = false;
  m_oSslData.ssl() = SSL_new(m_oSslData.ctx());
  int iStatus = SSL_set_fd(m_oSslData.ssl(), m_pParentSocket->getSocketFD());
  if (iStatus != 0)
  {
    iStatus = SSL_accept(m_oSslData.ssl());
    if (iStatus <= 0)
    {
      char string1000[1000];
      int iErrorNr = SSL_get_error(m_oSslData.ssl(), iStatus);
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
 
