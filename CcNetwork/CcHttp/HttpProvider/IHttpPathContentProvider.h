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
 * @subpage   IHttpPathContentProvider
 *
 * @page      IHttpPathContentProvider
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IHttpPathContentProvider
 */
#ifndef H_IHttpPathContentProvider_H_
#define H_IHttpPathContentProvider_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IHttpPathProvider.h"

/**
 * @brief Example Class implementation
 */
class CcHttpSHARED IHttpPathContentProvider : public IHttpPathProvider
{
public:
  /**
   * @brief Constructor
   */
  IHttpPathContentProvider(const CcString& sPath = "");

  /**
   * @brief Destructor
   */
  virtual ~IHttpPathContentProvider();

  virtual CcStatus execGet(CcHttpWorkData& oData) override;

  virtual const CcString& getMimeType();

  virtual const void* getContent(size_t& Size) = 0;
};

#endif // _IHttpPathContentProvider_H_
