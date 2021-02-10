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
 * @brief     Class IHttpPathContentProvider
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "IHttpPathProvider.h"

/**
 * @brief A content provider will represent a file for direct output file contents.
 */
class CcHttpSHARED IHttpPathContentProvider : public IHttpPathProvider
{
public:
  /**
   * @brief Constructor
   * @param sPath: Path for content provider
   */
  IHttpPathContentProvider(const CcString& sPath = "");

  /**
   * @brief Destructor
   */
  virtual ~IHttpPathContentProvider();

  /**
   * @brief Override get to parse path for file and direct output file content
   * @param oData: Workset with path to file
   * @return Status of operation.
   */
  virtual CcStatus execGet(CcHttpWorkData& oData) override;

  /**
   * @brief Mimetype of file for header info.
   *        Default is CcHttpGlobalStrings::MimeTypes::ApplicationOctet.
   * @return Mimetype as string
   */
  virtual const CcString& getMimeType();

  /**
   * @brief Get size of file content for header info
   * @param Size: size of file
   * @return Pointer to buffer
   */
  virtual const void* getContent(size_t& Size) = 0;
};
