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
 * @subpage   CcSslData
 *
 * @page      CcSslData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSslData
 **/
#ifndef CcSslData_H_
#define CcSslData_H_

#include "CcBase.h"
#include "CcSsl.h"

class CcString;
/// @name Forward declarations
struct bio_st;
struct ssl_st;
struct ssl_ctx_st;


/**
 * @brief Control openssl library
 */
class CcSslSHARED CcSslData
{
public:
  /**
   * @brief Constructor
   */
  CcSslData(void);

  /**
   * @brief Destructor
   */
  ~CcSslData(void);

  bool initClient();
  bool initServer();

  bool loadCertificate(const CcString& sCertificateFile);
  bool loadKey(const CcString& sKeyFile);


  ssl_st*& ssl()
    {return m_pSsl;}

  ssl_ctx_st*& ctx()
    {return m_pSslCtx;}

private:
  ssl_st*      m_pSsl;
  ssl_ctx_st*  m_pSslCtx;
};
#endif /* CcSslData_H_ */
