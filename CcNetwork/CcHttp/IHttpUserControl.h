/*
 * This file is part of IHttpUserControl.
 *
 * IHttpUserControl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IHttpUserControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with IHttpUserControl.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class IHttpUserControl
 **/
#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "IHttpUser.h"

class CcHttpWorkData;

/**
 * @brief Basic user control interface
 */
class IHttpUserControl
{
public:
  /**
   * @brief Constructor
   */
  IHttpUserControl();

  /**
   * @brief Check if user, parsed from @p oWorkData, is valid.
   * @param oWorkData: Incoming data from webserver
   * @return True if valid user is set
   */
  virtual bool checkAuth(CcHttpWorkData& oWorkData);

  /**
   * @brief Get valid user by parsing Input form @p oWorkData
   * @param oData: Data to parse
   * @return Pointer to valid user if found or nullptr if no valid
   *          User could be found in @p oWorkData
   */
  virtual IHttpUser* getUser(CcHttpWorkData& oData);
private:
  CcList<IHttpUser> m_oUserList;
};
