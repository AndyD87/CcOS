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
 * @subpage   CcHttpWebframeworkJsProvider
 *
 * @page      CcHttpWebframeworkJsProvider
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpWebframeworkJsProvider
 */
#ifndef H_CcHttpWebframeworkJsProvider_H_
#define H_CcHttpWebframeworkJsProvider_H_

#include "CcBase.h"
#include "HttpProvider/IHttpPathContentProvider.h"

namespace NHttp
{
namespace Application
{
namespace RestApiWebframework
{
/**
 * @brief Example Class implementation
 */
class CcHttpWebframeworkJsProvider : public IHttpPathContentProvider
{
public:
  /**
   * @brief Constructor
   */
  CcHttpWebframeworkJsProvider(const CcString& sRcDeviceJsPath = "/CcOSWebframework.js") : IHttpPathContentProvider(sRcDeviceJsPath)
  {
  }

  /**
   * @brief Destructor
   */
  virtual ~CcHttpWebframeworkJsProvider() = default;

  virtual const void* getContent(size_t& Size) override;

  virtual const CcString& getMimeType() override;
};
}
}
}
#endif // H_CcHttpWebframeworkJsProvider_H_
