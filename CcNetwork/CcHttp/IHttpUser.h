/*
 * This file is part of IHttpUser.
 *
 * IHttpUser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IHttpUser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with IHttpUser.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      IHttpUser
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class IHttpUser
 **/
#ifndef H_IHttpUser_H_
#define H_IHttpUser_H_

#include "CcBase.h"
#include "CcUser.h"

/**
 * @brief IHttpUser implementation
 *        Main class wich is loaded to start Application.
 */
class IHttpUser
{
public:
  /**
   * @brief Constructor
   */
  IHttpUser();

  const CcString& getName() const
  { return m_sName; }

private:
  CcString m_sName;
};

#endif // H_IHttpUser_H_
