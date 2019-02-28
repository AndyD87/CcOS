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
 * @page      CcSql
 * @subpage   CcWmiInterface
 *
 * @page      CcWmiInterface
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWmiInterface
 */
#pragma once

#ifndef _CcWmiInterface_H_
#define _CcWmiInterface_H_

#include <Wbemidl.h>
#include "CcBase.h"
#include "CcSql.h"
#include "CcISqlDatabase.h"

/**
 * @brief Class for Handiling requests to windows through the
 *        Windows Management Instrumentation (WMI) Interface.
 */
class CcSqlSHARED CcWmiInterface : public CcISqlDatabase
{
public:
  /**
   * @brief Constructor
   */
  CcWmiInterface();
  
  /**
   * @brief Destructor
   */
  virtual ~CcWmiInterface();
  
  /**
   * @brief Connect with Interface to Windows COM-System
   * @return true if connection was successfully;
   */
  CcStatus open();

  /**
  * @brief Disconnect from Interface
  */
  CcStatus close();

  /**
  * @brief Send a query to Database
  * @param queryString: SQL-Request as String
  * @return Table with content
  */
  CcSqlResult query(const CcString& queryString);

private:
  IWbemLocator  *m_pLoc;    ///< Handle to Interface-Location in System
  IWbemServices *m_pSvc;    ///< Handle to Interface-Service for requests
};

#endif // _CcWmiInterface_H_
