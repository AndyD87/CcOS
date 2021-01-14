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
 * @brief     Class CcSslControl
 **/
#ifndef H_CcSslControl_H_
#define H_CcSslControl_H_

#include "CcBase.h"
#include "CcSsl.h"


// Forward declarations
class CcString;
class CcByteArray;
struct bio_st;

/**
 * @brief Control openssl library
 */
class CcSslSHARED CcSslControl {
public:
  /**
   * @brief Initialize Ssl subsystem (openssl)
   * @return true if all succeded.
   */
  static bool initSsl();

  /**
   * @brief Create a local Certificate (RSA 2048) for secure communication.
   *        This Method will create a simple certifcate with some default values as paramter.
   *
   * @param sCertFilePath: Path to output file for certificate
   * @param sKeyFilePath:  Path to output file for key.
   * @return
   */
  static bool createCert(const CcString& sCertFilePath, const CcString& sKeyFilePath);

  /**
   * @brief Get matching error string to error number.
   * @param uiErrorCode: Errorcode from varias return statements.
   * @return Errormessage or empty if not found.
   */
  static CcString getErrorString(uint32 uiErrorCode);

  /**
   * @brief Get date content of an binary input/output data.
   * @param pBioData
   * @return
   */
  static CcByteArray getBioData(bio_st* pBioData);

private:
  CcSslControl() = delete;
  ~CcSslControl() = delete;

private:
  static bool s_bIsInitialized;  //!< avoid double initializing by setting this to true after first run.
};
#endif // H_CcSslControl_H_
