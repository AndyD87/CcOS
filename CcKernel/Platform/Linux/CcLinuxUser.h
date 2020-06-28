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
 * @subpage   CcLinuxUser
 *
 * @page      CcLinuxUser
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Class CcLinuxUser
 */
#ifndef H_CcLinuxUser_H_
#define H_CcLinuxUser_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcUser.h"

/**
 * @brief Example Class impelmentation
 */
class CcLinuxUser : public CcUser
{
public:
  /**
   * @brief Constructor
   */
  CcLinuxUser(const CcString& Username, const CcString& HomeDir, uint32 iUserId, bool bIsOwner = false);

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxUser();

  /**
   * @brief Check if User/Password combination is correct with Linux User
   * @param Username: Username on Linux System
   * @param Password: Password for User on System
   * @return true, if login is correct
   */
  virtual bool login(const CcPassword &Password) override;
};

#endif // H_CcLinuxUser_H_
