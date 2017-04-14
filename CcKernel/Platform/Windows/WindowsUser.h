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
 * @page      WindowsUser
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class WindowsUser
 */
#ifndef WindowsUser_H_
#define WindowsUser_H_

#include "CcBase.h"
#include "WindowsGlobals.h"
#include "CcUser.h"
#include "CcString.h"

/**
 * @brief Example Class impelmentation
 */
class WindowsUser :public CcUser{
public:
  /**
   * @brief Constructor
   */
  WindowsUser(const CcString& Username);

  /**
   * @brief Destructor
   */
  virtual ~WindowsUser( void );

  bool login(const CcPassword& Password) const override;

  void setWindowsHomeDir(const CcString& sHomeDir);
  void setWindowsPassword(const CcString& sPassword);
private:
  CcString m_sHomeDir;
  CcString m_sPassword;
};

#endif /* WindowsUser_H_ */
