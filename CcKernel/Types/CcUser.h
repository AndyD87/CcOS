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
 * @page      Types
 * @subpage   CcUser
 *
 * @page      CcUser
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUser
 */
#ifndef H_CcUser_H_
#define H_CcUser_H_

#include "CcBase.h"
#include "CcKernelBase.h"
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

  bool isUser(const CcString& Username) const;
  virtual bool login(const CcPassword& oPassword);

  CcGroupList& groupList()
    {return m_GroupList;}

  void setUserName(const CcString& sUserName);
  void setPassword(const CcPassword& oPassword);
  void setPassword(const CcString& oPassword, EHashType eType = EHashType::Sha256);
  void setMail(const CcString& sMail);
  void setHomeDir(const CcString& sHomeDir);
  const CcGroupList& getGroupList() const
    {return m_GroupList;}

  const CcString& getUserName() const;
  const CcString& getMail() const;
  const CcString& getHomeDir() const;
  const CcPassword& getPassword() const
    { return m_oPassword; }
  uint32 getId() const
    { return m_uiId; }

protected:
  CcString m_sUsername;
  CcPassword m_oPassword;
  CcGroupList m_GroupList;
  CcString m_sMail;
  CcString m_sHomeDir;
  uint32   m_uiId = 0;
};

#endif // H_CcUser_H_
