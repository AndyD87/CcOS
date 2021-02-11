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
 * @brief     Class CcUser
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcGroup.h"
#include "CcPassword.h"
#include "CcGroupList.h"
#include "CcVector.h"
#include "CcHandle.h"

class CcUser;

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<CcGroup*>;
#endif

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcUser
{
public:
  /**
   * @brief Constructor
   */
  CcUser() = default;

  /**
   * @brief Constructor
   */
  CcUser(const CcString& Username);

  /**
   * @brief Destructor
   */
  virtual ~CcUser() = default;
  

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcUser& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcUser& oToCompare) const
  { return !operator==(oToCompare); }

  /**
   * @brief Compare username with string
   * @param Username: Username to compare with
   * @return True if both usernames are matching
   */
  bool isUser(const CcString& Username) const;

  /**
   * @brief Try to login with password.
   * @param oPassword:  Password used for this user
   * @return True if login succeded and password is correct
   */
  virtual bool login(const CcPassword& oPassword);

  //! @return List of groups of user
  CcGroupList& groupList()
  { return m_GroupList; }
  //! @return List of groups of user
  const CcGroupList& getGroupList() const
  { return m_GroupList; }
  //! @return Get username
  const CcString& getUserName() const;
  //! @return Get E-Mail address of user
  const CcString& getMail() const;
  //! @return Get home directory of user
  const CcString& getHomeDir() const;
  //! @return Get password of user, normaly it is encrypred.
  const CcPassword& getPassword() const
  { return m_oPassword; }
  //! @return Get user id
  uint32 getId() const
  { return m_uiId; }

  //! @param sUserName: New username to set
  void setUserName(const CcString& sUserName);
  //! @param oPassword: New password to set
  void setPassword(const CcPassword& oPassword);
  //! @param oPassword: New password to set
  //! @param eType:     Target hashypte of password
  void setPassword(const CcString& oPassword, EHashType eType = EHashType::Sha256);
  //! @param sMail: Set mail address of user
  void setMail(const CcString& sMail);
  //! @param sMail: Set HOME directory of user
  void setHomeDir(const CcString& sHomeDir);


protected:
  CcString m_sUsername;     //!< Internal access to Username
  CcPassword m_oPassword;   //!< Internal access to Password
  CcGroupList m_GroupList;  //!< Internal access to Grouplist
  CcString m_sMail;         //!< Internal access to Mail
  CcString m_sHomeDir;      //!< Internal access to HomeDir
  uint32   m_uiId = 0;      //!< Internal access to ID
};
