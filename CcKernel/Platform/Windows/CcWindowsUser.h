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
 * @page      Windows
 * @subpage   CcWindowsUser
 *
 * @page      CcWindowsUser
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWindowsUser
 */
#ifndef CcWindowsUser_H_
#define CcWindowsUser_H_

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "CcUser.h"
#include "CcString.h"

/**
 * @brief Example Class impelmentation
 */
class CcWindowsUser :public CcUser{
public:
  /**
   * @brief Constructor
   */
  CcWindowsUser(const CcString& Username);

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsUser( void );

  bool login(const CcPassword& Password) const override;

  void setWindowsHomeDir(const CcString& sHomeDir);
  void setWindowsPassword(const CcString& sPassword);
private:
  CcString m_sHomeDir;
  CcString m_sPassword;
};

#endif /* CcWindowsUser_H_ */
