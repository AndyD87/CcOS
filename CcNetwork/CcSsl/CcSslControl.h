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
 * @page      CcSsl
 * @subpage   CcSslControl
 *
 * @page      CcSslControl
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSslControl
 **/
#ifndef CcSslControl_H_
#define CcSslControl_H_

#include "CcBase.h"
#include "CcSsl.h"
#include "CcSslData.h"

class CcString;
class CcByteArray;

/**
 * @brief Control openssl library
 */
class CcSslSHARED CcSslControl {
public:
  /**
   * @brief Constructor
   */
  CcSslControl( void );

  /**
   * @brief Destructor
   */
  virtual ~CcSslControl( void );

  static bool initSsl();
  static bool createCert(const CcString& sCertFilePath, const CcString& sKeyFilePath);
  static CcString getErrorString(uint32 uiErrorCode);

private:
  static CcByteArray getBioData(bio_st* pBioData);

private:
  static bool s_bIsInitialized;
};
#endif /* CcSslControl_H_ */
