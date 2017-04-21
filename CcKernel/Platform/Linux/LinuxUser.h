/**
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @page      Linux
 * @subpage   LinuxUser
 *
 * @page      LinuxUser
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2015-10
 * @par       Language   C++ ANSI V3
 * @brief     Class LinuxUser
 */
#ifndef LinuxUser_H_
#define LinuxUser_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcUser.h"

/**
 * @brief Example Class impelmentation
 */
class LinuxUser : public CcUser {
public:
  /**
   * @brief Constructor
   */
  LinuxUser(const CcString Username, const CcString HomeDir );

  /**
   * @brief Destructor
   */
  virtual ~LinuxUser( void );

  /**
   * @brief Check if User/Password combination is correct with Linux User
   * @param Username: Username on Linux System
   * @param Password: Password for User on System
   * @return true, if login is correct
   */
  bool login(const CcString &Password);
};

#endif /* LinuxUser_H_ */
