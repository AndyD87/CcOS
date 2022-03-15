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
 * @brief     Class IHttpProvider
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcRegEx.h"
#include "Network/CcSocket.h"
#include "CcHttpWorkData.h"

/**
 * @brief Basic Webpage-Provider for registering Webpages on Server
 */
class CcHttpSHARED IHttpProvider
{
public:
  /**
   * @brief Constructor
   */
  IHttpProvider();

  /**
   * @brief Destructor
   */
  virtual ~IHttpProvider();

  /**
   * @brief Check if path of @p oData is matching with this provider.
   *        A provider can check additional parameters if required.
   * @param oData: Data to check for path
   * @return True if this Provider is target provider
   */
  virtual bool checkResponsible(const CcHttpWorkData &oData) const = 0;

  /**
   * @brief Process raw data set with custom http methods.
   *        If not implementation from inheriting class, IHttpProvider will
   *        forward it to execGet, execPost or execHead.
   * @param oData: Http workset with further informations
   * @return Status of operation
   */
  virtual CcStatus exec(CcHttpWorkData& oData);

  /**
   * @brief Process GET request
   * @param oData: Http workset with further informations
   * @return Status of operation
   */
  virtual CcStatus execGet(CcHttpWorkData& oData);

  /**
   * @brief Process POST request
   * @param oData: Http workset with further informations
   * @return Status of operation
   */
  virtual CcStatus execPost(CcHttpWorkData& oData);

  /**
   * @brief Process HEAD request
   * @param oData: Http workset with further informations
   * @return Status of operation
   */
  virtual CcStatus execHead(CcHttpWorkData& oData);
};
