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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcUser
 */
#ifndef CCUSER_H_
#define CCUSER_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcGroup.h"
#include "CcPassword.h"
#include "CcListCommon.h"
#include "CcVector.h"

#ifdef WIN32
template class CcKernelSHARED CcVector<CcGroup*>;
#endif

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcUser {
public:
  /**
   * @brief Constructor
   */
  CcUser(void) {}

  /**
   * @brief Constructor
   */
  CcUser(const CcString& Username);

  /**
   * @brief Destructor
   */
  virtual ~CcUser( void );
  bool isUser(const CcString& Username) const;
  virtual bool login(const CcPassword& oPassword) const;

  void setUserName(const CcString& sUserName);
  void setPassword(const CcPassword& oPassword);
  void setPassword(const CcString& oPassword, EPasswordType eType = EPasswordType::SHA256);
  void setMail(const CcString& sMail);
  void setHomeDir(const CcString& sHomeDir);

  const CcString& getUserName(void) const;
  const CcString& getMail(void) const;
  const CcString& getHomeDir(void) const;
  const CcPassword& getPassword(void) const
    { return m_oPassword; }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcUser& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcUser& oToCompare) const
    { return !operator==(oToCompare); }
 

protected:
  CcString m_sUsername;
  CcPassword m_oPassword;
  CcGroupList m_GroupList;
  CcString m_sMail;
  CcString m_sHomeDir;
};

#endif /* CCUSER_H_ */
