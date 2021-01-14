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
#ifndef H_IHttpUserControl_H_
#define H_IHttpUserControl_H_

#include "CcBase.h"
#include "CcList.h"
#include "IHttpUser.h"

class CcHttpWorkData;

/**
 * @brief IHttpUserControl implementation
 *        Main class wich is loaded to start Application.
 */
class IHttpUserControl
{
public:
  /**
   * @brief Constructor
   */
  IHttpUserControl();

  virtual bool checkAuth(CcHttpWorkData& oWorkData);

  virtual IHttpUser* getUser(CcHttpWorkData& oData);
private:
  CcList<IHttpUser> m_oUserList;
};

#endif // H_IHttpUserControl_H_
