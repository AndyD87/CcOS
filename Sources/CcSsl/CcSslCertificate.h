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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSslCertificate
 **/
#pragma once

#include "CcBase.h"
#include "CcSsl.h"

// Forward declarations
class CcString;
class CcByteArray;

/**
 * @brief Control openssl library
 */
class CcSslSHARED CcSslCertificate
{
public:
  /**
   * @brief Constructor
   */
  CcSslCertificate();

  /**
   * @brief Destructor
   */
  virtual ~CcSslCertificate();

  /**
   * @brief Add extension using V3 code: we can set the config file as NULL
   *        because we wont reference any other sections.
   * @param nid
   * @param value
   * @return
   */
  int add_ext(int nid, char *value);

  /**
   * @brief Create certificate
   * @param bits:   Number of bits to use
   * @param serial: Serial naumber to create
   * @param days:   Days of validity
   * @return True if certificate was generated
   */
  bool mkcert(int bits, int serial, int days);

  /**
   * @brief Callback from openssl
   * @param p:
   * @param n:
   * @param arg:
   */
  static void callback(int p, int n, void *arg);

  /**
   * @brief on commandline for comparision:
   *        openssl req -config cnf/openssl.cnf -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 10
   * @param sCertFilePath
   * @param sKeyFilePath
   * @return
   */
  bool create(const CcString& sCertFilePath, const CcString& sKeyFilePath);

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr; //!< Private data
};
