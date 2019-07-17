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
 * @page      CcHttp
 * @subpage   CcHttpRestApiProvider
 *
 * @page      CcHttpRestApiProvider
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpRestApiProvider
 */
#ifndef _CcHttpRestApiProvider_H_
#define _CcHttpRestApiProvider_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "HttpProvider/IHttpPathProvider.h"
#include "RestApi/IRestApi.h"

/**
 * @brief Example Class impelmentation
 */
class CcHttpSHARED CcHttpRestApiProvider : public IHttpPathProvider, public IRestApi
{
public:
  /**
   * @brief Constructor
   */
  CcHttpRestApiProvider(const CcString& sRootPath);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpRestApiProvider();

  virtual CcStatus exec(CcHttpWorkData& oData) override;

};

#endif /* _CcHttpRestApiProvider_H_ */
