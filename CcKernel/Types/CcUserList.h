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
 * @brief     Class CcUserList
 */
#pragma once

#include "CcBase.h"
#include "CcBase.h"
#include "CcUser.h"
#include "CcList.h"
#include "CcReferenceCount.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcUser>;
#endif

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcUserList : public CcList<CcUser*>
{
public:
  /**
   * @brief Constructor
   */
  CcUserList();
  ~CcUserList();

  /**
   * @brief Constructor with initial list
   * @param oToCopy: List to copy from
   */
  CcUserList(const CcUserList& oToCopy);

  /**
   * @brief Constructor with initial list
   * @param oToCopy: List to copy from
   * @return Handle to this.
   */
  CcUserList& operator=(const CcUserList& oToCopy) = default;

  /**
   * @brief Find user in list by name
   * @param Username: Name to serach for
   * @return Pointer to user in list
   */
  CcUser* findUser(const CcString& Username);
  //! @copydoc findUser()
  CcUser* findUser(const CcString& Username) const;

  /**
   * @brief Find user and try to login
   * @param Username: Username to sarch
   * @param Password: Password for login
   * @return Pointer to user if successfully, otherwise nullptr.
   */
  CcUser* findUserPassword(const CcString& Username, const CcString& Password);

  /**
   * @brief Check if user can be found by name in list
   * @param Username: Name to serach for
   * @return true if found, otherwise false
   */
  bool containsUser(const CcString& Username) const;

  //! @brief Get current used user
  inline CcUser* currentUser() const
  { return m_CurrentUser; }
  
  //! @param Username: Change current user
  bool setCurrentUser(const CcString& Username);
private:
  CcReferenceCount* m_pReference = nullptr;
  CcUser*           m_CurrentUser = nullptr;
};
